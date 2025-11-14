/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef ACCESS_CONTROL_XPU_H
#define ACCESS_CONTROL_XPU_H

#include <common/debug.h>
#include <qti_xpu_err.h>

/*
 * Determine if the given xPU is disabled
 *
 * @param xpu_id	xPU ID of the xPU to check if disabled
 *
 * @return TRUE is disabled else FALSE
 */
bool ac_is_xpu_disabled(HAL_xpu2_XPU2Type xpu_id);

/*
 * Validate the secure domain ID details passed
 *
 * @param sd_array	Array holding secure domain ID
 * @param count		Number of elements in array passed
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_validate_domain_array(ac_sec_domain_id *sd_array, uint32_t count);

/*
 * Validate the alignment and overflow of address range passed
 *
 * @param start		start address to be validated
 * @param size		size to be validated
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_validate_address_region(uint64_t start, uint64_t size);

/*
 * Get the AC secure domain ID for execution environment
 * Based on compilation flags respective domains are passed
 *
 * @return AC secure domain ID
 */
ac_sec_domain_id ac_xpu4_get_env_sec_domain_id(void);

/*
 * Validate the secure domain permission details passed
 *
 * @param sd_perm_array		Array holding secure domain ID, R/W permission
 *				and lock details
 * @param count			Number of elements in array passed
 * @param check_domain_only	Skip validation of R/W and lock permission
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_validate_perm_array(ac_sec_domain_perm *sd_perm_array,
				 uint32_t count,
				 bool check_domain_only);

/*
 * AC Get xPU ID from address range
 *
 * @param start		start address of client space protected by an xPU
 * @param size		size from the start address protected
 * @param xpu_id	xPU identifier corresponding client address
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_get_xpu_id_from_range(uint64_t start,
				  uint64_t size,
				  HAL_xpu2_XPU2Type *xpu_id);

/*
 * API to enable APNSPE
 *
 * @param milestone	Milestone call in which API is invoked
 * @param xpu_cfg	Array holding the xPU configuration data
 * @param count		Number of elements in the array
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_configure_xpu_apnspe(ac_milestone milestone, ac_xpu_cfg *xpu_cfg,
				  uint32_t count);

/*
 * Access control initialization.
 *
 * @pre xPU Clocks has to be inited
 *
 * @param milestone	Milestone call in which API is invoked
 * @param cfg		Array address holding the xPU configuration data
 * @param size		Size of array of xPU configuration elements passed
 *
 * @return Success/error otherwise
 *
 * @details xPU initialization and configuration happens for all xPUs passed
 *          in the configuration list
 */
AC_ERROR ac_apply_static_xpu_config(ac_milestone milestone, void *cfg,
				     size_t size);

/*
 * API to protect/lock region in xPU
 *
 * @details xPU protect region based on start/size, xPU is picked for programming
 *          It is determined whether it has to be programmed in xPU RG (or)
 *          Locking has to be done based Start/End/Permission matches
 *          1. If start/end doesn't match with any RG then new free RG is
 *             programmed/locked with details provided
 *          2. If start/end match with permissions for already programmed RG
 *             then locking is done and config verified
 *          3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000)
 *             and there is adjacent RG-B(0x1000--0x2000)
 *             then locking done in RG-A and unlocking done in RG-B
 *
 * @pre Clocks for xPU protecting the client address space has to be enabled
 *
 * @param start		start address which has to be protected
 *			Has to be aligned with MPU width size
 * @param size		Size of region which has to be protected (alignment same as above)
 * @param wowp		Write protection to be enabled for the configuration
 * @param sd_perm_array	Base address of Array structure holding input Secure Domain ID/VMID,
 *			R/W Permission, Lock details
 * @param count		Number of elements passed in ACSecDomainPerm field
 * @param merge		Merge flag decides whether address has to be merged if
 *			consecutive with other address range with same permissions
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_protect_region(uint64_t start,
			    uint64_t size,
			    bool wowp,
			    ac_sec_domain_perm *sd_perm_array,
			    uint32_t count,
			    bool merge);

/*
 * API to unlock/unprotect the region in xPU
 *
 * @details xPU picked up based on start/size
 *          It is determined whether it has to be unlocked or it has to be disabled or both
 *          If RG start/end matches exactly (or) falls completely within the address range passed
 *          then based on lock details passed unlock is done and RG is tried for disablement
 *          (might not if some QAD holds the lock). If no QAD lock is passed then RG tried for
 *          disablement
 *
 * @param start		start address which has to be unprotected
 *			Has to be aligned with MPU width size
 * @param size		Size of region which has to be unprotected (alignment same as above)
 * @param sd_array	Base address of array holding the AC secure domain ID
 *			(APP-S or APP-NS or both) for which unlock has to be done
 * @param count		Number of elements passed in sdUnlockQAD array
 * @param blanket	Blanket has to be set TRUE for unlocking complete range
 *			spun across multiple RGs.
 *			Checks the static RGs as well.
 *			If Blanket is FALSE looks for exact match in single RG.
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_unprotect_region(uint64_t start,
			      uint64_t size,
			      ac_sec_domain_id *sd_array,
			      uint32_t count,
			      bool blanket);

/*
 * API to enable or disable error reporting
 *
 * @param start		start address for which error has to be controlled
 * @param size		Size for which error has to be controlled
 * @param sd_array	Base address of array holding the AC secure domain ID
 *			(APP-S or APP-NS or both) for which error reporting has to be disabled
 * @param count		Number of elements passed in sdUnlockQAD array
 * @param enable	Field to identify error has to be enabled or disabled
 *
 * @return Success/error otherwise
 *
 * @details From start/end address, xPU is identified and error reporting is enabled or
 *          disabled for that xPU. It has to be noted error reporting is at xPU level,
 *          which means if particular address is passed for this API in IMEM, then it disables
 *          error reporting for the whole IMEM MPU.
 */
AC_ERROR ac_error_reporting_control(uint64_t start,
				     uint64_t size,
				     ac_sec_domain_id *sd_array,
				     uint32_t count,
				     bool enable);

/*
 * API to drive the static signals for RA xPU
 *
 * @param enable_silent_logging	Control parameter to enable or disable silent logging for RA xPU
 *
 * @return Success/error otherwise
 *
 * @details Iterates through all RA xPU register/data configuration and programs them in xPU HW
 */
int ac_set_ra_xpu_static_signals(bool enable_silent_logging);

/*
 * API to protect/lock region in xPU
 *
 * @details xPU protect region based on start/end of xpu_id
 *          It is determined whether it has to be programmed in xPU RG (or)
 *          Locking has to be done based Start/End/Permission matches
 *          1. If start/end doesn't match with any RG then new free RG is
 *             programmed/locked with details provided
 *          2. If start/end match with permissions for already programmed RG
 *             then locking is done and config verified
 *          3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000)
 *             and there is adjacent RG-B(0x1000--0x2000)
 *             then locking done in RG-A and unlocking done in RG-B
 *
 * @pre Clocks for xPU protecting the client address space has to be enabled
 *
 * @param xpu_id	Range protected based on xPU ID
 * @param start		start address which has to be protected
 *			Has to be aligned with MPU width size
 * @param end		End of region which has to be protected (alignment same as above)
 * @param wowp		Write protection to be enabled for the configuration
 * @param sd_perm_array	Base address of Array structure holding input Secure Domain ID/VMID,
 *			R/W Permission, Lock details
 * @param count		Number of elements passed in ACSecDomainPerm field
 * @param merge		Merge flag decides whether address has to be merged if
 *			consecutive with other address range with same permissions
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_protect_region_by_xpu_id(uint32_t xpu_id,
				      uint64_t start,
				      uint64_t end,
				      bool wowp,
				      ac_sec_domain_perm *sd_perm_array,
				      uint32_t count,
				      bool merge);

/*
 * API to unlock/unprotect the region in xPU
 *
 * @details xPU protect region based on start/end of xpu_id
 *          It is determined whether it has to be unlocked or it has to be disabled or both
 *          If RG start/end matches exactly (or) falls completely within the address range passed
 *          then based on lock details passed unlock is done and RG is tried for disablement
 *          (might not if some QAD holds the lock). If no QAD lock is passed then RG tried for
 *          disablement
 *
 * @param xpu_id	Range protected based on xPU ID
 * @param start		start address which has to be unprotected
 *			Has to be aligned with MPU width size
 * @param end		End of region which has to be unprotected (alignment same as above)
 * @param sd_array	Base address of array holding the AC secure domain ID
 *			(APP-S or APP-NS or both) for which unlock has to be done
 * @param count		Number of elements passed in sdUnlockQAD array
 * @param blanket	Blanket has to be set TRUE for unlocking complete range
 *			spun across multiple RGs.
 *			Checks the static RGs as well.
 *			If Blanket is FALSE looks for exact match in single RG.
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_unprotect_region_by_xpu_id(uint32_t xpu_id,
					uint64_t start,
					uint64_t end,
					ac_sec_domain_id *sd_array,
					uint32_t count,
					bool blanket);

/*
 * Validate the alignment and underflow of address range passed
 *
 * @param start		start address to be validated
 * @param end		end address to be validated
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_validate_underflow_address_region(uint64_t start, uint64_t end);

/*
 * API to enable APNSPE
 *
 * @param xpu_id	xpu id of dynamic initialize api
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_configure_dynamic_xpu_apnspe(uint32_t xpu_id);

/*
 * Check if download magic is set (crash dump mode)
 *
 * @return TRUE if download magic is set, FALSE otherwise
 */
bool is_dload_magic_set(void);

#endif /* ACCESS_CONTROL_XPU_H */
