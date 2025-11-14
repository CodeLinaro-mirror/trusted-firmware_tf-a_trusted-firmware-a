/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef AC_XPU_H
#define AC_XPU_H

#include <cdefs.h>
#include <stdbool.h>
#include <stdint.h>

#include <qti_xpu.h>

/* QAD bit configurations - Bitmask to be passed for R/W permissions */
#define INVALID_QAD_BIT		(0)
#define APP_SEC_QAD_BIT		((1UL << 0) | (1UL << 31))
#define APP_NSEC_QAD_BIT	((1UL << 0) | (1UL << 30))
#define TME_ROM_QAD_BIT		(1UL << 1)
#define TME_FW_QAD_BIT		(1UL << 2)
#define DEBUG_QAD_BIT		(1UL << 3)
#define AOP_QAD_BIT		(1UL << 4)
#define MODEM_QAD_BIT		(1UL << 5)
#define PRIME_QAD_BIT		(1UL << 6)
#define RESERVED_7_QAD_BIT	(1UL << 7)
#define RESERVED_8_QAD_BIT	(1UL << 8)
#define RESERVED_9_QAD_BIT	(1UL << 9)
#define RESERVED_10_QAD_BIT	(1UL << 10)
#define RESERVED_11_QAD_BIT	(1UL << 11)

#define DBGAR_OFFSET		0x304

/* Register tuple structure for interrupt configuration */
typedef struct {
	uint32_t addr;	/* Register address */
	uint32_t mask;	/* Register mask */
} ac_register_tuple;

/* XPU error mapping structure for interrupt bit position to XPU mapping */
typedef struct {
	uint32_t xpu_id;	/* XPU identifier enum */
	const char *name;	/* XPU name string */
} ac_xpu_err_mapping_t;

/*
 * API to enable or disable error reporting
 *
 * @param start_addr	start address for which error has to be controlled
 * @param end_addr	End address for which error has to be controlled
 * @param sd_unlock_qad	Base address of array holding the AC secure domain ID
 *			(APP-S or APP-NS or both) for which error reporting
 *			has to be disabled
 * @param sd_count	Number of elements passed in sd_unlock_qad array
 * @param enable	Field to identify error has to be enabled or disabled
 *
 * @return Success/error otherwise
 *
 * From start/end address, xPU is identified and error reporting is enabled or
 * disabled for that xPU. It has to be noted error reporting is at xPU level,
 * which means if particular address is passed for this API in IMEM, then it
 * disables error reporting for the whole IMEM MPU.
 */
AC_ERROR ac_configure_error_reporting(uintptr_t start_addr, uintptr_t end_addr,
				       ac_sec_domain_id *sd_unlock_qad,
				       uint32_t sd_count, bool enable);

/*
 * API to protect/lock region in xPU
 *
 * xPU protect region based on start/end of xpu_id
 * It is determined whether it has to be programmed in xPU RG (or)
 * Locking has to be done based Start/End/Permission matches
 * 1. If start/end doesn't match with any RG then new free RG is
 *    programmed/locked with details provided
 * 2. If start/end match with permissions for already programmed RG then
 *    locking is done and config verified
 * 3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000) and
 *    there is adjacent RG-B(0x1000--0x2000) then locking done in RG-A and
 *    unlocking done in RG-B
 *
 * @param xpu_id		Range protected based on xPU ID
 * @param start_addr		start address which has to be protected
 *				Has to be aligned with MPU width size
 * @param end_addr		End of region which has to be protected
 *				(alignment same as above)
 * @param wowp			Write protection to be enabled for the
 *				configuration
 * @param sd_perm_array		Base address of Array structure holding input
 *				Secure Domain ID/VMID, R/W Permission, Lock
 *				details
 * @param sd_count		Number of elements passed in sd_perm_array field
 * @param merge			Merge flag decides whether address has to be
 *				merged if consecutive with other address range
 *				with same permissions
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_id_protect_region(uint32_t xpu_id, uint64_t start_addr,
				   uint64_t end_addr, bool wowp,
				   ac_sec_domain_perm *sd_perm_array,
				   uint32_t sd_count, bool merge);

/*
 * API to unlock/unprotect the region in xPU
 *
 * xPU protect region based on start/end of xpu_id
 * It is determined whether it has to be unlocked or it has to be disabled or
 * both. If RG start/end matches exactly (or) falls completely within the
 * address range passed then based on lock details passed unlock is done and
 * RG is tried for disablement (might not if some QAD holds the lock). If no
 * QAD lock is passed then RG tried for disablement
 *
 * @param xpu_id		Range protected based on xPU ID
 * @param start_addr		Start address which has to be unprotected.
 * @param end_addr		End of region which has to be unprotected.
 * @param sd_unlock_qad		Base address of array holding the AC secure
 *				domain ID (APP-S or APP-NS or both) for which
 *				unlock has to be done
 * @param sd_count		Number of elements passed in sd_unlock_qad array
 * @param blanket		Blanket has to be set TRUE for unlocking
 *				complete range spun across multiple RGs.
 *				Checks the static RGs as well.
 *				If Blanket is FALSE looks for exact match in
 *				single RG.
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_id_unprotect_region(uint32_t xpu_id, uint64_t start_addr,
				     uint64_t end_addr,
				     ac_sec_domain_id *sd_unlock_qad,
				     uint32_t sd_count, bool blanket);

/*
 * API to update XPU configuration owner to secure domain
 *
 * Sets the XPU configuration owner to secure domain, ensuring that only
 * secure software can modify XPU configurations.
 *
 * @return None
 */
void ac_xpu_v4_set_config_owner_secure(void);

/*
 * API to enable XPU clocks
 *
 * Enables the necessary clocks for XPU operation. This function must be
 * called before accessing XPU registers to ensure proper functionality.
 *
 * @return Success/error code
 */
int ac_enable_xpu_clocks(void);

/*
 * API to check if silent logging is enabled
 *
 * Checks the current state of silent logging configuration for XPU
 * error reporting.
 *
 * @return true if silent logging is enabled, false otherwise
 */
bool ac_is_silent_logging_enabled(void);

#endif /* AC_XPU_H */
