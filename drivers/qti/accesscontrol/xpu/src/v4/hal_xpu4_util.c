/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * HAL XPU4 utility functions
 */

#include <stdbool.h>
#include <stdint.h>

#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#include "hal_xpu4.h"
#include "hal_xpu4_util.h"

/*
 * Based on xPU ID fetch the address to obtain offset and mask to be applied
 * offset param holds the address which has to be subtracted to get the address
 * to be programmed in HW. Similarly mask holds the bitmask to be applied for
 * getting the address to be programmed
 * Internal API caller makes sure !NULL passed to args
 *
 * @param xpu_id	XPU identifier
 * @param offset	Pointer to store offset value
 * @param mask		Pointer to store mask value
 *
 * @return true if found, false otherwise
 */
static bool ac_xpu_get_addr_offset_mask(HAL_xpu2_XPU2Type xpu_id,
					 uint64_t *offset,
					 uint64_t *mask)
{
	uint32_t i;
	const tzbsp_xpu_hwaddr_mask *pac_xpu_hw_addr_mask_offset = NULL;
	uint32_t uac_xpu_hw_addr_mask_offset_count = 0;

	ac_xpu_get_addr_offset_mask_info(&pac_xpu_hw_addr_mask_offset,
					 &uac_xpu_hw_addr_mask_offset_count);
	for (i = 0; i < uac_xpu_hw_addr_mask_offset_count; i++) {
		if (xpu_id == pac_xpu_hw_addr_mask_offset[i].xpu_index) {
			*offset = pac_xpu_hw_addr_mask_offset[i].base_addr;
			*mask = pac_xpu_hw_addr_mask_offset[i].mask_value;
			return true;
		}
	}

	return false;
}

/*
 * Compute the region relationship between two regions
 *
 * @param start1	Start address of first region
 * @param end1		End address of first region
 * @param start2	Start address of second region
 * @param end2		End address of second region
 *
 * @return Region overlap type
 */
static region_overlap_t region_overlap_compute(uint64_t start1,
						uint64_t end1,
						uint64_t start2,
						uint64_t end2)
{
	uint64_t min_end = MIN(end1, end2);
	uint64_t max_start = MAX(start1, start2);

	if (start1 == start2 && end1 == end2)
		return REGION_EQUAL;

	if (min_end == max_start) {
		/*
		 * This condition means that the regions are adjacent or equal,
		 * but since we have established by this point that the regions
		 * are not equal, they must be adjacent only
		 */
		return REGION_ADJACENT;
	} else if (min_end < max_start) {
		return REGION_NO_OVERLAP;
	} else if (min_end > max_start) {
		if (start2 >= start1 && end2 <= end1) {
			if (start2 > start1 && end2 < end1)
				return REGION_OVERLAP_OUTER;
			else
				return REGION_OVERLAP_OUTER_ADJACENT;
		} else if (start1 >= start2 && end1 <= end2) {
			if (start1 > start2 && end1 < end2)
				return REGION_OVERLAP_INNER;
			else
				return REGION_OVERLAP_INNER_ADJACENT;
		} else {
			return REGION_OVERLAP;
		}
	}
	return REGION_NO_OVERLAP;
}

/*
 * Compute the region relationship between two regions
 *
 * @param start1	Start address of first region
 * @param end1		End address of first region
 * @param start2	Start address of second region
 * @param end2		End address of second region
 * @param overlap	Pointer to store overlap result
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR region_overlap(uint64_t start1, uint64_t end1,
			uint64_t start2, uint64_t end2,
			region_overlap_t *overlap)
{
	if ((start1 == end1) || (start2 == end2) ||
	    (start1 > end1) || (start2 > end2) ||
	    (overlap == NULL))
		return AC_ERR_INPUT_VALIDATION;

	*overlap = region_overlap_compute(start1, end1, start2, end2);

	return AC_SUCCESS;
}

/*
 * Determine if the given xPU is RA type
 *
 * @param xpu_id	XPU identifier
 *
 * @return true if RA type, false otherwise
 */
bool ac_is_ra_xpu(HAL_xpu2_XPU2Type xpu_id)
{
	uint32_t i;
	const ac_ra_xpu_info *pac_ra_xpu_info = NULL;
	uint32_t uac_ra_xpu_info_count = 0;

	ac_get_ra_xpu_info(&pac_ra_xpu_info, &uac_ra_xpu_info_count);
	for (i = 0; i < uac_ra_xpu_info_count; i++) {
		if (xpu_id == pac_ra_xpu_info[i].xpu_id)
			return true;
	}

	return false;
}

/*
 * Fetch the QAD vector for execution environment in which SW is running
 *
 * @return QAD vector for current execution environment
 */
ac_xpu4_qad_vector ac_xpu4_get_env_qad_vector(void)
{
	static uint32_t s_ac_ee_qad_vector;

	if (!s_ac_ee_qad_vector) {
		ac_sec_domain_id domain_id[2] = {AC_SD_AP_SEC, AC_SD_AP_NSEC};

		s_ac_ee_qad_vector = ac_xpu_encode_qad_vector(domain_id, 2);
	}

	return s_ac_ee_qad_vector;
}

/*
 * Encode the AC secure domain ID to HW QAD bitmask
 *
 * @param domain_ids	Array of domain IDs
 * @param count		Number of domain IDs
 *
 * @return Encoded QAD vector
 */
ac_xpu4_qad_vector ac_xpu_encode_qad_vector(ac_sec_domain_id *domain_ids,
					     uint32_t count)
{
	ac_xpu4_qad_vector output = 0;
	uint32_t i;

	if (domain_ids != NULL) {
		for (i = 0; i < count; i++) {
			switch (domain_ids[i]) {
			case AC_SD_AP_SEC:
				output |= APP_SEC_QAD_BIT;
				break;

			case AC_SD_AP_NSEC:
				output |= APP_NSEC_QAD_BIT;
				break;

			default:
				if (domain_ids[i] < (sizeof(ac_xpu4_qad_vector) * 8))
					output |= (ac_xpu4_qad_vector)(0x1UL << domain_ids[i]);
			}
		}
	}

	return output;
}

/*
 * Parse the permission array and compute HW QAD vector for Read, Write and Lock
 * NOTE: Struct elements in "perms" arg must be validated before calling this function
 *
 * @param perms			Permission array
 * @param count			Number of permissions
 * @param read_perm_vector	Pointer to store read permission vector
 * @param write_perm_vector	Pointer to store write permission vector
 * @param lock_vector		Pointer to store lock vector
 */
void ac_xpu_encode_qad_vector3(ac_sec_domain_perm *perms,
			       uint32_t count,
			       ac_xpu4_qad_vector *read_perm_vector,
			       ac_xpu4_qad_vector *write_perm_vector,
			       ac_xpu4_qad_vector *lock_vector)
{
	ac_xpu4_qad_vector read_output = 0;
	ac_xpu4_qad_vector write_output = 0;
	ac_xpu4_qad_vector lock_output = 0;
	uint32_t i;

	if (!read_perm_vector && !write_perm_vector && !lock_vector) {
		/* Nothing to do */
		return;
	}

	if (perms != NULL) {
		for (i = 0; i < count; i++) {
			ac_sec_domain_perm *per_domain = &perms[i];
			ac_xpu4_qad_vector temp = 0;

			switch (per_domain->sd_id) {
			case AC_SD_AP_SEC:
				temp = APP_SEC_QAD_BIT;
				break;

			case AC_SD_AP_NSEC:
				temp = APP_NSEC_QAD_BIT;
				break;

			default:
				if (per_domain->sd_id < (sizeof(ac_xpu4_qad_vector) * 8))
					temp = (ac_xpu4_qad_vector)(0x1UL << per_domain->sd_id);
			}

			switch (per_domain->perm) {
			case AC_READ_ONLY:
				read_output |= temp;
				break;

			case AC_WRITE_ONLY:
				write_output |= temp;
				break;

			case AC_READ_WRITE:
				read_output |= temp;
				write_output |= temp;
				break;

			default:
				/* do nothing */
				break;
			}

			if (per_domain->lock)
				lock_output |= temp;
		}
	}

	if (read_perm_vector)
		*read_perm_vector = read_output;

	if (write_perm_vector)
		*write_perm_vector = write_output;

	if (lock_vector)
		*lock_vector = lock_output;
}

/*
 * Based on Execution Environment it is determined whether xPU policy
 * is expected to be programmed by previous environment
 *
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 *
 * @return true if configured by earlier ROT, false otherwise
 */
bool ac_xpu_rg_configured_by_earlier_rot(ac_xpu4_qad_vector read_perm_vector,
					  ac_xpu4_qad_vector write_perm_vector)
{
	ac_xpu4_qad_vector curr_ee_qad_vec = ac_xpu4_get_env_qad_vector();
	ac_xpu4_qad_vector higher_ee_qad_vec = 0;

	if ((curr_ee_qad_vec == (APP_SEC_QAD_BIT)) ||
	    (curr_ee_qad_vec == (APP_NSEC_QAD_BIT)) ||
	    (curr_ee_qad_vec == (APP_SEC_QAD_BIT | APP_NSEC_QAD_BIT)))
		higher_ee_qad_vec = TME_FW_QAD_BIT | TME_ROM_QAD_BIT;

	if ((read_perm_vector & higher_ee_qad_vec) ||
	    (write_perm_vector & higher_ee_qad_vec))
		return true;
	else
		return false;
}

/*
 * Fetch the xPU permission programmed earlier in xPU RGs
 * Locking/unlocking by particular QAD affects it, so R/W QADs are
 * cached in SW
 * AC internal API caller makes sure there !NULL passed
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param read_perm_vector	Pointer to store read permission vector
 * @param write_perm_vector	Pointer to store write permission vector
 */
void ac_xpu_get_shadow_perm(ac_xpu4_priv_info *xpu_info,
			     uint32_t rg_num,
			     ac_xpu4_qad_vector *read_perm_vector,
			     ac_xpu4_qad_vector *write_perm_vector)
{
	ac_xpu4_qad_vector shadow_read_vec = 0;
	ac_xpu4_qad_vector shadow_write_vec = 0;

	if ((xpu_info != NULL) && (xpu_info->dyn_rgs != NULL)) {
		if ((rg_num >= xpu_info->dyn_rgs->rg_start) &&
		    (rg_num < (xpu_info->dyn_rgs->rg_start + xpu_info->dyn_rgs->rg_count))) {
			rg_num -= xpu_info->dyn_rgs->rg_start;

			if (xpu_info->dyn_rgs->shadow_read_perm)
				shadow_read_vec = xpu_info->dyn_rgs->shadow_read_perm[rg_num];

			if (xpu_info->dyn_rgs->shadow_write_perm)
				shadow_write_vec = xpu_info->dyn_rgs->shadow_write_perm[rg_num];
		}
	}
	if (read_perm_vector != NULL)
		*read_perm_vector = shadow_read_vec;
	if (write_perm_vector != NULL)
		*write_perm_vector = shadow_write_vec;
}

/*
 * Cache the xPU permission programmed in xPU RG in SW(prior to HW update)
 * Locking/unlocking by particular QAD affects it, so R/W QADs are
 * cached in SW
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 */
void ac_xpu_cache_shadow_perm(ac_xpu4_priv_info *xpu_info,
			       uint32_t rg_num,
			       ac_xpu4_qad_vector read_perm_vector,
			       ac_xpu4_qad_vector write_perm_vector)
{
	if ((xpu_info == NULL) || (xpu_info->dyn_rgs == NULL))
		return;

	if ((rg_num < xpu_info->dyn_rgs->rg_start) ||
	    (rg_num >= (xpu_info->dyn_rgs->rg_start + xpu_info->dyn_rgs->rg_count)))
		return;

	/* Track permissions only for Dynamic RGs */
	rg_num -= xpu_info->dyn_rgs->rg_start;

	if (xpu_info->dyn_rgs->shadow_read_perm) {
		uint32_t *shadow = &xpu_info->dyn_rgs->shadow_read_perm[rg_num];

		*shadow = read_perm_vector;
	}

	if (xpu_info->dyn_rgs->shadow_write_perm) {
		uint32_t *shadow = &xpu_info->dyn_rgs->shadow_write_perm[rg_num];

		*shadow = write_perm_vector;
	}
}

/*
 * Get XPU info structure by XPU ID
 *
 * @param xpu_id	XPU identifier
 *
 * @return Pointer to XPU info structure, NULL if not found
 */
ac_xpu4_priv_info *ac_xpu_get_xpu_info(HAL_xpu2_XPU2Type xpu_id)
{
	uint32_t i;
	ac_xpu4_priv_info *pac_xpu_infos = NULL;
	uint32_t uac_xpu_infos_count = 0;

	ac_xpu_get_xpu_infos(&pac_xpu_infos, &uac_xpu_infos_count);

	for (i = 0; i < uac_xpu_infos_count; i++) {
		if (xpu_id == pac_xpu_infos[i].xpu_id)
			return &pac_xpu_infos[i];
	}

	return NULL;
}

/*
 * Convert the SOC address into xPU related address
 *
 * @param xpu_info	XPU info structure
 * @param addr		SOC address
 *
 * @return Converted peripheral XPU address
 */
uint64_t ac_xpu_addr_soc2_peripheral_xpu(ac_xpu4_priv_info *xpu_info,
					  uint64_t addr)
{
	uint64_t offset;
	uint64_t mask;
	uint32_t extra_high_bits;
	uint32_t extra_low_bits;

	if (xpu_info != NULL) {
		/* Bus level offset and masking */
		if (ac_xpu_get_addr_offset_mask(xpu_info->xpu_id, &offset, &mask)) {
			if (addr < offset) {
				ERROR("AC_XPU ErrCode: %d, Addr:0x%lx offset:0x%lx",
				      AC_ERR_INPUT_VALIDATION, addr, offset);
			}
			addr -= offset;
			addr &= mask;
		}

		/* XPU level masking */
		extra_high_bits = 63 - xpu4_decode_idr_addr_msb(xpu_info);
		extra_low_bits = xpu4_decode_idr_addr_lsb(xpu_info);
		addr = addr << extra_high_bits >> (extra_high_bits + extra_low_bits)
		       << extra_low_bits;
	}
	return addr;
}

/*
 * Read CLERE register value from XPU
 *
 * @param xpu_info	XPU info structure
 *
 * @return CLERE register value
 */
ac_xpu4_qad_vector ac_xpu_read_clere(ac_xpu4_priv_info *xpu_info)
{
	if (xpu_info == NULL)
		return 0;

	return XPU4_IN(xpu_info->addr, CLERE);
}

/*
 * Write CLERE register value to XPU
 *
 * @param xpu_info	XPU info structure
 * @param value		Value to write to CLERE register
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_write_clere(ac_xpu4_priv_info *xpu_info,
			     ac_xpu4_qad_vector value)
{
	if (xpu_info == NULL)
		return AC_ERR_NULL_POINTER;

	XPU4_OUT(xpu_info->addr, CLERE, value);
	return AC_SUCCESS;
}
