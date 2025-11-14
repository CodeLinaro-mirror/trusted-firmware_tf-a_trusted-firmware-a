/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*---------------------THIS FILE IS AUTO GENERATED---------------------*/


#include <stdint.h>
#include <stddef.h>
#include "hal_xpu4_target.h"
#include "ac_xpu.h"
#include "ac_xpu_target_info.h"


const ac_register_tuple g_ac_ra_xpu_intr_status_reg[RA_XPU_ERR_INT_REG_NUM] = {
};

const ac_register_tuple g_ac_ra_xpu_intr_enable_reg[RA_XPU_ERR_INT_REG_NUM] = {
};

const uint32_t g_ac_xpu_dbgars[] = {
};
const uint32_t g_ac_xpu_dbgars_count = ARRAY_SIZE(g_ac_xpu_dbgars);

static uint32_t s_shadow_read_perm_MACHX_LLCC_MPU[10];
static uint32_t s_shadow_write_perm_MACHX_LLCC_MPU[10];
static const ac_xpu_dynamic_rgs s_ac_xpu_dynamic_rgs_MACHX_LLCC_MPU = {
	.rg_start = 30,
	.rg_count = 10,
	.shadow_read_perm = s_shadow_read_perm_MACHX_LLCC_MPU,
	.shadow_write_perm = s_shadow_write_perm_MACHX_LLCC_MPU,
};

ac_xpu4_priv_info g_ac_xpu_infos[] = {
	{
		.soc_addr = 0x003fe000,
		.addr = 0x003fe000,
		.xpu_id = HAL_XPU2_BOOT_ROM,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00566000,
		.addr = 0x00566000,
		.xpu_id = HAL_XPU2_CNOC_CMN_PLL_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x0056c000,
		.addr = 0x0056c000,
		.xpu_id = HAL_XPU2_CNOC_LPASS_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00560000,
		.addr = 0x00560000,
		.xpu_id = HAL_XPU2_CNOC_NOC_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x0056e000,
		.addr = 0x0056e000,
		.xpu_id = HAL_XPU2_CNOC_NSSNOC_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00562000,
		.addr = 0x00562000,
		.xpu_id = HAL_XPU2_CNOC_PCIE_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00564000,
		.addr = 0x00564000,
		.xpu_id = HAL_XPU2_CNOC_PROCESSOR_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00568000,
		.addr = 0x00568000,
		.xpu_id = HAL_XPU2_CNOC_QDSS_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00572000,
		.addr = 0x00572000,
		.xpu_id = HAL_XPU2_CNOC_SDCC_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x0057e000,
		.addr = 0x0057e000,
		.xpu_id = HAL_XPU2_CNOC_TCU_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00570000,
		.addr = 0x00570000,
		.xpu_id = HAL_XPU2_CNOC_USBCTL_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x0057a000,
		.addr = 0x0057a000,
		.xpu_id = HAL_XPU2_CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x01880000,
		.addr = 0x01880000,
		.xpu_id = HAL_XPU2_GCC_RPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x042e0000,
		.addr = 0x042e0000,
		.xpu_id = HAL_XPU2_GEMNOC_CFG_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x04060000,
		.addr = 0x04060000,
		.xpu_id = HAL_XPU2_MACHX_LLCC_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = &s_ac_xpu_dynamic_rgs_MACHX_LLCC_MPU,
	},
	{
		.soc_addr = 0x040b0000,
		.addr = 0x040b0000,
		.xpu_id = HAL_XPU2_MACHX_LLCC_SCID_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x004ac000,
		.addr = 0x004ac000,
		.xpu_id = HAL_XPU2_MPM2_MPM_APU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00052000,
		.addr = 0x00052000,
		.xpu_id = HAL_XPU2_IMEM_APU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00054000,
		.addr = 0x00054000,
		.xpu_id = HAL_XPU2_IMEM_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x060e0000,
		.addr = 0x060e0000,
		.xpu_id = HAL_XPU2_QDSS_CFG_DCC_DCC,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00060000,
		.addr = 0x00060000,
		.xpu_id = HAL_XPU2_QMIP_XPU_CFG,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x079b1000,
		.addr = 0x079b1000,
		.xpu_id = HAL_XPU2_QPIC_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x07981000,
		.addr = 0x07981000,
		.xpu_id = HAL_XPU2_QPIC_APU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x0780c000,
		.addr = 0x0780c000,
		.xpu_id = HAL_XPU2_SDC1_SDCC_ICE,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x000ae000,
		.addr = 0x000ae000,
		.xpu_id = HAL_XPU2_SEC_CTRL_APU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x01984000,
		.addr = 0x01984000,
		.xpu_id = HAL_XPU2_TCSR_REGS,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x01300000,
		.addr = 0x01300000,
		.xpu_id = HAL_XPU2_TLMM,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x01a00000,
		.addr = 0x01a00000,
		.xpu_id = HAL_XPU2_QUPV3_5_GSI_TOP,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00620000,
		.addr = 0x00620000,
		.xpu_id = HAL_XPU2_XS_PCIE_1LANE_0_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00622000,
		.addr = 0x00622000,
		.xpu_id = HAL_XPU2_XS_PCIE_1LANE_1_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00624000,
		.addr = 0x00624000,
		.xpu_id = HAL_XPU2_XS_PCIE_2LANE_0_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00626000,
		.addr = 0x00626000,
		.xpu_id = HAL_XPU2_XS_PCIE_2LANE_1_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
	{
		.soc_addr = 0x00628000,
		.addr = 0x00628000,
		.xpu_id = HAL_XPU2_XS_PCIE_2LANE_2_MPU,
		.idr = {0},
		.rev = 0,
		.dyn_rgs = NULL,
	},
};
const uint32_t g_ac_xpu_infos_count = ARRAY_SIZE(g_ac_xpu_infos);

/*MD5:f515649ebdcdb1db8f301b875c6d9663*/
