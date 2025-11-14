/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * Target-specific XPU configuration
 */

#include <stdbool.h>
#include <stdio.h>

#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#include "hal_xpu4.h"

/* Type definitions */
typedef HAL_xpu2_XPU2Type ACHAL_xpu_Type;

/* XPU configuration base pointers array */
uintptr_t xpu_config_base_pointers[AC_XPU_CFG_MAX] = {
	[AC_IS_XPU_ENABLED]				= (uintptr_t)&g_ac_enabled,
	[AC_XPUCFG_ARRAY]				= (uintptr_t)&g_ac_xpu_cfg,
	[AC_XPUCFG_ARRAY_SIZE_PTR]			= (uintptr_t)&g_ac_xpu_cfg_count,
	[AC_XPUDBGAR_ARRAY]				= (uintptr_t)&g_ac_xpu_dbgars,
	[AC_XPUDBGAR_ARRAY_SIZE_PTR]			= (uintptr_t)&g_ac_xpu_dbgars_count,
	[AC_XPUINFOS_ARRAY]				= (uintptr_t)&g_ac_xpu_infos,
	[AC_XPUINFOS_ARRAY_SIZE_PTR]			= (uintptr_t)&g_ac_xpu_infos_count,
	[AC_XPUHWADDR_MASK_ARRAY]			= (uintptr_t)&g_ac_xpu_hw_addr_mask_offset,
	[AC_XPUHWADDR_MASK_ARRAY_SIZE_PTR] =
		(uintptr_t)&g_ac_xpu_hw_addr_mask_offset_count,
	[AC_XPUPROTECTEDRANGES_ARRAY]			= (uintptr_t)&g_ac_xpu_protected_ranges,
	[AC_XPUPROTECTEDRANGES_ARRAY_SIZE_PTR] =
		(uintptr_t)&g_ac_xpu_protected_ranges_count,
	[AC_RAXPU_INFO_ARRAY]				= (uintptr_t)&g_ac_ra_xpu_info,
	[AC_RAXPU_INFO_ARRAY_SIZE_PTR]			= (uintptr_t)&g_ac_ra_xpu_info_count,
};

/* Global configuration data structure for legacy config */
const ac_global_data_t g_ac_global_data_legacy_config = {
	.u_magic_cookie		= AC_MAGIC_COOKIE_LEGACY_CONFIG,
	.u_version		= 0x1,
	.u_num_entries		= AC_XPU_CFG_MAX,
	.p_global_data_ptr	= xpu_config_base_pointers
};
