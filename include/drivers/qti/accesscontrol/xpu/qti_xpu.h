/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef QTI_XPU_H
#define QTI_XPU_H

#include <stdint.h>
#include <stdbool.h>
#include <qti_xpu_err.h>

/*----------------------------------------------------------------------------
 * DEFINITIONS AND TYPE DECLARATIONS
 * -------------------------------------------------------------------------
 */

/**
 * Different AC milestones
 */
typedef enum {
	AC_MILESTONE_COLD_BOOT = 0,
	AC_MILESTONE_SDI_PASS2 = 1,
} ac_milestone;

/**
 * Access control secure domain ID's separated in xPU HW
 */
typedef enum ac_sec_domain_id {
	AC_SD_AP_SEC = 0,
	AC_SD_TME_ROM = 1,
	AC_SD_TME_FW = 2,
	AC_SD_DEBUG = 3,
	AC_SD_AOP = 4,
	AC_SD_MODEM = 5,
	AC_SD_SP = 6,
	AC_SD_AP_NSEC = 7,
	AC_SD_SAFETY_MGR = 8,
	AC_SD_OOBM_NS = 9,
	AC_SD_OOBM_SEC = 10,
	AC_SD_QECP_DEBUG = 11,
	AC_SD_END_MARKER
} ac_sec_domain_id;

/**
 * Macros for the read/write configuration specification
 */
typedef enum {
	AC_RW_NONE = 0,
	AC_READ_ONLY = 1,
	AC_WRITE_ONLY = 2,
	AC_READ_WRITE = 3,
	AC_RW_END_MARKER = 4,
} ac_xpu_permission;

/**
 * Access control secure domain ID's separated in xPU HW
 */
typedef enum ac_qad_lock {
	AC_SD_NOP = 0,
	AC_SD_LOCK = 1,
	AC_SD_MAX_OP = 2,
} ac_qad_lock;

/**
 * Structure representing the AC secure domain details passed for xPU lock/protect
 */
typedef struct ac_sec_domain_perm {
	ac_sec_domain_id sd_id;
	ac_xpu_permission perm; /* RO, RW */
	ac_qad_lock lock;
} ac_sec_domain_perm;

/*----------------------------------------------------------------------------
 * FUNCTION DECLARATIONS
 * -------------------------------------------------------------------------
 */

/**
 * @brief Access control initialization.
 *
 * @pre xPU/VMIDMT Clocks has to be inited
 *
 * @param milestone The milestone where this call is invoked
 *
 * @return Success/error otherwise
 *
 * @details xPU initialization and VMIDMT/SMMU configuration for DMA masters
 *          Enable the APNSPE in xPUs (per xPU) and run debug QAD for xPU
 */
AC_ERROR ac_init(ac_milestone milestone);

/**
 * @brief API to protect the memory regions
 *
 * @pre xPU initialization has to be done
 *
 * @param start_addr start address which has to be protected
 *                   Has to be aligned with MPU width size
 *
 * @param end_addr End address which has to be protected (alignment same as above)
 *
 * @param sd_array Base address of Array structure holding input Secure Domain ID/VMID,
 *                 R/W Permission, Lock details
 *
 * @param sd_count Number of elements passed in ac_sec_domain_perm field
 *
 * @param merge The merge flag decides whether address has to be merged if consecutive with other
 *              address range with same permissions
 *              For first segment loaded for image this has to be set to FALSE.
 *              During segmented image authentication of same image this has to be set TRUE
 *              This prevents merging 2 images with same permissions.
 *
 * @return Success/error otherwise
 *
 * @details Protect the address regions as per the secure domain details passed
 *          Sending the lock for only APP-S or APP-NS or both locks the RG/partition
 *          after protection/configuration. Once resource group are locked its configuration
 *          are not modifiable.
 *          If resource is unlocked for the last locked owner then it gets disabled
 *          Note : If RG is shared between TZ and TME_FW, if TZ locks and then TME_FW
 *          locks it. RG is disabled only after both unlocks it. In same case if only
 *          TZ locks and unlocks before TME_FW then also RG will be disabled.
 *          If memory is donated(e.g.TME_FW) then there is no necessity for locking to
 *          APPS/TZ QAD. TME_FW
 *          locks and validates for exclusive TME_FW permission.
 *          If memory is shared(e.g.TZ+TME_FW) then along with configuration APPS/TZ QAD lock
 *          has to be done. TME_FW locks and validates the permission for TZ+TME_FW.
 */
AC_ERROR ac_xpu_prot_region(uintptr_t start_addr, uintptr_t end_addr,
			     ac_sec_domain_perm *sd_array, uint32_t sd_count, bool merge);

/**
 * @brief API to unprotect the memory regions
 *        RG will be unlocked for current QAD only. So RG is still enabled if lock is hold by
 *        other QAD
 *
 * @pre xPU initialization has to be done
 *
 * @param start_addr start address which has to be unprotected
 *                   Has to be aligned with MPU width size
 *
 * @param end_addr End address which has to be unprotected (alignment same as above)
 *
 * @param sd_unlock_qad Base address of array holding the AC secure domain ID
 *                      (APP-S or APP-NS or both) for which unlock has to be done
 *
 * @param sd_count Number of elements passed in sd_unlock_qad array
 *
 * @param blanket Has to be set TRUE for unlocking complete range spun across multiple RGs.
 *                Checks the static RGs as well.
 *                If Blanket is FALSE looks for exact match in single RG.
 *
 * @return Success/error otherwise
 *
 * @details xPU initialization and VMIDMT/SMMU configuration done
 */
AC_ERROR ac_xpu_unprot_region(uintptr_t start_addr, uintptr_t end_addr,
			       ac_sec_domain_id *sd_unlock_qad, uint32_t sd_count, bool blanket);

/**
 * @brief Initialize Access Control (XPU) driver with platform-specific configuration
 *
 * This function performs complete XPU initialization including:
 * - AC driver initialization with cold boot milestone
 * - Secure memory region protection
 *
 * All XPU-specific implementation details including milestones, error codes,
 * and secure region protection are encapsulated within this function.
 * Platform code only needs to call this single API.
 *
 * @details This function replaces the need for platform code to directly
 *          call ac_init() and handle XPU-specific configuration details.
 */
void ac_xpu_init(void);

#endif /* QTI_XPU_H */
