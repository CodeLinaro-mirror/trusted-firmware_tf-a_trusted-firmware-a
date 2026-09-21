/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/extensions/rme.h>
#include <lib/utils_def.h>
#include <plat/common/common_def.h>
#include <plat/common/firme_gpt_policy.h>
#include <services/firme/firme_granule_mgmt.h>

#include "firme_gm_private.h"

static struct firme_gpt_geometry firme_gpt_geometry;
static const struct firme_gpt_policy *firme_gpt_policy;

#pragma weak plat_firme_get_gpt_policy
const struct firme_gpt_policy *plat_firme_get_gpt_policy(void)
{
	return NULL;
}

static bool firme_gm_par_size_is_supported(uint64_t size)
{
	return (size == SZ_1G) || (size == (SZ_1G << 4)) ||
	       (size == (SZ_1G << 6)) || (size == (SZ_1G << 9));
}

static bool firme_gm_policy_is_valid(const struct firme_gpt_policy *policy)
{
	uint64_t previous_top = 0U;

	if (policy == NULL) {
		return false;
	}

	if ((policy->exceptions == NULL) != (policy->exception_count == 0U)) {
		return false;
	}

	for (size_t i = 0U; i < policy->exception_count; i++) {
		const struct firme_gpt_manager_range *range =
			&policy->exceptions[i];

		if ((range->top <= range->base) ||
		    ((range->base & (firme_gpt_geometry.par_size - 1U)) != 0U) ||
		    ((range->top & (firme_gpt_geometry.par_size - 1U)) != 0U) ||
		    (range->top > firme_gpt_geometry.pps_size) ||
		    ((i != 0U) && (range->base < previous_top))) {
			return false;
		}

		previous_top = range->top;
	}

	return true;
}

bool firme_gm_range_end(uint64_t base, uint64_t size, uint64_t *end)
{
	if ((size == 0U) || (end == NULL) ||
	    __builtin_add_overflow(base, size, end)) {
		return false;
	}

	return *end != 0U;
}

bool firme_gm_ranges_overlap(const struct firme_gm_range *a,
			     const struct firme_gm_range *b)
{
	uint64_t a_end;
	uint64_t b_end;

	if ((a == NULL) || (b == NULL) ||
	    !firme_gm_range_end(a->base, a->size, &a_end) ||
	    !firme_gm_range_end(b->base, b->size, &b_end)) {
		return false;
	}

	return (a->base < b_end) && (b->base < a_end);
}

int firme_gm_geometry_init(void)
{
	static const uint8_t pps_bits[] = { 32U, 36U, 40U, 42U,
					    44U, 48U, 52U, 0U };
	static const uint8_t pgs_bits[] = { 12U, 16U, 14U, 0U };
	uint64_t gpccr = read_gpccr_el3();
	uint64_t gptbr = read_gptbr_el3();
	uint64_t entry_count;
	uint8_t pps;
	uint8_t pgs;
	uint8_t l0gptsz;
	uint8_t l1_index_width;

	pps = (uint8_t)((gpccr >> GPCCR_PPS_SHIFT) & GPCCR_PPS_MASK);
	pgs = (uint8_t)((gpccr >> GPCCR_PGS_SHIFT) & GPCCR_PGS_MASK);
	l0gptsz = (uint8_t)((gpccr >> GPCCR_L0GPTSZ_SHIFT) &
				GPCCR_L0GPTSZ_MASK);

	if ((pps >= ARRAY_SIZE(pps_bits)) || (pgs >= ARRAY_SIZE(pgs_bits)) ||
	    (pps_bits[pps] == 0U) || (pgs_bits[pgs] == 0U) ||
	    ((l0gptsz != GPCCR_L0GPTSZ_30BITS) &&
	     (l0gptsz != GPCCR_L0GPTSZ_34BITS) &&
	     (l0gptsz != GPCCR_L0GPTSZ_36BITS) &&
	     (l0gptsz != GPCCR_L0GPTSZ_39BITS))) {
		return -EINVAL;
	}

	firme_gpt_geometry.pps_encoding = pps;
	firme_gpt_geometry.pgs_encoding = pgs;
	firme_gpt_geometry.l0gptsz_encoding = l0gptsz;
	firme_gpt_geometry.pps_bits = pps_bits[pps];
	firme_gpt_geometry.pgs_bits = pgs_bits[pgs];
	firme_gpt_geometry.par_bits = l0gptsz + 30U;
	firme_gpt_geometry.pps_size =
		ULL(1) << firme_gpt_geometry.pps_bits;
	firme_gpt_geometry.par_size =
		ULL(1) << firme_gpt_geometry.par_bits;
	firme_gpt_geometry.granule_size =
		UL(1) << firme_gpt_geometry.pgs_bits;
	firme_gpt_geometry.l0_base =
		(uintptr_t)((gptbr >> GPTBR_BADDR_SHIFT) & GPTBR_BADDR_MASK)
		<< GPTBR_BADDR_VAL_SHIFT;

	if ((firme_gpt_geometry.par_bits <=
	     (firme_gpt_geometry.pgs_bits + 3U)) ||
	    (firme_gpt_geometry.l0_base == 0U)) {
		return -EINVAL;
	}

	l1_index_width = (firme_gpt_geometry.par_bits - 1U) -
			 (firme_gpt_geometry.pgs_bits + 3U);
	if (l1_index_width >= 63U) {
		return -EINVAL;
	}

	entry_count = ULL(1) << l1_index_width;
	if ((entry_count == 0U) ||
	    (entry_count > (SIZE_MAX / sizeof(uint64_t)))) {
		return -EINVAL;
	}

	firme_gpt_geometry.l1_entry_count = entry_count;
	firme_gpt_geometry.l1_index_mask = entry_count - 1U;
	firme_gpt_geometry.l1_table_size =
		(size_t)entry_count * sizeof(uint64_t);
	firme_gpt_geometry.par_granule_count =
		firme_gpt_geometry.par_size /
		firme_gpt_geometry.granule_size;
	firme_gpt_geometry.l1_granule_count =
		firme_gpt_geometry.l1_table_size /
		firme_gpt_geometry.granule_size;
	firme_gpt_geometry.l1_cnt_2mb =
		(uint16_t)(SZ_2M >> (firme_gpt_geometry.pgs_bits + 4U));

	firme_gpt_policy = plat_firme_get_gpt_policy();
	firme_gpt_geometry.l1_lifecycle_supported =
		firme_gm_par_size_is_supported(firme_gpt_geometry.par_size) &&
		((firme_gpt_geometry.l1_table_size %
		  firme_gpt_geometry.granule_size) == 0U) &&
		(firme_gpt_geometry.l1_granule_count != 0U) &&
		firme_gm_policy_is_valid(firme_gpt_policy);

#if !defined(PLAT_XLAT_TABLES_DYNAMIC) || !PLAT_XLAT_TABLES_DYNAMIC
	firme_gpt_geometry.l1_lifecycle_supported = false;
#endif

	return 0;
}

const struct firme_gpt_geometry *firme_gm_get_geometry(void)
{
	return &firme_gpt_geometry;
}

uint64_t firme_gm_geometry_feature_register(void)
{
	return ((uint64_t)firme_gpt_geometry.pgs_encoding <<
		FIRME_GM_PGS_SHIFT) |
	       ((uint64_t)firme_gpt_geometry.l0gptsz_encoding <<
		FIRME_GM_L0GPTSZ_SHIFT) |
	       ((uint64_t)firme_gpt_geometry.pps_encoding <<
		FIRME_GM_PPS_SHIFT);
}

bool firme_gm_l1_lifecycle_is_supported(void)
{
	return firme_gpt_geometry.l1_lifecycle_supported;
}

bool firme_gm_par_is_host_managed(uint64_t par_base)
{
	bool host_managed;

	if (!firme_gpt_geometry.l1_lifecycle_supported ||
	    (firme_gpt_policy == NULL) ||
	    (firme_gm_validate_par(par_base) != 0)) {
		return false;
	}

	host_managed = firme_gpt_policy->default_host_managed;
	for (size_t i = 0U; i < firme_gpt_policy->exception_count; i++) {
		const struct firme_gpt_manager_range *range =
			&firme_gpt_policy->exceptions[i];

		if (par_base < range->base) {
			break;
		}
		if (par_base < range->top) {
			host_managed = range->host_managed;
			break;
		}
	}

	return host_managed;
}

int firme_gm_validate_granule_range(uint64_t base, uint64_t count)
{
	uint64_t size;
	uint64_t end;

	if ((count == 0U) ||
	    ((base & (firme_gpt_geometry.granule_size - 1U)) != 0U) ||
	    __builtin_mul_overflow(count,
			   (uint64_t)firme_gpt_geometry.granule_size, &size) ||
	    !firme_gm_range_end(base, size, &end) ||
	    (end > firme_gpt_geometry.pps_size)) {
		return -EINVAL;
	}

	return 0;
}

int firme_gm_validate_par(uint64_t par_base)
{
	uint64_t end;

	if ((firme_gpt_geometry.par_size == 0U) ||
	    ((par_base & (firme_gpt_geometry.par_size - 1U)) != 0U) ||
	    !firme_gm_range_end(par_base, firme_gpt_geometry.par_size, &end) ||
	    (end > firme_gpt_geometry.pps_size)) {
		return -EINVAL;
	}

	return 0;
}

int firme_gm_validate_l1_object(uint64_t l1_base)
{
	uint64_t end;

	if (!firme_gpt_geometry.l1_lifecycle_supported ||
	    (l1_base == 0U) ||
	    ((l1_base & (firme_gpt_geometry.l1_table_size - 1U)) != 0U) ||
	    !firme_gm_range_end(l1_base, firme_gpt_geometry.l1_table_size,
				&end) ||
	    (end > firme_gpt_geometry.pps_size)) {
		return -EINVAL;
	}

	return 0;
}

int firme_gm_transition_reservation_range(uint64_t base, uint64_t size,
					  struct firme_gm_range *range)
{
#if (RME_GPT_MAX_BLOCK == 512)
	const uint64_t lock_size = SZ_512M;
#elif (RME_GPT_MAX_BLOCK == 32)
	const uint64_t lock_size = SZ_32M;
#elif (RME_GPT_MAX_BLOCK == 2)
	const uint64_t lock_size = SZ_2M;
#else
	const uint64_t lock_size = SZ_4M;
#endif
	uint64_t end;
	uint64_t lock_end;

	if ((range == NULL) || !firme_gm_range_end(base, size, &end) ||
	    __builtin_add_overflow(end, lock_size - 1U, &lock_end)) {
		return -EINVAL;
	}

	range->base = base & ~(lock_size - 1U);
	lock_end &= ~(lock_size - 1U);
	range->size = lock_end - range->base;

	return (range->size == 0U) ? -EINVAL : 0;
}
