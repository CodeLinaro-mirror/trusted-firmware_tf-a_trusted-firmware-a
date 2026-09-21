/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>

#include <arch.h>
#include <arch_features.h>
#include <common/debug.h>
#include <lib/extensions/rme.h>
#include <lib/gpt_rme/gpt_rme.h>
#include <lib/smccc.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <services/firme_svc.h>
#include <services/firme/firme_granule_mgmt.h>
#include "firme_gm_private.h"

static firme_gpt_config_t firme_gpt_config;

/*
 * This table allows us to easily look up GPI-specific information such as
 * descriptors, nse/nse2 fields, and allowed transitions without using large
 * blocks of nested conditionals which both take up a lot of space and are
 * very slow.
 *
 * This array contains an entry for each valid GPI, unused values are all zeros.
 * GPIs such as SA, NSP, and NSO are not part of base FEAT_RME so those policy
 * flags are set at runtime when FEAT_RME_GDI and FEAT_RME_GPC2 are enabled.
 *
 * uint64_t desc       The L1 descriptor associated with a GPI
 * uint8_t nse         NSE bits
 * uint8_t nse2        NSE2 bits
 * uint16_t policy[3] Contains bit fields representing which GPIs a given
 *                     security state can transition this GPI to. 0=s, 1=ns, and
 *                     0x2=realm.
 */
static firme_gpi_config_t firme_gpi_config[] = {
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ GPT_L1_SA_DESC, 0, GPT_NSE2_SA, { 0x0, 0x0, 0x0 } },
	{ GPT_L1_NSP_DESC, 0, GPT_NSE2_NSP, { 0x0, 0x0, 0x0 } },
	{ 0 },
	{ 0 },
	{ GPT_L1_SECURE_DESC,
	  GPT_NSE_SECURE,
	  0,
	  { (1 << GPT_GPI_NS), 0x0, 0x0 } },
	{ GPT_L1_NS_DESC,
	  GPT_NSE_NS,
	  0,
	  { (1 << GPT_GPI_SECURE), 0x0, (1 << GPT_GPI_REALM) } },
	{ GPT_L1_ROOT_DESC, GPT_NSE_ROOT, 0, { 0x0, 0x0, 0x0 } },
	{ GPT_L1_REALM_DESC, GPT_NSE_REALM, 0, { 0x0, 0x0, (1 << GPT_GPI_NS) } },
	{ 0 },
	{ GPT_L1_NSO_DESC, GPT_NSE_NS, 0, { 0x0, 0x0, 0x0 } },
	{ 0 },
	{ 0 },
};

/* Get the descriptor or NSE bits from GPI encoding. */
#define GPI_TO_DESC(_gpi)	(firme_gpi_config[_gpi].desc)
#define GPI_TO_NSE(_gpi)	\
	(((uint64_t)firme_gpi_config[_gpi].nse << GPT_NSE_SHIFT) | \
	 ((uint64_t)firme_gpi_config[_gpi].nse2 << GPT_NSE2_SHIFT))

/* Get the index into the L1 table from a physical address */
#define GPT_L1_INDEX(_pa)	\
	(((_pa) >> (unsigned int)GPT_L1_IDX_SHIFT(firme_gpt_config.pgs_bits)) & GPT_L1_IDX_MASK(firme_gpt_config.pgs_bits))

/* Number of 128-bit L1 entries in 2MB, 32MB and 512MB */
#define L1_QWORDS_2MB	(firme_gpt_config.l1_cnt_2mb / 2U)
#define L1_QWORDS_32MB	(L1_QWORDS_2MB * 16U)
#define L1_QWORDS_512MB	(L1_QWORDS_32MB * 16U)

/* Size in bytes of L1 entries in 2MB, 32MB */
#define L1_BYTES_2MB	(firme_gpt_config.l1_cnt_2mb * sizeof(uint64_t))
#define L1_BYTES_32MB	(L1_BYTES_2MB * 16U)

/* an unused sec_state value to represent ROOT state BIT5_NSE=1 BIT0_NS=0*/
#define SEC_STATE_ROOT	U(0x20)

#ifndef ALIGN_UP
#define ALIGN_UP(num, align)	(((num) + ((align) - 1)) & ~((align) - 1))
#endif

#define IS_ALIGNED(val, align)	(val == ALIGN_UP(val, align))

/* Granule size */
#define GPT_GSIZE		(1UL << (uint32_t)(firme_gpt_config.pgs_bits))

#if (RME_GPT_MAX_BLOCK == 512)
#define PA_RANGE_SIZE	(SZ_512M)
#elif (RME_GPT_MAX_BLOCK == 32)
#define PA_RANGE_SIZE	(SZ_32M)
#elif (RME_GPT_MAX_BLOCK == 2)
#define PA_RANGE_SIZE	(SZ_2M)
#else /* set default PA_RANGE_SIZE to 4MB, when contig is not used  */
#define PA_RANGE_SIZE	(SZ_4M)
#endif

#define INVALID_LOCK_IDX	U(0xff)

static int gm_gpi_pa_range_lock(uint64_t par_base, uint32_t *lock_index_ret)
{
	struct firme_gm_reservation reservation = {
		.slot = FIRME_GM_INVALID_RESERVATION
	};
	struct firme_gm_range range = {
		.base = par_base,
		.size = PA_RANGE_SIZE,
	};
	int rc;

	if (lock_index_ret == NULL) {
		return -EINVAL;
	}

	rc = firme_gm_reserve_ranges(&range, 1U, &reservation);
	if (rc == 0) {
		*lock_index_ret = reservation.slot;
	}

	return rc;
}

static void gm_gpi_pa_range_unlock(uint32_t lock_index)
{
	struct firme_gm_reservation reservation = {
		.slot = lock_index
	};

	firme_gm_release_reservation(&reservation);
}

static void fill_desc(uint64_t *l1, uint64_t l1_desc, unsigned int cnt)
{
	uint128_t *l1_quad = (uint128_t *)l1;
	uint128_t l1_quad_desc = (uint128_t)l1_desc | ((uint128_t)l1_desc << 64);

	VERBOSE("GPT: %s(%p 0x%"PRIx64" %u)\n", __func__, l1, l1_desc, cnt);

	for (unsigned int i = 0U; i < cnt; i++) {
		*l1_quad++ = l1_quad_desc;
	}
}

/*
 * Helper function to check if all L1 entries in 2MB block have
 * the same Granules descriptor value.
 *
 * Parameters
 *   base		Base address of the region to be checked
 *   gpi_info		Pointer to 'gpt_config_t' structure
 *   l1_desc		GPT Granules descriptor with all entries
 *			set to the same GPI.
 *
 * Return
 *   true if L1 all entries have the same descriptor value, false otherwise.
 */
__unused static bool check_fuse_2mb(uint64_t base,
				    const firme_gpi_info_t *gpi_info,
				    uint64_t l1_desc)
{
	/* Last L1 entry index in 2MB block */
	unsigned int long idx = GPT_L1_INDEX(ALIGN_2MB(base)) +
						firme_gpt_config.l1_cnt_2mb - 1UL;

	/* Number of L1 entries in 2MB block */
	unsigned int cnt = firme_gpt_config.l1_cnt_2mb;

	/*
	 * Start check from the last L1 entry and continue until the first
	 * non-matching to the passed Granules descriptor value is found.
	 */
	while (cnt-- != 0U) {
		if (gpi_info->gpt_l1_addr[idx--] != l1_desc) {
			/* Non-matching L1 entry found */
			return false;
		}
	}

	return true;
}

__unused static void fuse_2mb(uint64_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	/* L1 entry index of the start of 2MB block */
	unsigned long idx_2 = GPT_L1_INDEX(ALIGN_2MB(base));

	/* 2MB Contiguous descriptor */
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 2MB);

	VERBOSE("GPT: %s(0x%"PRIxPTR" 0x%"PRIx64")\n", __func__, base, l1_desc);

	fill_desc(&gpi_info->gpt_l1_addr[idx_2], l1_cont_desc, L1_QWORDS_2MB);
}

/*
 * Helper function to check if all 1st L1 entries of 2MB blocks
 * in 32MB have the same 2MB Contiguous descriptor value.
 *
 * Parameters
 *   base		Base address of the region to be checked
 *   gpi_info		Pointer to 'gpt_config_t' structure
 *   l1_desc		GPT Granules descriptor.
 *
 * Return
 *   true if all L1 entries have the same descriptor value, false otherwise.
 */
__unused static bool check_fuse_32mb(uint64_t base, const firme_gpi_info_t *gpi_info,
					uint64_t l1_desc)
{
	/* The 1st L1 entry index of the last 2MB block in 32MB */
	unsigned long idx = GPT_L1_INDEX(ALIGN_32MB(base)) +
					(15UL * firme_gpt_config.l1_cnt_2mb);

	/* 2MB Contiguous descriptor */
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 2MB);

	/* Number of 2MB blocks in 32MB */
	unsigned int cnt = 16U;

	/* Set the first L1 entry to 2MB Contiguous descriptor */
	gpi_info->gpt_l1_addr[GPT_L1_INDEX(ALIGN_2MB(base))] = l1_cont_desc;

	/*
	 * Start check from the 1st L1 entry of the last 2MB block and
	 * continue until the first non-matching to 2MB Contiguous descriptor
	 * value is found.
	 */
	while (cnt-- != 0U) {
		if (gpi_info->gpt_l1_addr[idx] != l1_cont_desc) {
			/* Non-matching L1 entry found */
			return false;
		}
		idx -= firme_gpt_config.l1_cnt_2mb;
	}

	return true;
}

__unused static void fuse_32mb(uint64_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	/* L1 entry index of the start of 32MB block */
	unsigned long idx_32 = GPT_L1_INDEX(ALIGN_32MB(base));

	/* 32MB Contiguous descriptor */
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 32MB);

	VERBOSE("GPT: %s(0x%"PRIxPTR" 0x%"PRIx64")\n", __func__, base, l1_desc);

	fill_desc(&gpi_info->gpt_l1_addr[idx_32], l1_cont_desc, L1_QWORDS_32MB);
}

/*
 * Helper function to check if all 1st L1 entries of 32MB blocks
 * in 512MB have the same 32MB Contiguous descriptor value.
 *
 * Parameters
 *   base		Base address of the region to be checked
 *   gpi_info		Pointer to 'gpt_config_t' structure
 *   l1_desc		GPT Granules descriptor.
 *
 * Return
 *   true if all L1 entries have the same descriptor value, false otherwise.
 */
__unused static bool check_fuse_512mb(uint64_t base, const firme_gpi_info_t *gpi_info,
					uint64_t l1_desc)
{
	/* The 1st L1 entry index of the last 32MB block in 512MB */
	unsigned long idx = GPT_L1_INDEX(ALIGN_512MB(base)) +
					(15UL * 16UL * firme_gpt_config.l1_cnt_2mb);

	/* 32MB Contiguous descriptor */
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 32MB);

	/* Number of 32MB blocks in 512MB */
	unsigned int cnt = 16U;

	/* Set the first L1 entry to 2MB Contiguous descriptor */
	gpi_info->gpt_l1_addr[GPT_L1_INDEX(ALIGN_32MB(base))] = l1_cont_desc;

	/*
	 * Start check from the 1st L1 entry of the last 32MB block and
	 * continue until the first non-matching to 32MB Contiguous descriptor
	 * value is found.
	 */
	while (cnt-- != 0U) {
		if (gpi_info->gpt_l1_addr[idx] != l1_cont_desc) {
			/* Non-matching L1 entry found */
			return false;
		}
		idx -= 16UL * firme_gpt_config.l1_cnt_2mb;
	}

	return true;
}

__unused static void fuse_512mb(uint64_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	/* L1 entry index of the start of 512MB block */
	unsigned long idx_512 = GPT_L1_INDEX(ALIGN_512MB(base));

	/* 512MB Contiguous descriptor */
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 512MB);

	VERBOSE("GPT: %s(0x%"PRIxPTR" 0x%"PRIx64")\n", __func__, base, l1_desc);

	fill_desc(&gpi_info->gpt_l1_addr[idx_512], l1_cont_desc, L1_QWORDS_512MB);
}

/*
 * Helper function to convert GPI entries in a single L1 table
 * from Granules to Contiguous descriptor.
 *
 * Parameters
 *   base		Base address of the region to be written
 *   gpi_info		Pointer to 'gpt_config_t' structure
 *   l1_desc		GPT Granules descriptor with all entries
 *			set to the same GPI.
 */
__unused static void fuse_block(uint64_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	/* Start with check for 2MB block */
	if (!check_fuse_2mb(base, gpi_info, l1_desc)) {
		/* Check for 2MB fusing failed */
		return;
	}

#if (RME_GPT_MAX_BLOCK == 2)
	fuse_2mb(base, gpi_info, l1_desc);
#else
	/* Check for 32MB block */
	if (!check_fuse_32mb(base, gpi_info, l1_desc)) {
		/* Check for 32MB fusing failed, fuse to 2MB */
		fuse_2mb(base, gpi_info, l1_desc);
		return;
	}

#if (RME_GPT_MAX_BLOCK == 32)
	fuse_32mb(base, gpi_info, l1_desc);
#else
	/* Check for 512MB block */
	if (!check_fuse_512mb(base, gpi_info, l1_desc)) {
		/* Check for 512MB fusing failed, fuse to 32MB */
		fuse_32mb(base, gpi_info, l1_desc);
		return;
	}

	/* Fuse to 512MB */
	fuse_512mb(base, gpi_info, l1_desc);

#endif	/* RME_GPT_MAX_BLOCK == 32 */
#endif	/* RME_GPT_MAX_BLOCK == 2 */
}

static void shatter_2mb(uintptr_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	unsigned long idx = GPT_L1_INDEX(ALIGN_2MB(base));

	VERBOSE("GPT: %s(0x%"PRIxPTR" 0x%"PRIx64")\n",
				__func__, base, l1_desc);

	/* Convert 2MB Contiguous block to Granules */
	fill_desc(&gpi_info->gpt_l1_addr[idx], l1_desc, L1_QWORDS_2MB);
}

static void shatter_32mb(uintptr_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	unsigned long idx = GPT_L1_INDEX(ALIGN_2MB(base));
	const uint64_t *l1_gran = &gpi_info->gpt_l1_addr[idx];
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 2MB);
	uint64_t *l1;

	VERBOSE("GPT: %s(0x%"PRIxPTR" 0x%"PRIx64")\n",
				__func__, base, l1_desc);

	/* Get index corresponding to 32MB aligned address */
	idx = GPT_L1_INDEX(ALIGN_32MB(base));
	l1 = &gpi_info->gpt_l1_addr[idx];

	/* 16 x 2MB blocks in 32MB */
	for (unsigned int i = 0U; i < 16U; i++) {
		/* Fill with Granules or Contiguous descriptors */
		fill_desc(l1, (l1 == l1_gran) ? l1_desc : l1_cont_desc,
							L1_QWORDS_2MB);
		l1 = (uint64_t *)((uintptr_t)l1 + L1_BYTES_2MB);
	}
}

static void shatter_512mb(uintptr_t base, const firme_gpi_info_t *gpi_info,
				uint64_t l1_desc)
{
	unsigned long idx = GPT_L1_INDEX(ALIGN_32MB(base));
	const uint64_t *l1_32mb = &gpi_info->gpt_l1_addr[idx];
	uint64_t l1_cont_desc = GPT_L1_CONT_DESC(l1_desc, 32MB);
	uint64_t *l1;

	VERBOSE("GPT: %s(0x%"PRIxPTR" 0x%"PRIx64")\n",
				__func__, base, l1_desc);

	/* Get index corresponding to 512MB aligned address */
	idx = GPT_L1_INDEX(ALIGN_512MB(base));
	l1 = &gpi_info->gpt_l1_addr[idx];

	/* 16 x 32MB blocks in 512MB */
	for (unsigned int i = 0U; i < 16U; i++) {
		if (l1 == l1_32mb) {
			/* Shatter this 32MB block */
			shatter_32mb(base, gpi_info, l1_desc);
		} else {
			/* Fill 32MB with Contiguous descriptors */
			fill_desc(l1, l1_cont_desc, L1_QWORDS_32MB);
		}

		l1 = (uint64_t *)((uintptr_t)l1 + L1_BYTES_32MB);
	}
}

/*
 * Helper function to convert GPI entries in a single L1 table
 * from Contiguous to Granules descriptor. This function updates
 * descriptor to Granules in passed 'gpt_config_t' structure as
 * the result of shuttering.
 *
 * Parameters
 *   base		Base address of the region to be written
 *   gpi_info		Pointer to 'gpt_config_t' structure
 *   l1_desc		GPT Granules descriptor set this range to.
 */
__unused static void shatter_block(uint64_t base, firme_gpi_info_t *gpi_info,
				   uint64_t l1_desc)
{
	/* Look-up table for 2MB, 32MB and 512MB locks shattering */
	static const gpt_shatter_func gpt_shatter_lookup[] = {
		shatter_2mb,
		shatter_32mb,
		shatter_512mb
	};

	/* Look-up table for invalidation TLBs for 2MB, 32MB and 512MB blocks */
	static const gpt_tlbi_lookup_t tlbi_lookup[] = {
		{ tlbirpalos_2m, ~(SZ_2M - 1UL) },
		{ tlbirpalos_32m, ~(SZ_32M - 1UL) },
		{ tlbirpalos_512m, ~(SZ_512M - 1UL) }
	};

	/* Get shattering level from Contig field of Contiguous descriptor */
	unsigned long level = GPT_L1_CONT_CONTIG(gpi_info->gpt_l1_desc) - 1UL;

	/* Shatter contiguous block */
	gpt_shatter_lookup[level](base, gpi_info, l1_desc);

	tlbi_lookup[level].function(base & tlbi_lookup[level].mask);
	dsbosh();

	/*
	 * Update 'gpt_config_t' structure's descriptor to Granules to reflect
	 * the shattered GPI back to caller.
	 */
	gpi_info->gpt_l1_desc = l1_desc;
}

static int firme_gm_active_l1_map(uint64_t base,
				 struct firme_gm_mapping *mapping,
				 uint64_t **table)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	uint64_t descriptor;
	uint64_t l0_index;
	uint64_t table_pa;
	uint64_t table_end;
	uint64_t *l0_base;
	int rc;

	if ((mapping == NULL) || (table == NULL)) {
		return -EINVAL;
	}

	l0_base = (uint64_t *)geometry->l0_base;
	l0_index = base >> geometry->par_bits;
	descriptor = __atomic_load_n(&l0_base[l0_index],
				     __ATOMIC_ACQUIRE);
	if (GPT_L0_TYPE(descriptor) != GPT_L0_TYPE_TBL_DESC) {
		return -ENOENT;
	}

	table_pa = (uint64_t)(uintptr_t)GPT_L0_TBLD_ADDR(descriptor);
	if ((table_pa == 0U) ||
	    ((table_pa & (geometry->l1_table_size - 1U)) != 0U) ||
	    !firme_gm_range_end(table_pa, geometry->l1_table_size, &table_end) ||
	    (table_end > geometry->pps_size)) {
		return -EINVAL;
	}

	rc = firme_gm_map_root(table_pa, geometry->l1_table_size, mapping);
	if (rc == -EPERM) {
		/*
		 * Preserve the legacy identity mapping used for boot-created GPTs
		 * on platforms which have not supplied the static-mapping hook.
		 */
		mapping->va = (uintptr_t)table_pa;
		mapping->size = geometry->l1_table_size;
		mapping->dynamic = false;
		rc = 0;
	}
	if (rc != 0) {
		return rc;
	}

	if (__atomic_load_n(&l0_base[l0_index], __ATOMIC_ACQUIRE) !=
	    descriptor) {
		firme_gm_unmap_root(mapping);
		return -EINPROGRESS;
	}

	*table = (uint64_t *)mapping->va;
	return 0;
}

static void read_gpi(uint64_t base, uint64_t *table,
		     firme_gpi_info_t *gpi_info)
{
	assert(table != NULL);

	/* Get the table index and GPI shift from PA */
	gpi_info->gpt_l1_addr = table;
	gpi_info->idx = (unsigned int)GPT_L1_INDEX(base);
	gpi_info->gpi_shift =
		GPT_L1_GPI_IDX(firme_gpt_config.pgs_bits, base) << 2;

	gpi_info->gpt_l1_desc = (gpi_info->gpt_l1_addr)[gpi_info->idx];

	if ((gpi_info->gpt_l1_desc & GPT_L1_TYPE_CONT_DESC_MASK) ==
				 GPT_L1_TYPE_CONT_DESC) {
		/* Read GPI from Contiguous descriptor */
		gpi_info->gpi = (unsigned int)GPT_L1_CONT_GPI(gpi_info->gpt_l1_desc);
	} else {
		/* Read GPI from Granules descriptor */
		gpi_info->gpi = (unsigned int)((gpi_info->gpt_l1_desc >> gpi_info->gpi_shift) &
						GPT_L1_GRAN_DESC_GPI_MASK);
	}
}

/*
 * This function checks to see if the target GPI value is valid that is passed
 * to FIRME GPI set ABI
 *
 * Parameters
 *   gpi		GPI to check for validity.
 *
 * Return
 *   true for a valid GPI, false for an invalid one.
 */
static bool firme_is_target_gpi_valid(uint8_t sec_state, uint8_t tgpi)
{
	bool tgpi_valid = false;

	switch (sec_state) {
	case SMC_FROM_NON_SECURE:
		if ((tgpi == GPT_GPI_NS) ||
		    (is_feat_rme_gpc2_supported() && (tgpi == GPT_GPI_NSO)) ||
		    (is_feat_rme_gdi_supported() && ((tgpi == GPT_GPI_NSP) ||
						     (tgpi == GPT_GPI_SA)))) {
			tgpi_valid = true;
		}
		break;
	case SMC_FROM_SECURE:
		if ((tgpi == GPT_GPI_NS) || (tgpi == GPT_GPI_SECURE)) {
			tgpi_valid = true;
		}
		break;
#if ENABLE_RMM
	case SMC_FROM_REALM:
		if ((tgpi == GPT_GPI_NS) || (tgpi == GPT_GPI_REALM)) {
			tgpi_valid = true;
		}
		break;
#endif
	case SEC_STATE_ROOT:
		if ((tgpi == GPT_GPI_NS) || (tgpi == GPT_GPI_ROOT)) {
			tgpi_valid = true;
		}
		break;
	default:
		break;
	}

	return tgpi_valid;
}

bool firme_gm_is_gpi_valid(uint8_t gpi)
{
	switch (gpi) {
	case GPT_GPI_SECURE:
	case GPT_GPI_NS:
	case GPT_GPI_ROOT:
	case GPT_GPI_REALM:
	case GPT_GPI_ANY:
		return true;
	case GPT_GPI_NSO:
		return is_feat_rme_gpc2_supported();
	case GPT_GPI_SA:
	case GPT_GPI_NSP:
		return is_feat_rme_gdi_supported();
	default:
		return false;
	}
}

static void tlbi_page_dsbosh(uintptr_t base)
{
	uint8_t pgs = EXTRACT(GPCCR_PGS, read_gpccr_el3());
	/* Look-up table for invalidation TLBs for 4KB, 16KB and 64KB pages */
	static const gpt_tlbi_lookup_t tlbi_page_lookup[] = {
		{ tlbirpalos_4k, ~(SZ_4K - 1UL) },
		{ tlbirpalos_64k, ~(SZ_64K - 1UL) },
		{ tlbirpalos_16k, ~(SZ_16K - 1UL) }
	};


	tlbi_page_lookup[pgs].function(base & tlbi_page_lookup[pgs].mask);
	dsbosh();
}

/*
 * A helper to write the value (target_pas << gpi_shift) to the index of
 * the gpt_l1_addr.
 */
static inline void write_gpt(uint64_t *gpt_l1_desc, uint64_t *gpt_l1_addr,
			     unsigned int gpi_shift, unsigned int idx,
			     unsigned int target_pas)
{
	*gpt_l1_desc &= ~(GPT_L1_GRAN_DESC_GPI_MASK << gpi_shift);
	*gpt_l1_desc |= ((uint64_t)target_pas << gpi_shift);
	gpt_l1_addr[idx] = *gpt_l1_desc;

	dsboshst();
}

static inline bool is_gpi_transition_permitted(uint8_t caller,
					       uint8_t current_gpi,
					       uint8_t target_gpi)
{
	/*
	 * Based on FIRME rule, if source amd target GPI are same then skip
	 * GPT update.
	 */
	if (current_gpi == target_gpi) {
		return true;
	}

	/* Allow transition to/from GPI_ROOT/GPI_NS only for caller state ROOT */
	if (caller == SEC_STATE_ROOT) {
		if (((current_gpi == GPT_GPI_NS) ||
		     (current_gpi == GPT_GPI_ROOT)) &&
		    ((target_gpi == GPT_GPI_NS) ||
		     (target_gpi == GPT_GPI_ROOT))) {
			return true;
		} else {
			return false;
		}
	}

	/*
	 * So we can use a small lookup table, change caller security state 0x21
	 * (from realm) to 0x2 so it can be an index.
	 */
	if (caller == SMC_FROM_REALM) {
		caller = 0x2;
	}

	assert(caller <= 0x2);
	assert(current_gpi <= GPT_GPI_ANY);

	return (firme_gpi_config[current_gpi].policy[caller] >> target_gpi) & 0x1;
}

static inline void gpt_write_entry(uint64_t base, uint8_t target_gpi,
				   firme_gpi_info_t *gpi_info)
{
	/* Update the GPI entry to the new state. */
	write_gpt(&gpi_info->gpt_l1_desc, gpi_info->gpt_l1_addr,
		  gpi_info->gpi_shift, gpi_info->idx, target_gpi);

	/* Ensure all agents observe new state. */
	tlbi_page_dsbosh(base);
}

static void flush_page_to_popa(uintptr_t addr)
{
	size_t size = GPT_PGS_ACTUAL_SIZE(firme_gpt_config.pgs_bits);

	if (is_feat_mte2_supported()) {
		flush_dcache_to_popa_range_mte2(addr, size);
	} else {
		flush_dcache_to_popa_range(addr, size);
	}
}

static inline void gpt_delegate(uint64_t base, uint8_t target_gpi,
				firme_gpi_info_t *gpi_info)
{
	uint8_t source_gpi = gpi_info->gpi;

	/*
	 * In order to maintain mutual distrust between states, remove any data
	 * speculatively fetched into the target physical address space.
	 */
	flush_page_to_popa(base | GPI_TO_NSE(target_gpi));

	gpt_write_entry(base, target_gpi, gpi_info);

	/* Ensure scrubbed data has made it past PoPA */
	flush_page_to_popa(base | GPI_TO_NSE(source_gpi));
}

static inline void gpt_undelegate(uint64_t base, uint8_t target_gpi,
				  firme_gpi_info_t *gpi_info)
{
	uint8_t source_gpi = gpi_info->gpi;

	/*
	 * In order to maintain mutual distrust between states, remove access
	 * now, in order to guarantee that writes to the currently-accessible
	 * physical address space will not later become observable.
	 */
	write_gpt(&gpi_info->gpt_l1_desc, gpi_info->gpt_l1_addr,
		  gpi_info->gpi_shift, gpi_info->idx, GPT_GPI_NO_ACCESS);

	/* Ensure all agents observe NO ACCESS state. */
	tlbi_page_dsbosh(base);

	/*
	 * Ensure that the scrubbed data have made it past the PoPA for both
	 * old and new security states.
	 */
	flush_page_to_popa(base | GPI_TO_NSE(source_gpi));
	flush_page_to_popa(base | GPI_TO_NSE(target_gpi));

	gpt_write_entry(base, target_gpi, gpi_info);
}

/*
 * Transition set of granules. On success all granules are transitioned, on
 * failure no granules are transitioned.
 *
 * This is naive implementation of range based GPI set.
 */
static int gpi_set_range(uint64_t base, uint64_t gcnt, uint8_t target_gpi,
			 uint8_t src_sec_state, uint64_t *gcnt_ret)
{
	firme_gpi_info_t gpi_info = { 0, NULL, 0, 0, 0 };
	struct firme_gm_mapping mapping = { 0 };
	uint64_t *table;
	uint32_t cnt;
	uint64_t addr;
	int rc;

	rc = firme_gm_active_l1_map(base, &mapping, &table);
	if (rc != 0) {
		*gcnt_ret = 0U;
		return rc;
	}

	/* Check if all granules are in PAS that can be transistioned */
	addr = base;
	for (cnt = 0; cnt < gcnt; cnt++) {
		/* Get GPI info for next granule to transition. */
		read_gpi(addr, table, &gpi_info);

		/* Verify that transition of this granule is allowed. */
		if (!is_gpi_transition_permitted(src_sec_state, gpi_info.gpi,
						 target_gpi)) {
			VERBOSE("(%s) Sec state %u is not allowed to "
				"transition %u to %u!\n", __func__,
				src_sec_state, gpi_info.gpi, target_gpi);
			*gcnt_ret = 0UL;
			rc = -EACCES;
			goto out;
		}

		addr += GPT_GSIZE;
	}

	addr = base;
	for (cnt = 0; cnt < gcnt; cnt++, addr += GPT_GSIZE) {
		/* Get GPI info for next granule to transition. */
		read_gpi(addr, table, &gpi_info);

		if (target_gpi == gpi_info.gpi) {
			continue;
		}

#if (RME_GPT_MAX_BLOCK != 0)
		/* Check for Contiguous descriptor */
		if ((gpi_info.gpt_l1_desc & GPT_L1_TYPE_CONT_DESC_MASK) ==
		    GPT_L1_TYPE_CONT_DESC) {
			shatter_block(addr, &gpi_info,
				      GPI_TO_DESC(gpi_info.gpi));
		}
#endif

		if (((target_gpi == GPT_GPI_NS) &&
		     (gpi_info.gpi == GPT_GPI_NSO)) ||
		    ((target_gpi == GPT_GPI_NSO) &&
		     (gpi_info.gpi == GPT_GPI_NS))) {
			/* Handle NS/NSO transition. */
			gpt_write_entry(addr, target_gpi, &gpi_info);
		} else if ((target_gpi == GPT_GPI_NS) ||
			   (target_gpi == GPT_GPI_NSO)) {
			/* Handle undelegate transition. */
			gpt_undelegate(addr, target_gpi, &gpi_info);
		} else {
			/* Handle delegate transition. */
			gpt_delegate(addr, target_gpi, &gpi_info);
		}

#if (RME_GPT_MAX_BLOCK != 0)
		if (gpi_info.gpt_l1_desc == GPI_TO_DESC(target_gpi)) {
			/* Try to fuse */
			fuse_block(addr, &gpi_info, GPI_TO_DESC(target_gpi));
		}
#endif
	}

	/* currently, on success all granules are transitioned */
	*gcnt_ret = gcnt;
	rc = 0;

out:
	firme_gm_unmap_root(&mapping);
	return rc;
}

/*
 * This function is for firme handler to set GPI encoding to 'target_gpi'
 * based on current_gpi and src_sec_state.
 *
 * The transition of GPI from source to target are based on FIRME rules
 *
 * Parameters
 *   base               Base address of the first granule to transition, aligned
 *                      to granule size.
 *   gcnt               Number of granules to set GPI
 *   target_gpi         GPI to transition the granules to.
 *   src_sec_state      Security state of the requesting entity. This will be
 *                      combined with target_gpi to determine whether a
 *                      transition is allowed.
 */
static int gm_gpi_set(uint64_t base, uint64_t gcnt, uint8_t target_gpi,
		      uint8_t src_sec_state, uint64_t *gcnt_ret)
{
	int res;
	size_t size;
	uint32_t lock_index = INVALID_LOCK_IDX;
	uint64_t par_base;
	uint64_t gbase_in_par;
	uint64_t gcnt_in_par;
	uint64_t gcnt_to_process;
	uint64_t gpt_l0_desc;
	uint64_t *gpt_l0_base;

	/* Ensure that the tables have been set up before taking requests */
	assert((unsigned long)GPT_L0BASE != 0UL);

	/* Ensure that MMU and caches are enabled */
	assert((read_sctlr_el3() & SCTLR_C_BIT) != 0UL);

	if (gcnt == 0UL) {
		return -EINVAL;
	}

	/* Make sure target GPI is valid. */
	if (!firme_is_target_gpi_valid(src_sec_state, target_gpi)) {
		VERBOSE("Caller secuity state: %d has Invalid target GPI: %u\n",
			src_sec_state, target_gpi);
		return -EINVAL;
	}

	/* Calculate total region size and zero out granule count. */
	size = gcnt * GPT_PGS_ACTUAL_SIZE(firme_gpt_config.pgs_bits);

	assert(gcnt_ret != NULL);

	/* Check that base and size are valid. */
	if ((ULONG_MAX - base) < size) {
		VERBOSE("GPT: Transition request address overflow!\n");
		VERBOSE("      Base=0x%" PRIx64 "\n", base);
		VERBOSE("      Size=%lu\n", size);
		return -EINVAL;
	}

	if (!IS_ALIGNED(base, GPT_GSIZE)) {
		return -EINVAL;
	}

	/* Make sure base and size are valid */
	if ((base + size) >= GPT_PPS_ACTUAL_SIZE(firme_gpt_config.pps_bits)) {
		VERBOSE("GPT: Invalid granule transition address range!\n");
		VERBOSE("      Base=0x%" PRIx64 "\n", base);
		VERBOSE("      Size=%lu\n", size);
		return -EINVAL;
	}

	/* Get PAR lock. */
	par_base = round_down(base, PA_RANGE_SIZE);
	res = gm_gpi_pa_range_lock(par_base, &lock_index);
	if (res != 0) {
		VERBOSE("PA lock failed: %d\n", res);
		return res;
	}

	/* Check if PAR is backed by table descriptor. */
	gpt_l0_base = (uint64_t *)GPT_L0BASE;
	gpt_l0_desc = gpt_l0_base[GPT_L0_IDX(par_base)];
	if (GPT_L0_TYPE(gpt_l0_desc) != GPT_L0_TYPE_TBL_DESC) {
		res = -ENOENT;
		goto out_unlock;
	}

	/*
	 * find number of granules that resides in a single PAR and limit the
	 * size of range to transition to 2MB for stateless LRO.
	 */
	gbase_in_par = base & (PA_RANGE_SIZE - 1);
	gcnt_in_par = (PA_RANGE_SIZE - gbase_in_par) / GPT_GSIZE;
	gcnt_to_process = MIN(gcnt, gcnt_in_par);

	if ((gcnt_to_process * GPT_GSIZE) > SZ_2M) {
		gcnt_to_process = SZ_2M / GPT_GSIZE;
	}

	/* Call set GPI range with PAR lock. */
	VERBOSE("gm_gpi_set: gcnt to process: %ld\n", gcnt_to_process);
	res = gpi_set_range(base, gcnt_to_process, target_gpi, src_sec_state,
			    gcnt_ret);

out_unlock:
	gm_gpi_pa_range_unlock(lock_index);

	return res;
}

int firme_gm_gpi_set_reserved(uint64_t base, uint64_t gcnt,
			      uint8_t target_gpi, uint64_t *gcnt_ret)
{
	uint64_t processed = 0U;

	if ((gcnt_ret == NULL) ||
	    ((target_gpi != GPT_GPI_NS) &&
	     (target_gpi != GPT_GPI_ROOT)) ||
	    (firme_gm_validate_granule_range(base, gcnt) != 0)) {
		return -EINVAL;
	}

	*gcnt_ret = 0U;
	while (processed < gcnt) {
		uint64_t addr = base + (processed * GPT_GSIZE);
		uint64_t in_lock_range = PA_RANGE_SIZE -
			(addr & (PA_RANGE_SIZE - 1U));
		uint64_t count = MIN(gcnt - processed,
				     in_lock_range / GPT_GSIZE);
		uint64_t count_done = 0U;
		int rc;

		if ((count * GPT_GSIZE) > SZ_2M) {
			count = SZ_2M / GPT_GSIZE;
		}

		rc = gpi_set_range(addr, count, target_gpi, SEC_STATE_ROOT,
				   &count_done);
		processed += count_done;
		if (rc != 0) {
			*gcnt_ret = processed;
			return rc;
		}
	}

	*gcnt_ret = processed;
	return 0;
}

int firme_gm_gpi_range_has_state(uint64_t base, uint64_t gcnt,
				 uint8_t expected_gpi)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	uint64_t checked = 0U;

	if (!firme_gm_is_gpi_valid(expected_gpi) ||
	    (firme_gm_validate_granule_range(base, gcnt) != 0)) {
		return -EINVAL;
	}

	while (checked < gcnt) {
		struct firme_gm_mapping mapping = { 0 };
		uint64_t addr = base + (checked * GPT_GSIZE);
		uint64_t par_left = geometry->par_size -
			(addr & (geometry->par_size - 1U));
		uint64_t count = MIN(gcnt - checked,
				     par_left / GPT_GSIZE);
		uint64_t *table;
		int rc;

		rc = firme_gm_active_l1_map(addr, &mapping, &table);
		if (rc != 0) {
			return rc;
		}

		for (uint64_t i = 0U; i < count; i++) {
			firme_gpi_info_t info = { 0 };

			read_gpi(addr + (i * GPT_GSIZE), table, &info);
			if (info.gpi != expected_gpi) {
				firme_gm_unmap_root(&mapping);
				return -EACCES;
			}
		}

		firme_gm_unmap_root(&mapping);
		checked += count;
	}

	return 0;
}

/* FIRME ABI handler to set GPI on range. Returns FIRME error codes  */
int firme_gm_gpi_set(uint64_t base, uint64_t gcnt, uint64_t attrs,
		     uint64_t flags, uint64_t *gcnt_ret)
{
	int rc;
	int firme_rc;
	uint8_t target_gpi;
	uint32_t src_sec_state = caller_sec_state(flags);

	/* Extract target GPI value from attributes in x3. */
	target_gpi = EXTRACT(FIRME_GM_GPI_SET_TGT_GPI, attrs);

	/* Make sure target GPI is valid. */
	if (!firme_is_target_gpi_valid(src_sec_state, target_gpi)) {
		VERBOSE("Caller security state: %d has invalid target GPI: %u\n",
			src_sec_state, target_gpi);
		return FIRME_INVALID_PARAMETERS;
	}

	rc = gm_gpi_set(base, gcnt, target_gpi, src_sec_state, gcnt_ret);

	firme_rc = firme_errno_from_generic_errno(rc);
	if ((firme_rc != FIRME_SUCCESS) && (firme_rc != FIRME_DENIED) &&
	    (firme_rc != FIRME_OP_CONFLICT) &&
	    (firme_rc != FIRME_NOT_FOUND)) {
		*gcnt_ret = 0U;
	}

	return firme_rc;
}

void firme_gm_gpi_init(void)
{
	static const uint8_t pps_to_bits[] = {
		32U, 36U, 40U, 42U, 44U, 48U, 52U, 56U
	};
	static const uint8_t pgs_to_bits[] = { 12U, 16U, 14U };

	firme_gpt_config.pps_bits =
		pps_to_bits[EXTRACT(GPCCR_PPS, read_gpccr_el3())];
	firme_gpt_config.pgs_bits =
		pgs_to_bits[EXTRACT(GPCCR_PGS, read_gpccr_el3())];
	firme_gpt_config.l1_cnt_2mb =
		GPT_L1_ENTRY_COUNT_2MB(firme_gpt_config.pgs_bits);

	/* Enable these GPIs in NSO/NSP transition policies used by GPI set */
	if (is_feat_rme_gdi_supported()) {
		firme_gpi_config[GPT_GPI_NS].policy[SMC_FROM_NON_SECURE] |=
			((1 << GPT_GPI_NSP) | (1 << GPT_GPI_SA));
		firme_gpi_config[GPT_GPI_NSO].policy[SMC_FROM_NON_SECURE] |=
			((1 << GPT_GPI_NSP) | (1 << GPT_GPI_SA));
		firme_gpi_config[GPT_GPI_NSP].policy[SMC_FROM_NON_SECURE] |=
			((1 << GPT_GPI_NS) | (1 << GPT_GPI_NSO));
		firme_gpi_config[GPT_GPI_SA].policy[SMC_FROM_NON_SECURE] |=
			((1 << GPT_GPI_NS) | (1 << GPT_GPI_NSO));
	}

	/* Enable NSO in NS transition policies used by FIRME GPI set. */
	if (is_feat_rme_gpc2_supported()) {
		firme_gpi_config[GPT_GPI_NS].policy[SMC_FROM_NON_SECURE] |=
			(1 << GPT_GPI_NSO);
		firme_gpi_config[GPT_GPI_NSO].policy[SMC_FROM_NON_SECURE] |=
			(1 << GPT_GPI_NS);
	}
}
