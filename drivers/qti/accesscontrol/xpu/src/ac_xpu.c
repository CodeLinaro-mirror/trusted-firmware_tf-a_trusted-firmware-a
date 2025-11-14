/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * Access control initialization and configuration
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <common/debug.h>
#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#include "ac_xpu_isr.h"
#include "ac_xpu_target_info.h"
#include "hal_hwio.h"
#include "hal_xpu4.h"
#include "hal_xpu4_util.h"
#include "target_hwio.h"
#include "target_clock.h"

uint32_t g_hal_xpu_count = HAL_XPU2_COUNT;
static uint32_t enabled_profiles;

static void ac_init_profiles(ac_milestone milestone)
{
	/*
	 * Always init to zero as the enabled profiles can change across
	 * milestone calls
	 */
	enabled_profiles = 0;

	/* Milestone based flags need to be enabled as needed per milestone */
	switch (milestone) {
	case AC_MILESTONE_SDI_PASS2:
		enabled_profiles |= (AC_PROFILE_DEBUG_SDI_PASS2 | AC_PROFILE_DEFAULT);
		break;
	default:
		enabled_profiles |= AC_PROFILE_DEFAULT;
	}
}

static bool ac_is_profile_matched(ac_milestone milestone,
				   uint32_t profile_flags)
{
	if (profile_flags >= AC_STATIC_POLICY_PROFILES_COUNT || profile_flags == 0) {
		ERROR("AC_XPU ErrCode: %d, Invalid profile flags: 0x%x",
		      AC_LOG_PROFILE_INVALID, profile_flags);
	}

	return ((enabled_profiles & profile_flags) == profile_flags);
}

/*
 * Access control initialization
 *
 * @param milestone	Initialization milestone
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_init(ac_milestone milestone)
{
	ac_xpu_cfg *p_xpu_cfg = NULL;
	uint32_t *p_xpu_dbg_cfg = NULL;
	uint32_t xpu_cfg_count = 0, xpu_dbg_cfg_count = 0;
	AC_ERROR ret = AC_SUCCESS;

	/* Enable XPU Clocks at the beginning */
	ac_enable_xpu_clocks();

	/* Register XPU interrupts after clock initialization */
	ac_register_xpu_interrupts();

	/*
	 * If Separate config Image is not loaded, initialize the p_acglobaldata
	 * with Legacy config structures
	 */
	p_ac_global_data = (ac_global_data_t *)&g_ac_global_data_legacy_config;

	if (ac_xpu_get_xpu_config_for_milestone(milestone, &p_xpu_cfg,
						 &xpu_cfg_count,
						 &p_xpu_dbg_cfg,
						 &xpu_dbg_cfg_count) != AC_SUCCESS)
		return AC_SHARED_DATA_INIT_FAILURE;

	if ((milestone == AC_MILESTONE_COLD_BOOT) ||
	    (milestone == AC_MILESTONE_SDI_PASS2)) {
		ret = ac_set_ra_xpu_static_signals(ac_is_silent_logging_enabled());
		if (ret != AC_SUCCESS)
			return AC_ERR_XPU_INIT_FAILED;
	}

	if ((p_xpu_cfg != NULL) && (xpu_cfg_count != 0)) {
		ret = ac_configure_xpu_apnspe(milestone, p_xpu_cfg, xpu_cfg_count);
		if (ret != AC_SUCCESS)
			return AC_ERR_XPU_INIT_FAILED;

		ret = ac_apply_static_xpu_config(milestone, (void *)p_xpu_cfg,
						 xpu_cfg_count);

		if (ret != AC_SUCCESS)
			return AC_ERR_NO_XPU_STATIC_POLICY;
	} else {
		return AC_ERR_NO_XPU_STATIC_POLICY;
	}

	/* Set Config Owner to Secure */
	ac_xpu_v4_set_config_owner_secure();

	if (is_dload_magic_set()) {
		/*
		 * TF-A DDR and OCIMEM region is MPU protected from all masters and configured
		 * as RAZ during crashdump path. Disable CLERE and INTR_ENABLE
		 * to avoid a violation when uboot reads it for crash dump
		 */
		ac_xpu4_priv_info *xpu_info = ac_xpu_get_xpu_info(DDR_MPU_XPU_ID);

		if (xpu_info != NULL) {
			ac_xpu4_qad_vector readvalue = ac_xpu_read_clere(xpu_info);

			readvalue = readvalue & ~DDR_MPU_XPU_CLERE_NS_BMSK;
			/* Disable Client Error reporting of DDR MPU from AP_NS master */
			ac_xpu_write_clere(xpu_info, readvalue);
			/* Disable xPU Interrupt for DDR MPU */
			HWIO_OUTF(TCSR_XPU_INTR0_ENABLE, DDR_MPU_IRQ_ENABLE, 0);
		}
		xpu_info = ac_xpu_get_xpu_info(OCIMEM_MPU_XPU_ID);

		if (xpu_info != NULL) {
			ac_xpu4_qad_vector readvalue = ac_xpu_read_clere(xpu_info);

			readvalue = readvalue & ~OCIMEM_MPU_XPU_CLERE_NS_BMSK;
			/* Disable Client Error reporting of OCIMEM MPU from AP_NS master */
			ac_xpu_write_clere(xpu_info, readvalue);
			/* Disable xPU Interrupt for OCIMEM MPU */
			HWIO_OUTF(TCSR_XPU_INTR0_ENABLE, OCIMEM_MPU_IRQ_ENABLE, 0);
		}
	}

	return AC_SUCCESS;
}

/*
 * API to protect the memory regions
 *
 * @param start_addr	Start address of region to protect
 * @param end_addr	End address of region to protect
 * @param sd_array	Security domain permission array
 * @param sd_count	Number of security domains
 * @param merge		Whether to merge with existing regions
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_prot_region(uintptr_t start_addr, uintptr_t end_addr,
			     ac_sec_domain_perm *sd_array, uint32_t sd_count,
			     bool merge)
{
	uint64_t size;
	AC_ERROR rc = AC_FAILURE;

	if (end_addr < start_addr)
		return AC_FAILURE;

	if ((sd_array == NULL) || (sd_count == 0) || (sd_count > AC_SD_END_MARKER))
		return AC_FAILURE;

	size = end_addr - start_addr;
	rc = ac_protect_region(start_addr, size, false, sd_array, sd_count, merge);
	if (rc == AC_ERR_REGION_XPU_BOGUS)
		return AC_ERR_REGION_XPU_NOT_FOUND;

	if (rc != AC_SUCCESS)
		return AC_ERR_XPU_PROTECT_REGION_FAILURE;

	return AC_SUCCESS;
}

/*
 * API to unprotect the memory regions
 *
 * @param start_addr	Start address of region to unprotect
 * @param end_addr	End address of region to unprotect
 * @param sd_unlock_qad	Security domain unlock array
 * @param sd_count	Number of security domains
 * @param blanket	Whether to apply blanket unlock
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_unprot_region(uintptr_t start_addr, uintptr_t end_addr,
			       ac_sec_domain_id *sd_unlock_qad, uint32_t sd_count,
			       bool blanket)
{
	uint64_t size;
	AC_ERROR rc = AC_FAILURE;

	if (end_addr < start_addr)
		return AC_FAILURE;

	if ((sd_unlock_qad == NULL) || (sd_count == 0) ||
	    (sd_count > AC_SD_END_MARKER))
		return AC_FAILURE;

	size = end_addr - start_addr;
	rc = ac_unprotect_region(start_addr, size, sd_unlock_qad, sd_count, blanket);
	if (rc == AC_ERR_REGION_XPU_BOGUS)
		return AC_ERR_REGION_XPU_NOT_FOUND;

	if (rc != AC_SUCCESS)
		return AC_ERR_XPU_UNLOCK_REGION_FAILURE;

	return AC_SUCCESS;
}

/*
 * API to enable or disable error reporting
 *
 * @param start_addr	Start address of region
 * @param end_addr	End address of region
 * @param sd_err_rep_qad	Security domain error reporting array
 * @param sd_count	Number of security domains
 * @param enable	Whether to enable error reporting
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_configure_error_reporting(uintptr_t start_addr, uintptr_t end_addr,
				       ac_sec_domain_id *sd_err_rep_qad,
				       uint32_t sd_count, bool enable)
{
	uint64_t size;
	AC_ERROR rc = AC_FAILURE;

	if (end_addr < start_addr)
		return AC_FAILURE;

	if ((sd_err_rep_qad == NULL) || (sd_count == 0) ||
	    (sd_count > AC_SD_END_MARKER))
		return AC_FAILURE;

	size = end_addr - start_addr;

	rc = ac_error_reporting_control(start_addr, size, sd_err_rep_qad,
					sd_count, enable);
	if (rc == AC_ERR_REGION_XPU_BOGUS)
		return AC_ERR_REGION_XPU_NOT_FOUND;

	if (rc != AC_SUCCESS)
		return AC_ERR_XPU_ERROR_REPORTING_CTRL_FAILURE;

	return AC_SUCCESS;
}

/*
 * API to protect the memory regions by XPU ID
 *
 * @param xpu_id	XPU identifier
 * @param start_addr	Start address of region to protect
 * @param end_addr	End address of region to protect
 * @param wowp		Write-once write-protect flag
 * @param sd_perm_array	Security domain permission array
 * @param sd_count	Number of security domains
 * @param merge		Whether to merge with existing regions
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_id_protect_region(uint32_t xpu_id, uint64_t start_addr,
				   uint64_t end_addr, bool wowp,
				   ac_sec_domain_perm *sd_perm_array,
				   uint32_t sd_count, bool merge)
{
	AC_ERROR rc = AC_FAILURE;

	if (end_addr < start_addr)
		return AC_FAILURE;

	if ((sd_perm_array == NULL) || (sd_count == 0) ||
	    (sd_count > AC_SD_END_MARKER))
		return AC_FAILURE;

	rc = ac_protect_region_by_xpu_id(xpu_id, start_addr, end_addr, wowp,
					 sd_perm_array, sd_count, merge);
	if (rc != AC_SUCCESS)
		return AC_ERR_XPU_PROTECT_REGION_FAILURE;

	return AC_SUCCESS;
}

/*
 * API to unprotect the memory regions by XPU ID
 *
 * @param xpu_id	XPU identifier
 * @param start_addr	Start address of region to unprotect
 * @param end_addr	End address of region to unprotect
 * @param sd_unlock_qad	Security domain unlock array
 * @param sd_count	Number of security domains
 * @param blanket	Whether to apply blanket unlock
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_id_unprotect_region(uint32_t xpu_id, uint64_t start_addr,
				     uint64_t end_addr,
				     ac_sec_domain_id *sd_unlock_qad,
				     uint32_t sd_count, bool blanket)
{
	AC_ERROR rc = AC_FAILURE;

	if (end_addr < start_addr)
		return AC_FAILURE;

	if ((sd_unlock_qad == NULL) || (sd_count == 0) ||
	    (sd_count > AC_SD_END_MARKER))
		return AC_FAILURE;

	rc = ac_unprotect_region_by_xpu_id(xpu_id, start_addr, end_addr,
					   sd_unlock_qad, sd_count, blanket);
	if (rc != AC_SUCCESS)
		return AC_ERR_XPU_UNLOCK_REGION_FAILURE;

	return AC_SUCCESS;
}

/*
 * Enable XPU Clocks
 *
 * This function enables target-specific clocks required for XPU operation.
 * The clock configuration is defined in target_clock.h and is data-driven
 * to allow easy customization per target. Uses HWIO_ADDR macros for register
 * addresses and sets CLK_ENABLE bit (bit 0) for each clock.
 *
 * @return 0 on success
 */
int ac_enable_xpu_clocks(void)
{
	uint32_t i;

	/* Iterate through target-specific clocks and enable each */
	for (i = 0; i < AC_XPU_CLOCKS_COUNT; i++)
		*((uint32_t *)((uintptr_t)ac_xpu_clocks[i])) |= 0x1;

	return 0; /* E_SUCCESS */
}

/*
 * Based on milestone in which API is invoked fills up the
 * xPU policy configuration data to be applied
 *
 * @param milestone	Initialization milestone
 * @param xpu_cfg	Pointer to store XPU configuration
 * @param xpu_cfg_count	Pointer to store XPU configuration count
 * @param xpu_dbg_cfg	Pointer to store XPU debug configuration
 * @param xpu_dbg_cfg_count	Pointer to store XPU debug configuration count
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_get_xpu_config_for_milestone(ac_milestone milestone,
					      ac_xpu_cfg **xpu_cfg,
					      uint32_t *xpu_cfg_count,
					      uint32_t **xpu_dbg_cfg,
					      uint32_t *xpu_dbg_cfg_count)
{
	error_if_ac_config_fail();
	if ((xpu_cfg == NULL) || (xpu_cfg_count == NULL))
		return AC_FAILURE;

	if ((xpu_dbg_cfg == NULL) || (xpu_dbg_cfg_count == NULL))
		return AC_FAILURE;

	ac_init_profiles(milestone);

	*xpu_cfg = (ac_xpu_cfg *)p_ac_global_data->p_global_data_ptr[AC_XPUCFG_ARRAY];
	*xpu_cfg_count = *(uint32_t *)p_ac_global_data->p_global_data_ptr[AC_XPUCFG_ARRAY_SIZE_PTR];
	*xpu_dbg_cfg = (uint32_t *)p_ac_global_data->p_global_data_ptr[AC_XPUDBGAR_ARRAY];
	*xpu_dbg_cfg_count = *(uint32_t *)
		p_ac_global_data->p_global_data_ptr[AC_XPUDBGAR_ARRAY_SIZE_PTR];
	g_hal_xpu_count = HAL_XPU2_COUNT;
	return AC_SUCCESS;
}

/*
 * Returns TRUE based on profile flags to be enabled on target
 * This operates on RG profiles
 *
 * @param milestone	Initialization milestone
 * @param profile_flags	Profile flags to check
 *
 * @return true if profile matches, false otherwise
 */
bool ac_is_rg_profile_matched(ac_milestone milestone, uint32_t profile_flags)
{
	return ac_is_profile_matched(milestone, profile_flags);
}

/*
 * Initialize Access Control (XPU) driver
 *
 * This function performs complete XPU initialization including:
 * - AC driver initialization with cold boot milestone
 * - Secure memory region protection (test code - can be disabled)
 *
 * All error handling and logging is contained within this module.
 */
void ac_xpu_init(void)
{
	/* Initialize Access Control (XPU) driver */
	AC_ERROR ac_ret = ac_init(AC_MILESTONE_COLD_BOOT);

	if (ac_ret != AC_SUCCESS) {
		ERROR("AC_XPU ErrCode: %d, Access Control initialization failed: %d\n",
		      AC_ERR_XPU_INIT_FAILED, ac_ret);
		return;
	}

	INFO("Access Control initialized successfully\n");

}

/*
 * Returns TRUE based on profile flags to be enabled on target
 * Say if DEBUG profile is passed and on secure device it returns FALSE
 *
 * @param milestone	Initialization milestone
 * @param profile_flags	Profile flags to check
 *
 * @return true if profile matches, false otherwise
 */
bool ac_is_xpu_profile_matched(ac_milestone milestone, uint32_t profile_flags)
{
	/* Default xPU profile */
	if (profile_flags == AC_PROFILE_DEFAULT)
		return true;

	return ac_is_profile_matched(milestone, profile_flags);
}
