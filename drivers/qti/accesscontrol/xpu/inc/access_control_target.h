/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef ACCESS_CONTROL_TARGET_H
#define ACCESS_CONTROL_TARGET_H

#include <stddef.h>
#include <stdint.h>
#include <lib/utils_def.h>

#include <common/debug.h>
#include <qti_xpu_err.h>

#include "hal_xpu4.h"

/*
 * Each of the definitions and enumerations in this file must be aligned with
 * their values in the ACPOLICY PW to allow proper parsing AC Policy images.
 */

#define AC_MAGIC_COOKIE			0x65677071
#define AC_HYP_MAGIC_COOKIE		0xebf20486

typedef enum {
	AC_IS_XPU_ENABLED				= 0,
	AC_POLICYVERSION_BASE				= 1,
	AC_POLICYTIMESTAMP_BASE				= 2,
	AC_XPUCFG_ARRAY					= 3,
	AC_XPUCFG_ARRAY_SIZE_PTR			= 4,
	AC_XPUDBGAR_ARRAY				= 5,
	AC_XPUDBGAR_ARRAY_SIZE_PTR			= 6,
	AC_XPUINFOS_ARRAY				= 7,
	AC_XPUINFOS_ARRAY_SIZE_PTR			= 8,
	AC_IS_SILENTLOGGING_ENABLED			= 9,
	AC_XPUTCSRREGS_ARRAY				= 10,
	AC_XPUTCSRREGS_ARRAY_SIZE_PTR			= 11,
	AC_XPUTCSRSLE_ADDR				= 12,
	AC_XPU_SLE_DISABLE_VALUE			= 13,
	AC_XPU_SLE_ENABLE_VALUE				= 14,
	AC_VALIDDOMAIN_IDS_ARRAY			= 15,
	AC_VALIDDOMAIN_IDS_ARRAY_SIZE_PTR		= 16,
	AC_XPUHWADDR_MASK_ARRAY				= 17,
	AC_XPUHWADDR_MASK_ARRAY_SIZE_PTR		= 18,
	AC_XPUINTR_POS_XPUMAP_ARRAY			= 19,
	AC_XPUINTREN_REG_ARRAY				= 20,
	AC_XPUINTRSTATUS_REG_ARRAY			= 21,
	AC_XPUINTR_TCSRREG_ARRAY_SIZE_PTR		= 22,
	AC_XPUPROTECTEDRANGES_ARRAY			= 23,
	AC_XPUPROTECTEDRANGES_ARRAY_SIZE_PTR		= 24,
	AC_OEMSPAREFUSEINFO_ARRAY			= 25,
	AC_OEMSPAREFUSEINFO_ARRAY_SIZE_PTR		= 26,
	AC_OEMSPAREFUSEINFO_COUNT			= 27,
	AC_XPUINT_NUMBER				= 28,
	AC_RAXPU_INFO_ARRAY				= 29,
	AC_RAXPU_INFO_ARRAY_SIZE_PTR			= 30,
	AC_MS_XPUPROTECTEDRANGES_ARRAY			= 31,
	AC_MS_XPUPROTECTEDRANGES_ARRAY_SIZE_PTR		= 32,
	AC_APURPU_PARTITIONS_ARRAY			= 33,
	AC_APURPU_PARTITIONS_ARRAY_SIZE_PTR		= 34,
	AC_VMIDMT_CFG_ARRAY				= 35,
	AC_VMIDMT_CFG_ARRAY_SIZE_PTR			= 36,
	AC_VMIDMT_MAP_ARRAY				= 37,
	AC_VMIDMT_MAP_ARRAY_SIZE_PTR			= 38,
	AC_VMIDMT_INTR_POS_MAP_ARRAY			= 39,
	AC_VMIDMT_INTR_POS_MAP_ARRAY_SIZE_PTR		= 40,
	AC_VMIDMT_DEVICE_VMIDMT_MAP_ARRAY		= 41,
	AC_XPU_VALID_QAD_MASK				= 42,
	AC_XPU_DDR_SLICE_XPU_GROUP			= 43,
	AC_XPU_DDR_SLICE_XPU_DETAILS			= 44,
	AC_XPU_DDR_SLICE_GROUP_COUNT			= 45,
	AC_MAX_XPU_ENUM					= 46,
	AC_DDR_SLICE_COUNT				= 47,
	AC_TZ_VMID_CFG_ARRAY				= 48,
	AC_TZ_VMID_CFG_ARRAY_SIZE			= 49,
	AC_TZ_SHADOW_VM_ARRAY				= 50,
	AC_TZ_SHADOW_VM_ARRAY_SIZE			= 51,
	AC_TZ_DEVICE_MEM_MAP_ARRAY			= 52,
	AC_TZ_DEVICE_MEM_MAP_ARRAY_SIZE			= 53,
	AC_INTR_DESC_ARRAY				= 54,
	AC_INTR_DESC_ARRAY_SIZE				= 55,
	AC_ALLOC_FROM_DDR_OFFSET			= 56,
	AC_XPU_WA_SUPPORTED				= 57,
	AC_IMAGE_VERSION_INFO				= 58,
	AC_TARGET_PROFILE_MAP_ARRAY			= 59,
	AC_TARGET_PROFILE_MAP_COUNT			= 60,
	AC_QB_RESTORE_XPU_ARRAY				= 61,
	AC_QB_RESTORE_XPU_COUNT				= 62,
	AC_QB_XPU_ARRAY					= 63,
	AC_QB_XPU_ARRAY_SIZE				= 64,
	AC_QB_VMIDMT_ARRAY				= 65,
	AC_QB_VMIDMT_COUNT				= 66,
	AC_QB_VMIDMT_ARRAY_ENTRY_COUNT			= 67,
	AC_QB_XPU_META_INFO				= 68,
	AC_XPU_CFG_MAX
} AC_EE_XPU_TARGET_CFG_t;

typedef struct {
	uint32_t u_magic_cookie;
	uint32_t u_version;
	uint32_t u_num_entries;
	uintptr_t *p_global_data_ptr;
} ac_global_data_t;

/* QAD Vector format for AP-S, TME-FW and TME-ROM */
#define DEVPRG_DEFAULT_PERM		(APP_SEC_QAD_BIT | TME_FW_QAD_BIT | \
					 TME_ROM_QAD_BIT)
/* QAD Vector format for AP-NS */
#define AC_PERM_APNS_BIT		APP_NSEC_QAD_BIT

/* Profiles */
#define AC_PROFILE_DEFAULT			(1U << 0)
#define AC_PROFILE_HW_V1			(1U << 1)
#define AC_PROFILE_HW_V2			(1U << 2)
#define AC_PROFILE_RAMDUMP_ENABLED		(1U << 3)
#define AC_PROFILE_DEBUG_ENABLED		(1U << 4)
#define AC_PROFILE_IGNORE_RG			(1U << 5)
#define AC_PROFILE_VMID_CHECK_DISABLED		(1U << 6)
#define AC_PROFILE_RG_OVERRIDE			(1U << 7)
#define AC_PROFILE_XPRESSCFG			(1U << 8)
#define AC_PROFILE_XBL_SEC			(1U << 9)
#define AC_PROFILE_DYNAMIC_INITIALIZE		(1U << 10)
#define AC_PROFILE_PLATFORM_AUTO_IVI		(1U << 11)
#define AC_PROFILE_PLATFORM_AUTO_ADAS		(1U << 12)
#define AC_PROFILE_PLATFORM_COMPUTE		(1U << 13)
#define AC_PROFILE_WP				(1U << 14)

/* HOYA targets only support bits 0-15 and must be placed above this comment */
#define AC_PROFILE_DEBUG_ENABLED_APPS_SECURE	(1U << 15)
#define AC_PROFILE_LCS_INTERNAL			(1U << 16)
#define AC_PROFILE_WA_PRE_ES			(1U << 17)
#define AC_PROFILE_DEBUG_BUSPM_ENABLE		(1U << 18)
#define AC_PROFILE_PLATFORM_LE			(1U << 19)
#define AC_PROFILE_PLATFORM_MN			(1U << 20)

/* Milestones */
#define AC_PROFILE_DEBUG_SDI_PASS2		(1U << 21)
#define AC_PROFILE_DEBUG_POLICY_READY		(1U << 22)
#define AC_PROFILE_xBL_SC_EXIT			(1U << 23)

/* For Binary compatible profiles */
#define AC_PROFILE_BIN_COMP_BASE		(1U << 24)
#define AC_PROFILE_BIN_COMP_1			(1U << 25)

#define AC_PROFILE_PLATFORM_RG_MN		(1U << 26)

/* For Auto specific profiles */
#define AC_PROFILE_PLATFORM_AUTO_FLEX		(1U << 27)
#define AC_PROFILE_PLATFORM_AUTO		(1U << 28)
#define AC_PROFILE_PLATFORM_CLOUD		(1U << 29)

/* When adding new profiles, please increment the number below */
#define AC_STATIC_POLICY_PROFILES_COUNT		(1U << 31)

/* maximum count configured to save updated DBGAR address */
#define DBGAR_MAX_COUNT			100

typedef struct {
	const uint64_t start;
	const uint64_t end;
	const HAL_xpu2_XPU2Type xpu_id;
} ac_xpu_protected_range;

typedef struct {
	uint32_t xpu_index;
	uint32_t mask_value;
	uint32_t base_addr;
} tzbsp_xpu_hwaddr_mask;

typedef struct {
	uint32_t xpu_id;
	uint32_t mask;
	uint32_t offset;
} ac_xpu_hw_addr_mask_offset;

typedef struct {
	uint32_t addr;
	uint32_t mask;
	uint32_t value;
} ac_addr_value_tuple;

typedef struct {
	char *intr_desc;
	ac_addr_value_tuple *static_signal_silent_logging;
	ac_addr_value_tuple *static_signal_others;
	uint32_t intr_num;
	uint32_t xpu_id;
	uint32_t static_signal_others_count;
} ac_ra_xpu_info;

extern const ac_xpu_protected_range g_ac_xpu_protected_ranges[];
extern const uint32_t g_ac_xpu_protected_ranges_count;

extern ac_xpu4_priv_info g_ac_xpu_infos[];
extern const uint32_t g_ac_xpu_infos_count;

extern const uint32_t g_ac_xpu_dbgars[];
extern const uint32_t g_ac_xpu_dbgars_count;

extern const tzbsp_xpu_hwaddr_mask g_tzbsp_xpu_addr_mask[];
extern const uint32_t g_tzbsp_xpu_addrmask_cnt;

extern const ac_xpu_hw_addr_mask_offset g_ac_xpu_hw_addr_mask_offset[];
extern const uint32_t g_ac_xpu_hw_addr_mask_offset_count;

extern const ac_xpu_cfg g_ac_xpu_cfg[];
extern const uint32_t g_ac_xpu_cfg_count;

extern const ac_ra_xpu_info g_ac_ra_xpu_info[];
extern const uint32_t g_ac_ra_xpu_info_count;

/*
 * For bypassing access control Turn ON this flag - APNSPE not enabled,
 * static and dynamic configuration skipped
 */
extern const uint32_t g_ac_enabled;
extern const ac_global_data_t g_ac_global_data_legacy_config;

extern ac_global_data_t *p_ac_global_data;

/*
 * This Magic cookie is used for Legacy config targets where policy is compiled
 * as part of the image
 */
#define AC_MAGIC_COOKIE_LEGACY_CONFIG		0x76716789

/* errorFatal if AC config is not loaded and/or Magic Cookie is not correct. */
static inline void error_if_ac_config_fail(void)
{
	if ((!p_ac_global_data) ||
	    (AC_MAGIC_COOKIE_LEGACY_CONFIG !=
	     p_ac_global_data->u_magic_cookie))
		ERROR("AC_XPU ErrCode: %d, AC config magic cookie mismatch",
		      AC_ERR_AC_MAGIC_COOKIE_MISMATCH);
}

static inline bool is_ac_disabled(void)
{
#ifdef XPU_DISABLE
	return true;
#else
	error_if_ac_config_fail();
	return false;
#endif
}

/*
 * Based on milestone in which API is invoked fills up the
 * xPU policy configuration data to be applied
 *
 * @param milestone		Milestone for which xPU configuration data has
 *				to be picked
 * @param xpu_cfg		Pointer to hold the xPU configuration data array
 * @param xpu_cfg_count		Number of xPU configuration record placed
 * @param xpu_dbg_cfg		Pointer to hold the xPU debug configuration
 *				data array
 * @param xpu_dbg_cfg_count	Number of xPU configuration record placed in
 *				debug config array
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_get_xpu_config_for_milestone(ac_milestone milestone,
					      ac_xpu_cfg **xpu_cfg,
					      uint32_t *xpu_cfg_count,
					      uint32_t **xpu_dbg_cfg,
					      uint32_t *xpu_dbg_cfg_count);

/*
 * Returns TRUE based on profile flags to be enabled on target
 * This operates on RG profiles
 *
 * @param milestone		Milestone call in which API is invoked
 * @param profile_flags		Profile flag value defined in xPU static policy
 *
 * @return TRUE if that policy has to be enabled on target for that milestone
 *         else FALSE
 */
bool ac_is_rg_profile_matched(ac_milestone milestone, uint32_t profile_flags);

/*
 * Returns TRUE based on profile flags to be enabled on target
 * Say if DEBUG profile is passed and on secure device it returns FALSE
 *
 * @param milestone		Milestone call in which API is invoked
 * @param profile_flags		Profile flag value defined in xPU static policy
 *
 * @return TRUE if that xPU has to be enabled on target for that milestone
 *         else FALSE
 */
bool ac_is_xpu_profile_matched(ac_milestone milestone, uint32_t profile_flags);

/*
 * Returns Pointers to the Protected ranges Info arrays
 *
 * @param pac_xpu_protected_ranges	Double Pointer where protected range
 *					info is updated
 * @param pac_xpu_protected_ranges_count Pointer to get the number of entries
 */
void ac_get_dynamic_protect_ranges(const ac_xpu_protected_range **pac_xpu_protected_ranges,
				   uint32_t *pac_xpu_protected_ranges_count);

/*
 * Returns Pointers to the RAXPU Info arrays
 *
 * @param pac_ra_xpu_info	Double Pointer where RAXPU info is updated
 * @param pac_ra_xpu_info_count	Pointer to get the number of entries
 */
void ac_get_ra_xpu_info(const ac_ra_xpu_info **pac_ra_xpu_info,
			uint32_t *pac_ra_xpu_info_count);

/*
 * Returns Pointers to the Global XPU Infos arrays
 *
 * @param pac_xpu_infos		Double Pointer where Global XPU info is updated
 * @param pac_xpu_infos_count	Pointer to get the number of entries
 */
void ac_xpu_get_xpu_infos(ac_xpu4_priv_info **pac_xpu_infos,
			  uint32_t *pac_xpu_infos_count);

/*
 * Returns Pointers to the XPU HW Addr/Mask Info arrays
 *
 * @param pac_xpu_hw_addr_mask_offset	Double Pointer where XPU HW Addr/Mask
 *					info is updated
 * @param pac_xpu_hw_addr_mask_offset_count Pointer to get the number of entries
 */
void ac_xpu_get_addr_offset_mask_info(const tzbsp_xpu_hwaddr_mask **pac_xpu_hw_addr_mask_offset,
				      uint32_t *pac_xpu_hw_addr_mask_offset_count);

/*
 * Returns Pointers to the AC XPU CFG Info arrays
 *
 * @param pac_xp_cfg		Double Pointer where AC XPU CFG info is updated
 * @param pac_xp_cfg_count	Pointer to get the number of entries
 */
void ac_xpu_get_xpu_cfg(const ac_xpu_cfg **pac_xp_cfg,
			uint32_t *pac_xp_cfg_count);

/*
 * Based on milestone in which API is invoked fills up the
 * xPU policy configuration data to be applied
 *
 * @param xpu_id		xPU ID of xpu details needed
 * @param p_out_xpu_cfg		Pointer to hold the xPU configuration data array
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_get_dynamic_xpu_config_from_xpu_id(uint32_t xpu_id,
						    ac_xpu_cfg **p_out_xpu_cfg);

/*
 * Returns TRUE if xPU has to be disabled on target
 *
 * @param u_xpu_addr	SOC xPU address which is disabled on target due to
 *			HW issues
 *
 * @return TRUE if xPU has to be disabled on bootup else FALSE
 */
extern bool ac_is_target_specific_xpu_disable(uintptr_t u_xpu_addr);

#endif /* ACCESS_CONTROL_TARGET_H */
