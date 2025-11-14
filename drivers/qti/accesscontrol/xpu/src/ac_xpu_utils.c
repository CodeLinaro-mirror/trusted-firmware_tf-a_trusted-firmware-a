/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * Access control support functions
 */

#include <stdbool.h>
#include <stdint.h>

#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#ifdef XPU_VERSION_4
#include "hal_xpu4.h"
#include "hal_xpu4_util.h"
#endif

#define AC_SIZE_4KB			0x1000

ac_global_data_t *p_ac_global_data;

static bool ac_is_addr_4k_aligned(uint64_t addr)
{
	return !(addr & (AC_SIZE_4KB - 1));
}

/*
 * Gets the Pointers to XpuProtectedRanges Info
 *
 * @param pac_xpu_protected_ranges	Pointer to store protected ranges array
 * @param pac_xpu_protected_ranges_count	Pointer to store count
 */
void ac_get_dynamic_protect_ranges(const ac_xpu_protected_range **pac_xpu_protected_ranges,
				    uint32_t *pac_xpu_protected_ranges_count)
{
	error_if_ac_config_fail();

	*pac_xpu_protected_ranges = (const ac_xpu_protected_range *)
		p_ac_global_data->p_global_data_ptr[AC_XPUPROTECTEDRANGES_ARRAY];
	if (p_ac_global_data->p_global_data_ptr[AC_XPUPROTECTEDRANGES_ARRAY_SIZE_PTR]) {
		*pac_xpu_protected_ranges_count = *(uint32_t *)
			p_ac_global_data->p_global_data_ptr[AC_XPUPROTECTEDRANGES_ARRAY_SIZE_PTR];
	} else {
		*pac_xpu_protected_ranges_count = 0;
	}
}

/*
 * Gets the Pointers to RAXpu Info
 *
 * @param pac_ra_xpu_info	Pointer to store RA XPU info array
 * @param pac_ra_xpu_info_count	Pointer to store count
 */
void ac_get_ra_xpu_info(const ac_ra_xpu_info **pac_ra_xpu_info,
			uint32_t *pac_ra_xpu_info_count)
{
	error_if_ac_config_fail();

	*pac_ra_xpu_info = (const ac_ra_xpu_info *)
		p_ac_global_data->p_global_data_ptr[AC_RAXPU_INFO_ARRAY];
	if (p_ac_global_data->p_global_data_ptr[AC_RAXPU_INFO_ARRAY_SIZE_PTR]) {
		*pac_ra_xpu_info_count = *(uint32_t *)
			p_ac_global_data->p_global_data_ptr[AC_RAXPU_INFO_ARRAY_SIZE_PTR];
	} else {
		*pac_ra_xpu_info_count = 0;
	}
}

/*
 * Gets the Pointers to XPU Info
 *
 * @param pac_xpu_infos	Pointer to store XPU info array
 * @param pac_xpu_infos_count	Pointer to store count
 */
void ac_xpu_get_xpu_infos(ac_xpu4_priv_info **pac_xpu_infos,
			  uint32_t *pac_xpu_infos_count)
{
	error_if_ac_config_fail();
	if (pac_xpu_infos != NULL && pac_xpu_infos_count != NULL) {
		*pac_xpu_infos = (ac_xpu4_priv_info *)
			p_ac_global_data->p_global_data_ptr[AC_XPUINFOS_ARRAY];
		if (p_ac_global_data->p_global_data_ptr[AC_XPUINFOS_ARRAY_SIZE_PTR]) {
			*pac_xpu_infos_count = *(uint32_t *)
				p_ac_global_data->p_global_data_ptr[AC_XPUINFOS_ARRAY_SIZE_PTR];
		} else {
			*pac_xpu_infos_count = 0;
		}
	}
}

/*
 * Gets the Pointers to XPU HW ADDR Offset/Mask Info
 *
 * @param pac_xpu_hw_addr_mask_offset	Pointer to store HW addr mask array
 * @param pac_xpu_hw_addr_mask_offset_count	Pointer to store count
 */
void ac_xpu_get_addr_offset_mask_info(const tzbsp_xpu_hwaddr_mask **pac_xpu_hw_addr_mask_offset,
				       uint32_t *pac_xpu_hw_addr_mask_offset_count)
{
	error_if_ac_config_fail();
	if (pac_xpu_hw_addr_mask_offset != NULL &&
	    pac_xpu_hw_addr_mask_offset_count != NULL) {
		*pac_xpu_hw_addr_mask_offset = (const tzbsp_xpu_hwaddr_mask *)
		p_ac_global_data->p_global_data_ptr[AC_XPUHWADDR_MASK_ARRAY];
		if (p_ac_global_data->p_global_data_ptr[AC_XPUHWADDR_MASK_ARRAY_SIZE_PTR]) {
			*pac_xpu_hw_addr_mask_offset_count = *(uint32_t *)
			p_ac_global_data->p_global_data_ptr[AC_XPUHWADDR_MASK_ARRAY_SIZE_PTR];
		} else {
			*pac_xpu_hw_addr_mask_offset_count = 0;
		}
	}
}

/*
 * Gets the Pointers to XPU Config Info
 *
 * @param pac_xp_cfg	Pointer to store XPU config array
 * @param pac_xp_cfg_count	Pointer to store count
 */
void ac_xpu_get_xpu_cfg(const ac_xpu_cfg **pac_xp_cfg,
			uint32_t *pac_xp_cfg_count)
{
	error_if_ac_config_fail();
	if (pac_xp_cfg != NULL && pac_xp_cfg_count != NULL) {
		*pac_xp_cfg = (const ac_xpu_cfg *)
			p_ac_global_data->p_global_data_ptr[AC_XPUCFG_ARRAY];
		if (p_ac_global_data->p_global_data_ptr[AC_XPUCFG_ARRAY_SIZE_PTR]) {
			*pac_xp_cfg_count = *(uint32_t *)
				p_ac_global_data->p_global_data_ptr[AC_XPUCFG_ARRAY_SIZE_PTR];
		} else {
			*pac_xp_cfg_count = 0;
		}
	}
}

/*
 * Determine if the given xPU is disabled
 *
 * @param xpu_id	XPU identifier
 *
 * @return true if XPU is disabled, false otherwise
 */
bool ac_is_xpu_disabled(HAL_xpu2_XPU2Type xpu_id)
{
	size_t i;
	const ac_xpu_cfg *pac_xp_cfg = NULL;
	uint32_t uac_xpu_cfg_count = 0;

	ac_xpu_get_xpu_cfg(&pac_xp_cfg, &uac_xpu_cfg_count);

	for (i = 0; i < uac_xpu_cfg_count; i++) {
		if (xpu_id == pac_xp_cfg[i].xpu_id) {
			return ((pac_xp_cfg[i].status != XPU_ENABLED) ||
				(ac_is_xpu_profile_matched(AC_MILESTONE_COLD_BOOT,
							   pac_xp_cfg[i].profile_flags) != true));
		}
	}

	return false;
}

/*
 * Validate address region alignment and overflow
 *
 * @param start	Start address
 * @param size	Region size
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_validate_address_region(uint64_t start, uint64_t size)
{
	if (!ac_is_addr_4k_aligned(start)) {
		ERROR("AC_XPU ErrCode: %d, Address not 4KB aligned: 0x%lx",
		      AC_ERR_REGION_ADDR_NOT_ALIGNED, start);
		return AC_ERR_REGION_ADDR_NOT_ALIGNED;
	}

	if (!ac_is_addr_4k_aligned(size)) {
		ERROR("AC_XPU ErrCode: %d, Size not 4KB aligned: 0x%lx",
		      AC_ERR_REGION_SIZE_NOT_ALIGNED, size);
		return AC_ERR_REGION_SIZE_NOT_ALIGNED;
	}

	if (size == 0) {
		ERROR("AC_XPU ErrCode: %d, Region size is zero", AC_ERR_REGION_SIZE_ZERO);
		return AC_ERR_REGION_SIZE_ZERO;
	}

	if (start > UINT64_MAX - size) {
		ERROR("AC_XPU ErrCode: %d, Region overflow - start: 0x%x%08x, size: 0x%x%08x",
		      AC_ERR_REGION_OVERFLOW, (uint32_t)(start >> 32), (uint32_t)start,
		      (uint32_t)(size >> 32), (uint32_t)size);

		return AC_ERR_REGION_OVERFLOW;
	}

	return AC_SUCCESS;
}

/*
 * Validate secure domain ID details
 *
 * @param sd_array	Security domain array
 * @param count		Number of domains
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_validate_domain_array(ac_sec_domain_id *sd_array, uint32_t count)
{
	uint32_t i;

	if (sd_array == NULL) {
		ERROR("AC_XPU ErrCode: %d, Domain array is NULL", AC_ERR_NULL_POINTER);
		return AC_ERR_NULL_POINTER;
	}

	if (count == 0) {
		ERROR("AC_XPU ErrCode: %d, Domain count is zero", AC_ERR_DOMAIN_COUNT_ZERO);
		return AC_ERR_DOMAIN_COUNT_ZERO;
	}

	if (count > AC_SD_END_MARKER) {
		ERROR("AC_XPU ErrCode: %d, Domain count exceeds maximum: %u",
		      AC_ERR_INPUT_VALIDATION, count);
		return AC_ERR_INPUT_VALIDATION;
	}

	if ((sd_array + count) < sd_array) {
		ERROR("AC_XPU ErrCode: %d, Pointer overflow - array: %p, count: %u",
		      AC_ERR_POINTER_OVERFLOW, sd_array, count);
		return AC_ERR_POINTER_OVERFLOW;
	}

	for (i = 0; i < count; i++) {
		if (sd_array[i] >= AC_SD_END_MARKER || sd_array[i] < 0) {
			ERROR("AC_XPU ErrCode: %d, Invalid domain ID: %d",
			      AC_ERR_DOMAIN_INVALID, sd_array[i]);
			return AC_ERR_DOMAIN_INVALID;
		}
	}

	return AC_SUCCESS;
}

/*
 * Validate secure domain ID valid for particular
 * execution environment (locking operation)
 *
 * @param sd_id	Security domain ID
 *
 * @return true if valid, false otherwise
 */
bool ac_xpu4_validate_sec_domain_id_for_ee(ac_sec_domain_id sd_id)
{
	bool bret = false;

	if ((sd_id == AC_SD_AP_SEC) || (sd_id == AC_SD_AP_NSEC))
		bret = true;

	return bret;
}

/*
 * Validate secure domain ID, permission and lock details
 *
 * @param sd_perm_array	Security domain permission array
 * @param count		Number of permissions
 * @param check_domain_only	Whether to check domain only
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_validate_perm_array(ac_sec_domain_perm *sd_perm_array,
				 uint32_t count,
				 bool check_domain_only)
{
	uint32_t i;

	if (sd_perm_array == NULL) {
		ERROR("AC_XPU ErrCode: %d, Permission array is NULL", AC_ERR_NULL_POINTER);
		return AC_ERR_NULL_POINTER;
	}

	if (count == 0) {
		ERROR("AC_XPU ErrCode: %d, Permission count is zero",
		      AC_ERR_DOMAIN_COUNT_ZERO);
		return AC_ERR_DOMAIN_COUNT_ZERO;
	}

	if (count > AC_SD_END_MARKER) {
		ERROR("AC_XPU ErrCode: %d, Permission count exceeds maximum: %u",
		      AC_ERR_INPUT_VALIDATION, count);
		return AC_ERR_INPUT_VALIDATION;
	}

	if (sd_perm_array + count < sd_perm_array) {
		ERROR("AC_XPU ErrCode: %d, Pointer overflow - array: %p, count: %u",
		      AC_ERR_POINTER_OVERFLOW, sd_perm_array, count);
		return AC_ERR_POINTER_OVERFLOW;
	}

	for (i = 0; i < count; i++) {
		ac_sec_domain_perm *sd_perm = &sd_perm_array[i];

		if (sd_perm->sd_id >= AC_SD_END_MARKER || sd_perm->sd_id < 0) {
			ERROR("AC_XPU ErrCode: %d, Invalid domain ID: %d",
			      AC_ERR_DOMAIN_INVALID, sd_perm->sd_id);
			return AC_ERR_DOMAIN_INVALID;
		}

		if (check_domain_only)
			continue;

		if (sd_perm->perm >= AC_RW_END_MARKER || sd_perm->perm < 0) {
			ERROR("AC_XPU ErrCode: %d, Invalid permission: %d",
			      AC_ERR_PERM_INVALID, sd_perm->perm);
			return AC_ERR_PERM_INVALID;
		}

		if (sd_perm->lock >= AC_SD_MAX_OP || sd_perm->lock < 0) {
			ERROR("AC_XPU ErrCode: %d, Invalid lock: %d",
			      AC_ERR_DOMAIN_LOCK_INVALID, sd_perm->lock);
			return AC_ERR_DOMAIN_LOCK_INVALID;
		}

		if ((sd_perm->lock == AC_SD_LOCK) &&
		    ((sd_perm->perm == AC_RW_NONE) ||
		     (ac_xpu4_validate_sec_domain_id_for_ee(sd_perm->sd_id) != true))) {
			ERROR("AC_XPU ErrCode: %d, InvalidDomainLock - sd_id:%d, perm:%d, lock:%d",
				AC_ERR_DOMAIN_LOCK_INVALID, sd_perm->sd_id, sd_perm->perm,
				sd_perm->lock);
			return AC_ERR_DOMAIN_LOCK_INVALID;
		}
	}

	return AC_SUCCESS;
}

/*
 * Get xPU-ID details from start, size passed
 *
 * @param start	Start address
 * @param size	Region size
 * @param xpu_id	Pointer to store XPU ID
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_get_xpu_id_from_range(uint64_t start, uint64_t size,
				   HAL_xpu2_XPU2Type *xpu_id)
{
	uint32_t i;
	const ac_xpu_protected_range *pac_xpu_protected_ranges = NULL;
	uint32_t uac_xpu_protected_ranges_count = 0;

	if (xpu_id == NULL)
		return AC_ERR_NULL_POINTER;

	ac_get_dynamic_protect_ranges(&pac_xpu_protected_ranges,
				      &uac_xpu_protected_ranges_count);

	if (pac_xpu_protected_ranges == NULL || uac_xpu_protected_ranges_count == 0) {
		ERROR("AC_XPU ErrCode: %d, No protected ranges available",
		      AC_ERR_REGION_XPU_NOT_FOUND);
		return AC_ERR_REGION_XPU_NOT_FOUND;
	}

	for (i = 0; i < uac_xpu_protected_ranges_count; i++) {
		ac_xpu_protected_range *range =
			(ac_xpu_protected_range *)&pac_xpu_protected_ranges[i];
		if (range->start <= start && range->end >= start + size) {
			*xpu_id = range->xpu_id;
			return AC_SUCCESS;
		}
	}

	ERROR("AC_XPU ErrCode: %d, XPU not found for region - start: 0x%x%08x, size: 0x%x%08x\n",
	      AC_ERR_REGION_XPU_NOT_FOUND, (uint32_t)(start >> 32), (uint32_t)start,
	      (uint32_t)(size >> 32), (uint32_t)size);

	return AC_ERR_REGION_XPU_NOT_FOUND;
}

/*
 * Validate address region alignment and underflow
 *
 * @param start	Start address
 * @param end	End address
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_validate_underflow_address_region(uint64_t start, uint64_t end)
{
	if (!ac_is_addr_4k_aligned(start)) {
		ERROR("AC_XPU ErrCode: %d, Start address not 4KB aligned: 0x%lx",
		      AC_ERR_REGION_ADDR_NOT_ALIGNED, start);
		return AC_ERR_REGION_ADDR_NOT_ALIGNED;
	}

	if (!ac_is_addr_4k_aligned(end)) {
		ERROR("AC_XPU ErrCode: %d, End address not 4KB aligned: 0x%lx",
		      AC_ERR_REGION_ADDR_NOT_ALIGNED, end);
		return AC_ERR_REGION_ADDR_NOT_ALIGNED;
	}

	if (end < start && (end < INT64_MIN + start)) {
		ERROR("AC_XPU ErrCode: %d, Region underflow - start: 0x%x%08x, end: 0x%x%08x",
		      AC_ERR_REGION_UNDERFLOW, (uint32_t)(start >> 32), (uint32_t)start,
		      (uint32_t)(end >> 32), (uint32_t)end);
		return AC_ERR_REGION_UNDERFLOW;
	}
	return AC_SUCCESS;
}
