/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * Access control XPU implementation
 */

#include <stdbool.h>
#include <stdint.h>

#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#include "ac_xpu_target_info.h"
#ifdef XPU_VERSION_4
#include "hal_hwio.h"
#include "hal_xpu4.h"
#include "hal_xpu4_util.h"
#endif

extern uint32_t g_hal_xpu_count;

static AC_ERROR ac_init_xpu_and_get_info(HAL_xpu2_XPU2Type xpu_id,
					  ac_xpu4_priv_info **xpu_info)
{
	ac_xpu4_priv_info *info = NULL;
	AC_ERROR rc = AC_FAILURE;

	if ((xpu_info == NULL) || (xpu_id >= g_hal_xpu_count) || (xpu_id < 0))
		return AC_ERR_INPUT_VALIDATION;

	info = ac_xpu_get_xpu_info(xpu_id);
	if (!info) {
		ERROR("AC_XPU ErrCode: %d, No XPU info for xpu_id: %u",
		      AC_ERR_NO_XPU_INFO, xpu_id);
		return AC_ERR_NO_XPU_INFO;
	}

	if (!info->rev) {
		rc = ac_xpu_init_xpu(info);
		if (rc != AC_SUCCESS)
			return rc;
	}

	*xpu_info = info;
	return AC_SUCCESS;
}

static AC_ERROR ac_get_xpu_info_from_range(uint64_t start, uint64_t size,
					    ac_xpu4_priv_info **xpu_info)
{
	HAL_xpu2_XPU2Type xpu_id;
	ac_xpu4_priv_info *info = NULL;
	AC_ERROR rc = AC_FAILURE;

	if (xpu_info == NULL)
		return AC_ERR_INPUT_VALIDATION;

	rc = ac_get_xpu_id_from_range(start, size, &xpu_id);
	if (rc != AC_SUCCESS)
		return rc;

	if (ac_is_xpu_disabled(xpu_id))
		return AC_ERR_XPU_DISABLED;

	if (g_hal_xpu_count == xpu_id)
		return AC_ERR_REGION_XPU_BOGUS;

	rc = ac_init_xpu_and_get_info(xpu_id, &info);
	if (rc != AC_SUCCESS)
		return rc;

	*xpu_info = info;
	return AC_SUCCESS;
}

/*
 * ac_configure_xpu_apnspe - Configure XPU APNSPE
 *
 * @param milestone	Initialization milestone
 * @param xpu_cfg	XPU configuration array
 * @param count		Number of XPU configurations
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_configure_xpu_apnspe(ac_milestone milestone, ac_xpu_cfg *xpu_cfg,
				  uint32_t count)
{
	AC_ERROR rc = AC_SUCCESS;
	const ac_xpu_protected_range *pac_xpu_protected_ranges = NULL;
	uint32_t uac_xpu_protected_ranges_count = 0;
	uint32_t i;

	if ((xpu_cfg != NULL) && (count < g_hal_xpu_count)) {
		for (i = 0; i < count; i++) {
			ac_xpu4_priv_info *xpu_info;

			if ((xpu_cfg[i].status != XPU_ENABLED) ||
			    (ac_is_xpu_profile_matched(milestone,
						       xpu_cfg[i].profile_flags) != true))
				continue;

			rc = ac_init_xpu_and_get_info(xpu_cfg[i].xpu_id, &xpu_info);
			if (rc != AC_SUCCESS)
				break;

			if (ac_is_xpu_disabled(xpu_info->xpu_id))
				continue;

			rc = ac_xpu_configure_apnspe(xpu_info);
			if (rc != AC_SUCCESS)
				break;
		}

		if (rc != AC_SUCCESS) {
			ERROR("AC_XPU ErrCode: %d, XPU configure APNSPE failure, rc: %d",
			      AC_ERR_XPU_CONFIGURE_APNSPE_FAILURE, rc);
			return AC_ERR_XPU_CONFIGURE_APNSPE_FAILURE;
		}
	} else {
		return AC_ERR_INPUT_VALIDATION;
	}

	ac_get_dynamic_protect_ranges(&pac_xpu_protected_ranges,
				      &uac_xpu_protected_ranges_count);
	for (i = 0; i < uac_xpu_protected_ranges_count; i++) {
		ac_xpu4_priv_info *xpu_info;

		if (g_hal_xpu_count == pac_xpu_protected_ranges[i].xpu_id)
			continue;

		if (ac_is_xpu_disabled(pac_xpu_protected_ranges[i].xpu_id))
			continue;

		rc = ac_init_xpu_and_get_info(pac_xpu_protected_ranges[i].xpu_id,
					      &xpu_info);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_xpu_configure_apnspe(xpu_info);
		if (rc != AC_SUCCESS)
			break;
	}

	if (rc != AC_SUCCESS) {
		ERROR("AC_XPU ErrCode: %d, XPU configure APNSPE failure, rc: %d",
		      AC_ERR_XPU_CONFIGURE_APNSPE_FAILURE, rc);
		return AC_ERR_XPU_CONFIGURE_APNSPE_FAILURE;
	}

	return AC_SUCCESS;
}

static AC_ERROR ac_write_static_signal_reg(uint32_t addr, uint32_t mask,
					    uint32_t value)
{
	uint32_t hwValue;

	hwValue = in_dword(addr);
	value = (value & mask) | (hwValue & ~mask);

	/*
	 * Always write even if value does not change, because some CSR's/bits
	 * are write-once and must not be allowed to change later
	 */
	out_dword(addr, value);
	hwValue = in_dword(addr);

	if (hwValue != value) {
		ERROR("AC_XPU ErrCode: %d, RegWrite mismatch  0x%x, expected: 0x%x, actual: 0x%x",
			AC_ERR_XPU_RG_WRITE_MISMATCH, addr, value, hwValue);
		return AC_ERR_XPU_RG_WRITE_MISMATCH;
	}

	return AC_SUCCESS;
}

/*
 * ac_set_ra_xpu_static_signals - Set RA XPU static signals
 *
 * @param enable_silent_logging	Whether to enable silent logging
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
int ac_set_ra_xpu_static_signals(bool enable_silent_logging)
{
	AC_ERROR ret = AC_SUCCESS;
	uint32_t i;
	const ac_ra_xpu_info *pac_ra_xpu_info = NULL;
	uint32_t uac_ra_xpu_info_count = 0;

	ac_get_ra_xpu_info(&pac_ra_xpu_info, &uac_ra_xpu_info_count);
	for (i = 0; i < uac_ra_xpu_info_count; i++) {
		const ac_ra_xpu_info *info = &pac_ra_xpu_info[i];
		uint32_t value;

		/*
		 * Write silent logging enable CSR first because it is write-once and
		 * may alias other static signal CSR
		 */
		if (info->static_signal_silent_logging) {
			value = enable_silent_logging ?
				info->static_signal_silent_logging->value : 0;
			ret = ac_write_static_signal_reg(info->static_signal_silent_logging->addr,
							 info->static_signal_silent_logging->mask,
							 value);
			if (ret != AC_SUCCESS)
				return ret;
		}

		if (info->static_signal_others) {
			uint32_t k;

			for (k = 0; k < info->static_signal_others_count; k++) {
				ret = ac_write_static_signal_reg(
					info->static_signal_others[k].addr,
					info->static_signal_others[k].mask,
					info->static_signal_others[k].value);
				if (ret != AC_SUCCESS)
					return ret;
			}
		}
	}

	return ret;
}

/*
 * ac_apply_static_xpu_config - AC xPU static configuration application
 * Parses the xPU configuration data passed and applies the
 * configuration in HW
 *
 * @param milestone	Initialization milestone
 * @param cfg		Configuration data
 * @param size		Configuration size
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_apply_static_xpu_config(ac_milestone milestone, void *cfg,
				     size_t size)
{
	AC_ERROR rc = AC_SUCCESS;
	ac_xpu_cfg *xpu_cfg = (ac_xpu_cfg *)cfg;
	uint32_t i = 0;

	do {
		if ((size < g_hal_xpu_count) && (cfg != NULL)) {
			for (i = 0; i < size; i++) {
				ac_xpu4_priv_info *xpu_info;

				if ((xpu_cfg[i].status == XPU_ENABLED) &&
				    ac_is_xpu_profile_matched(milestone,
							      xpu_cfg[i].profile_flags)) {
					rc = ac_init_xpu_and_get_info(xpu_cfg[i].xpu_id,
								      &xpu_info);
					if (rc != AC_SUCCESS)
						break;

#ifdef xPU_4_2_SUPPORT

					rc = ac_xpu_apply_static_config(milestone, xpu_info,
									&xpu_cfg[i]);
					if (rc != AC_SUCCESS)
						break;

					rc = ac_xpu_apply_xpress_config(xpu_info, &xpu_cfg[i]);
					if (rc != AC_SUCCESS)
						break;

					rc = ac_xpu_set_unmapped_region_perms(xpu_info,
									      &xpu_cfg[i]);
					if (rc != AC_SUCCESS)
						break;

					rc = ac_xpu_set_cfg_owner(xpu_info, &xpu_cfg[i]);
					if (rc != AC_SUCCESS)
						break;
#else
					rc = ac_xpu_apply_static_config(milestone, xpu_info,
									&xpu_cfg[i]);
					if (rc != AC_SUCCESS)
						break;
#endif
				}
			}
		} else {
			ERROR("AC_XPU ErrCode: %d, No XPU static policy available",
			      AC_ERR_NO_XPU_STATIC_POLICY);
			rc = AC_ERR_NO_XPU_STATIC_POLICY;
		}

		if ((rc != AC_SUCCESS) && (xpu_cfg != NULL)) {
			ERROR("AC_XPU ErrCode: %d, XPU static config failed for xpu_id: %u",
			      rc, xpu_cfg[i].xpu_id);
			break;
		}
	} while (0);

	return rc;
}

/*
 * ac_protect_region - xPU protect region based on start/size, xPU is picked
 * for programming. It is determined whether it has to be programmed in xPU RG
 * (or) Locking has to be done based Start/End/Permission matches
 * 1. If start/end doesn't match with any RG then new free RG is
 *    programmed/locked with details provided
 * 2. If start/end match with permissions for already programmed RG then
 *    locking is done and config verified
 * 3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000) and there
 *    is adjacent RG-B(0x1000--0x2000) then locking done in RG-A and unlocking
 *    done in RG-B
 *
 * @param start		Start address
 * @param size		Region size
 * @param wowp		Write-once write-protect flag
 * @param sd_perm_array	Security domain permission array
 * @param count		Number of permissions
 * @param merge		Whether to merge regions
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_protect_region(uint64_t start, uint64_t size, bool wowp,
			    ac_sec_domain_perm *sd_perm_array, uint32_t count,
			    bool merge)
{
	AC_ERROR rc;

	do {
		ac_xpu4_priv_info *xpu_info = {0};

		rc = ac_validate_address_region(start, size);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_validate_perm_array(sd_perm_array, count, false);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_get_xpu_info_from_range(start, size, &xpu_info);
		if (rc == AC_ERR_REGION_XPU_BOGUS)
			return rc;
		if (rc == AC_ERR_XPU_DISABLED)
			return AC_SUCCESS;
		if (rc != AC_SUCCESS)
			break;

		if (ac_is_xpu_disabled(xpu_info->xpu_id))
			return AC_SUCCESS;

		rc = ac_xpu_protect_region(xpu_info, start, size, wowp,
					   sd_perm_array, count, merge);
		if (rc != AC_SUCCESS)
			break;

		return AC_SUCCESS;
	} while (0);

	ERROR("AC_XPU ErrCode: %d, Protect failed - start: 0x%x%08x, size: 0x%x%08x, rc: %d\n",
		AC_ERR_XPU_PROTECT_REGION_FAILURE, (uint32_t)(start >> 32),
		(uint32_t)start, (uint32_t)(size >> 32), (uint32_t)size, rc);
	return rc;
}

/*
 * ac_unprotect_region - xPU picked up based on start/size
 * It is determined whether it has to be unlocked or it has to be disabled or both
 * If RG start/end matches exactly (or) falls completely within the address range passed
 * then based on lock details passed unlock is done and RG is tried for disablement
 * (might not if some QAD holds the lock). If no QAD lock is passed then RG tried for
 * disablement
 *
 * @param start		Start address
 * @param size		Region size
 * @param sd_array	Security domain array
 * @param count		Number of domains
 * @param blanket	Whether to apply blanket unlock
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_unprotect_region(uint64_t start, uint64_t size,
			      ac_sec_domain_id *sd_array, uint32_t count,
			      bool blanket)
{
	AC_ERROR rc;

	do {
		ac_xpu4_priv_info *xpu_info;

		rc = ac_validate_address_region(start, size);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_validate_domain_array(sd_array, count);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_get_xpu_info_from_range(start, size, &xpu_info);
		if (rc == AC_ERR_XPU_DISABLED)
			return AC_SUCCESS;
		if (rc == AC_ERR_REGION_XPU_BOGUS)
			return rc;

		if (rc != AC_SUCCESS)
			break;

		if (ac_is_xpu_disabled(xpu_info->xpu_id))
			return AC_SUCCESS;

		rc = ac_xpu_unprotect_region(xpu_info, start, size, sd_array,
					     count, blanket);
		if (rc != AC_SUCCESS)
			break;

		return AC_SUCCESS;
	} while (0);

	ERROR("AC_XPU ErrCode: %d, Unlock failed - start: 0x%x%08x, size: 0x%x%08x, rc: %d",
		AC_ERR_XPU_UNLOCK_REGION_FAILURE, (uint32_t)(start >> 32),
		(uint32_t)start, (uint32_t)(size >> 32), (uint32_t)size, rc);
	return rc;
}

/*
 * ac_error_reporting_control - Enable/Disable error reporting for the QADs passed
 * Validated against QADs on which SW is running
 *
 * @param start		Start address
 * @param size		Region size
 * @param sd_array	Security domain array
 * @param count		Number of domains
 * @param enable	Whether to enable error reporting
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_error_reporting_control(uint64_t start, uint64_t size,
				     ac_sec_domain_id *sd_array, uint32_t count,
				     bool enable)
{
	AC_ERROR rc;

	do {
		ac_xpu4_priv_info *xpu_info;

		rc = ac_validate_address_region(start, size);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_validate_domain_array(sd_array, count);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_get_xpu_info_from_range(start, size, &xpu_info);
		if (rc == AC_ERR_XPU_DISABLED)
			return AC_SUCCESS;
		if (rc == AC_ERR_REGION_XPU_BOGUS)
			return rc;
		if (rc != AC_SUCCESS)
			break;

		rc = ac_xpu_configure_client_error_reporting(xpu_info, sd_array,
							     count, enable);
		if (rc != AC_SUCCESS)
			break;

		return AC_SUCCESS;
	} while (0);

	ERROR("AC_XPU ErrCode: %d, failed - start: 0x%x%08x, size: 0x%x%08x, rc: %d",
		AC_ERR_XPU_ERROR_REPORTING_CTRL_FAILURE, (uint32_t)(start >> 32), (uint32_t)start,
		(uint32_t)(size >> 32),	(uint32_t)size, rc);
	return rc;
}

/*
 * ac_protect_region_by_xpu_id - Protect region based on xPU ID which will
 * protect the range of addresses
 * 1. If start/end doesn't match with any RG then new free RG is
 *    programmed/locked with details provided
 * 2. If start/end match with permissions for already programmed RG then
 *    locking is done and config verified
 * 3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000) and there
 *    is adjacent RG-B(0x1000--0x2000) then locking done in RG-A and unlocking
 *    done in RG-B
 *
 * @param xpu_id	XPU identifier
 * @param start		Start address
 * @param end		End address
 * @param wowp		Write-once write-protect flag
 * @param sd_perm_array	Security domain permission array
 * @param count		Number of permissions
 * @param merge		Whether to merge regions
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_protect_region_by_xpu_id(uint32_t xpu_id, uint64_t start,
				      uint64_t end, bool wowp,
				      ac_sec_domain_perm *sd_perm_array,
				      uint32_t count, bool merge)
{
	AC_ERROR rc;
	uint64_t size = 0;

	do {
		ac_xpu4_priv_info *xpu_id_info = NULL;

		rc = ac_validate_underflow_address_region(start, end);
		if (rc != AC_SUCCESS)
			break;
		size = end - start;

		rc = ac_validate_address_region(start, size);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_validate_perm_array(sd_perm_array, count, false);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_init_xpu_and_get_info(xpu_id, &xpu_id_info);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_xpu_protect_region(xpu_id_info, start, size, wowp,
					   sd_perm_array, count, merge);
		if (rc != AC_SUCCESS)
			break;

		return AC_SUCCESS;
	} while (0);

	ERROR("AC_XPU ErrCode: %d, failed - start: 0x%x%08x, size: 0x%x%08x, rc: %d",
		AC_ERR_XPU_PROTECT_REGION_FAILURE, (uint32_t)(start >> 32), (uint32_t)start,
		(uint32_t)(size >> 32), (uint32_t)size, rc);
	return rc;
}

/*
 * ac_unprotect_region_by_xpu_id - Unprotect region based on xPU ID
 * It is determined whether it has to be unlocked or it has to be disabled or both
 * If RG start/end matches exactly (or) falls completely within the address range passed
 * then based on lock details passed unlock is done and RG is tried for disablement
 * (might not if some QAD holds the lock). If no QAD lock is passed then RG tried for
 * disablement
 *
 * @param xpu_id	XPU identifier
 * @param start		Start address
 * @param end		End address
 * @param sd_array	Security domain array
 * @param count		Number of domains
 * @param blanket	Whether to apply blanket unlock
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_unprotect_region_by_xpu_id(uint32_t xpu_id, uint64_t start,
					uint64_t end, ac_sec_domain_id *sd_array,
					uint32_t count, bool blanket)
{
	AC_ERROR rc;
	uint64_t size = 0;

	do {
		ac_xpu4_priv_info *xpu_info = NULL;

		rc = ac_validate_underflow_address_region(start, end);
		if (rc != AC_SUCCESS)
			break;
		size = end - start;

		rc = ac_validate_address_region(start, size);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_validate_domain_array(sd_array, count);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_init_xpu_and_get_info(xpu_id, &xpu_info);
		if (rc != AC_SUCCESS)
			break;

		rc = ac_xpu_unprotect_region(xpu_info, start, size, sd_array,
					     count, blanket);
		if (rc != AC_SUCCESS)
			break;

		return AC_SUCCESS;
	} while (0);

	ERROR("AC_XPU ErrCode: %d, failed - start: 0x%x%08x, size: 0x%x%08x, rc: %d",
		AC_ERR_XPU_UNLOCK_REGION_FAILURE, (uint32_t)(start >> 32), (uint32_t)start,
		(uint32_t)(size >> 32),	(uint32_t)size, rc);
	return rc;
}
