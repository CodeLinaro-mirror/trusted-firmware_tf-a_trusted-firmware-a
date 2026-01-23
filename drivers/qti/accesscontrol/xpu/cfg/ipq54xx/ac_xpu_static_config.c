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



#define QAD_APPS	(0)
#define QAD_TME_ROM	(1)
#define QAD_TME_FW	(2)
#define QAD_DEBUG	(3)
#define QAD_AP_QC_BL	(4)
#define QAD_MSA		(5)
#define QAD_PRIME	(6)

#define AC_AP_NS_QAD_VECTOR_OFFSET	(30)
#define AC_AP_SEC_QAD_VECTOR_OFFSET	(31)

#define AC_DOMAIN_NONE			(0)
#define AC_DOMAIN_AP_NS_BIT		((1U << QAD_APPS) | (0x1U << AC_AP_NS_QAD_VECTOR_OFFSET))
#define AC_DOMAIN_APPS_SEC_BIT	((1U << QAD_APPS) | (0x1U << AC_AP_SEC_QAD_VECTOR_OFFSET))
#define AC_DOMAIN_TME_ROM_BIT	(1U << QAD_TME_ROM)
#define AC_DOMAIN_TME_FW_BIT	(1U << QAD_TME_FW)
#define AC_DOMAIN_DEBUG_BIT		(1U << QAD_DEBUG)
#define AC_DOMAIN_AP_QC_BL_BIT		(1U << QAD_AP_QC_BL)
#define AC_DOMAIN_MSA_BIT		(1U << QAD_MSA)
#define AC_DOMAIN_PRIME_BIT		(1U << QAD_PRIME)
#define AC_DOMAIN_RESERVED_7_BIT	(1U << 7)
#define AC_DOMAIN_RESERVED_8_BIT	(1U << 8)
#define AC_DOMAIN_RESERVED_9_BIT	(1U << 9)
#define AC_DOMAIN_RESERVED_10_BIT	(1U << 10)
#define AC_DOMAIN_RESERVED_11_BIT	(1U << 11)
/* BOOT_ROM_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg boot_rom_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00300000,
		.end          = 0x00328000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
};

/* CNOC_CE_EDPD_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_ce_edpd_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00200000,
		.end          = 0x0021c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x0021c000,
		.end          = 0x00300000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
};

/* CNOC_APSS_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_apss_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x0f100000,
		.end          = 0x0f410000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x0f411000,
		.end          = 0x0f422000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x0f600000,
		.end          = 0x0f800000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x0f800000,
		.end          = 0x0fe00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* CNOC_CMN_PLL_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_cmn_pll_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x07a00000,
		.end          = 0x07a21000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x0009b000,
		.end          = 0x0009c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x00090000,
		.end          = 0x00091000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_LPASS_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_lpass_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x0a000000,
		.end          = 0x0b000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_NOC_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_noc_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00001000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00001000,
		.end          = 0x00080000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x00600000,
		.end          = 0x00601000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x00601000,
		.end          = 0x00610000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x00611000,
		.end          = 0x00640000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x00640000,
		.end          = 0x00650000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x00580000,
		.end          = 0x00581000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x00581000,
		.end          = 0x00587000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.start        = 0x00588000,
		.end          = 0x00600000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 11,
		.start        = 0x01d00000,
		.end          = 0x01d85000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 13,
		.start        = 0x01d86000,
		.end          = 0x01d90000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 14,
		.start        = 0x01d90000,
		.end          = 0x01da0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* CNOC_NSSNOC_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_nssnoc_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x38000000,
		.end          = 0x3ab01000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x3ab01000,
		.end          = 0x3ab21000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x3ab21000,
		.end          = 0x3ab39000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x3ab39000,
		.end          = 0x3ab51000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x3ab51000,
		.end          = 0x40000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00080000,
		.end          = 0x00088000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00088000,
		.end          = 0x00090000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x000f0000,
		.end          = 0x000f8000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x000f8000,
		.end          = 0x00100000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_SLAVE_1LANE_0_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_slave_1lane_0_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x70000000,
		.end          = 0x70001000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x70001000,
		.end          = 0x70001100,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x70001100,
		.end          = 0x70001200,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x70001200,
		.end          = 0x70001300,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x70001300,
		.end          = 0x70001400,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x70001400,
		.end          = 0x70001500,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x70001500,
		.end          = 0x70001600,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x70001600,
		.end          = 0x70001700,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x70001700,
		.end          = 0x70001800,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.start        = 0x70001800,
		.end          = 0x70001900,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.start        = 0x70001900,
		.end          = 0x70001a00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 11,
		.start        = 0x70001a00,
		.end          = 0x70001b00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.start        = 0x70001b00,
		.end          = 0x70001c00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 13,
		.start        = 0x70001c00,
		.end          = 0x70001d00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 14,
		.start        = 0x70001d00,
		.end          = 0x70001e00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 15,
		.start        = 0x70001e00,
		.end          = 0x70001f00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 16,
		.start        = 0x70001f00,
		.end          = 0x70002000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 17,
		.start        = 0x70002000,
		.end          = 0x72000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 18,
		.start        = 0x72000000,
		.end          = 0x80000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_SLAVE_1LANE_1_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_slave_1lane_1_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x60000000,
		.end          = 0x60001000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x60001000,
		.end          = 0x60001100,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x60001100,
		.end          = 0x60001200,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x60001200,
		.end          = 0x60001300,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x60001300,
		.end          = 0x60001400,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x60001400,
		.end          = 0x60001500,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x60001500,
		.end          = 0x60001600,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x60001600,
		.end          = 0x60001700,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x60001700,
		.end          = 0x60001800,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.start        = 0x60001800,
		.end          = 0x60001900,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.start        = 0x60001900,
		.end          = 0x60001a00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 11,
		.start        = 0x60001a00,
		.end          = 0x60001b00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.start        = 0x60001b00,
		.end          = 0x60001c00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 13,
		.start        = 0x60001c00,
		.end          = 0x60001d00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 14,
		.start        = 0x60001d00,
		.end          = 0x60001e00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 15,
		.start        = 0x60001e00,
		.end          = 0x60001f00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 16,
		.start        = 0x60001f00,
		.end          = 0x60002000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 17,
		.start        = 0x60002000,
		.end          = 0x62000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 18,
		.start        = 0x62000000,
		.end          = 0x70000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_SLAVE_2LANE_0_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_slave_2lane_0_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x50000000,
		.end          = 0x50001000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x50001000,
		.end          = 0x50001100,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x50001100,
		.end          = 0x50001200,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x50001200,
		.end          = 0x50001300,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x50001300,
		.end          = 0x50001400,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x50001400,
		.end          = 0x50001500,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x50001500,
		.end          = 0x50001600,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x50001600,
		.end          = 0x50001700,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x50001700,
		.end          = 0x50001800,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.start        = 0x50001800,
		.end          = 0x50001900,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.start        = 0x50001900,
		.end          = 0x50001a00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 11,
		.start        = 0x50001a00,
		.end          = 0x50001b00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.start        = 0x50001b00,
		.end          = 0x50001c00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 13,
		.start        = 0x50001c00,
		.end          = 0x50001d00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 14,
		.start        = 0x50001d00,
		.end          = 0x50001e00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 15,
		.start        = 0x50001e00,
		.end          = 0x50001f00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 16,
		.start        = 0x50001f00,
		.end          = 0x50002000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 17,
		.start        = 0x50002000,
		.end          = 0x52000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 18,
		.start        = 0x52000000,
		.end          = 0x60000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_SLAVE_2LANE_1_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_slave_2lane_1_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x40000000,
		.end          = 0x40001000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x40001000,
		.end          = 0x40001100,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x40001100,
		.end          = 0x40001200,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x40001200,
		.end          = 0x40001300,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x40001300,
		.end          = 0x40001400,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x40001400,
		.end          = 0x40001500,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x40001500,
		.end          = 0x40001600,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x40001600,
		.end          = 0x40001700,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x40001700,
		.end          = 0x40001800,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.start        = 0x40001800,
		.end          = 0x40001900,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.start        = 0x40001900,
		.end          = 0x40001a00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 11,
		.start        = 0x40001a00,
		.end          = 0x40001b00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.start        = 0x40001b00,
		.end          = 0x40001c00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 13,
		.start        = 0x40001c00,
		.end          = 0x40001d00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 14,
		.start        = 0x40001d00,
		.end          = 0x40001e00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 15,
		.start        = 0x40001e00,
		.end          = 0x40001f00,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 16,
		.start        = 0x40001f00,
		.end          = 0x40002000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 17,
		.start        = 0x40002000,
		.end          = 0x42000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 18,
		.start        = 0x42000000,
		.end          = 0x50000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PMU_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pmu_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x0009a000,
		.end          = 0x0009b000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* CNOC_QDSS_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_qdss_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x06000000,
		.end          = 0x07000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
};

/* CNOC_WCSS_SLV_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_wcss_slv_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x0c100000,
		.end          = 0x0c140000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x0c140000,
		.end          = 0x0c300000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x0c300000,
		.end          = 0x0c900000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
};

/* CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_usbphy_usb2_core_phy_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00070000,
		.end          = 0x00080000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x01e00000,
		.end          = 0x02000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* DDR_PHY_XPU_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg ddr_phy_xpu_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x02000000,
		.end          = 0x04010000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* GCC_RPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg gcc_rpu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 14,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 17,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 18,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 19,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 23,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 25,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 28,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 32,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 33,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 34,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 36,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 39,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 40,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 41,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 42,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 43,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 44,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 45,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 46,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 48,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 49,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 50,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 51,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 52,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 53,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 54,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 55,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 56,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 57,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 58,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 60,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 61,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 62,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 63,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 64,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 65,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 66,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 67,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 68,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 69,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 70,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 71,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 73,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* GEMNOC_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg gemnoc_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00800000,
		.end          = 0x00801000,
		.profile_flags = AC_PROFILE_xBL_SC_EXIT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{//need to remove
		.rg_num        = 1,
		.start        = 0x00830000,
		.end          = 0x00834000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x00841000,
		.end          = 0x00842000,
		.profile_flags = AC_PROFILE_xBL_SC_EXIT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x00880000,
		.end          = 0x008a0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x008a0000,
		.end          = 0x00a82000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x00ac0000,
		.end          = 0x00acd000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x00ace000,
		.end          = 0x00acf000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x00acf000,
		.end          = 0x00b00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* LLCC_LLCC_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg llcc_llcc_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x8a600000,
		.end          = 0x8a680000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
		{
		.rg_num        = 2,
		.start        = 0x89E00000,
		.end          = 0x8A080000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* LLCC_LLCC_SCID_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg llcc_llcc_scid_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00000001,
		.end          = 0x00000004,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x00000004,
		.end          = 0x00000005,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x00000005,
		.end          = 0x00000020,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* TLMM_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg tlmm_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x01000000,
		.end          = 0x01400000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
};

/* WCSS_XPU_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg wcss_xpu_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x0c000000,
		.end          = 0x0c100000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x0c900000,
		.end          = 0x0ca00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x0ca00000,
		.end          = 0x0ca4d000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x0ca4d000,
		.end          = 0x0cb50000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x0cb50000,
		.end          = 0x0cb51000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x0cb51000,
		.end          = 0x0cc00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x0cc00000,
		.end          = 0x0d000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 7,
		.start        = 0x0d000000,
		.end          = 0x0d100000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 8,
		.start        = 0x0d100000,
		.end          = 0x0d180000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT | AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT | AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.start        = 0x0d180000,
		.end          = 0x0d200000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
};

/* MPM2_MPM_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg mpm2_mpm_apu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* OCIMEM_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg ocimem_apu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* OCIMEM_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg ocimem_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x08600000,
		.end          = 0x08601000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x08601000,
		.end          = 0x08604000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x08604000,
		.end          = 0x08605000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x08605000,
		.end          = 0x08608000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x08608000,
		.end          = 0x0860e000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 5,
		.start        = 0x0860e000,
		.end          = 0x0860f000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 6,
		.start        = 0x0860f000,
		.end          = 0x0861c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* PRNG_RNG_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg prng_rng_apu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 14,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 15,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
};

/* Q6_CFG_XPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg q6_cfg_xpu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_MSA_BIT
	},
};

/* QDSS_CFG_DCC_DCC_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg qdss_cfg_dcc_dcc_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 14,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 15,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* QPIC_QPIC_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg qpic_qpic_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* QPIC_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg qpic_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 14,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 15,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 16,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 17,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 18,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 19,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 20,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 21,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 22,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* SDC1_SDCC_ICE_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg sdc1_sdcc_ice_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* SDCC_XPU_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg sdcc_xpu_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* SEC_CTRL_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg sec_ctrl_apu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
};

/* TCSR_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg tcsr_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 16,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 20,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 28,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
	{
		.rg_num        = 29,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT
	},
};

/* USB_XPU_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg usb_xpu_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 14,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 15,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 16,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 17,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 18,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 19,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 20,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 21,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 22,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_MSA_BIT
	},
	{
		.rg_num        = 23,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 24,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 25,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 26,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 27,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 28,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 29,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 30,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 31,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 32,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 33,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 34,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

const ac_xpu_cfg g_ac_xpu_cfg[] = {
	{	/* BOOT_ROM_XPU4 */
		.base_addr     = 0x003fe000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_BOOT_ROM,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(boot_rom_xpu4),
		.rg           = {boot_rom_xpu4},
	},
	{	/* CNOC_APSS_CFG_MPU_XPU4 */
		.base_addr     = 0x0057a000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_APSS_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_apss_cfg_mpu_xpu4),
		.rg           = {cnoc_apss_cfg_mpu_xpu4},
	},
	{	/* CNOC_CE_EDPD_MPU_XPU4 */
		.base_addr     = 0x00564000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_CE_EDPD_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_ce_edpd_mpu_xpu4),
		.rg           = {cnoc_ce_edpd_mpu_xpu4},
	},
	{	/* CNOC_CMN_PLL_CFG_MPU_XPU4 */
		.base_addr     = 0x00566000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_CMN_PLL_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_cmn_pll_cfg_mpu_xpu4),
		.rg           = {cnoc_cmn_pll_cfg_mpu_xpu4},
	},
	{	/* CNOC_LPASS_CFG_MPU_XPU4 */
		.base_addr     = 0x0056c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_LPASS_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_lpass_cfg_mpu_xpu4),
		.rg           = {cnoc_lpass_cfg_mpu_xpu4},
	},
	{	/* CNOC_NOC_CFG_MPU_XPU4 */
		.base_addr     = 0x00560000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_NOC_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_noc_cfg_mpu_xpu4),
		.rg           = {cnoc_noc_cfg_mpu_xpu4},
	},
	{	/* CNOC_NSSNOC_MPU_XPU4 */
		.base_addr     = 0x0056e000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_NSSNOC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_nssnoc_mpu_xpu4),
		.rg           = {cnoc_nssnoc_mpu_xpu4},
	},
	{	/* CNOC_PCIE_CFG_MPU_XPU4 */
		.base_addr     = 0x00562000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_PCIE_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_cfg_mpu_xpu4),
		.rg           = {cnoc_pcie_cfg_mpu_xpu4},
	},
	{	/* CNOC_PCIE_SLAVE_1LANE_0_MPU_XPU4 */
		.base_addr     = 0x00570000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_1LANE_0_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_slave_1lane_0_mpu_xpu4),
		.rg           = {cnoc_pcie_slave_1lane_0_mpu_xpu4},
	},
	{	/* CNOC_PCIE_SLAVE_1LANE_1_MPU_XPU4 */
		.base_addr     = 0x00572000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_1LANE_1_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_slave_1lane_1_mpu_xpu4),
		.rg           = {cnoc_pcie_slave_1lane_1_mpu_xpu4},
	},
	{	/* CNOC_PCIE_SLAVE_2LANE_0_MPU_XPU4 */
		.base_addr     = 0x00574000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_2LANE_0_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_slave_2lane_0_mpu_xpu4),
		.rg           = {cnoc_pcie_slave_2lane_0_mpu_xpu4},
	},
	{	/* CNOC_PCIE_SLAVE_2LANE_1_MPU_XPU4 */
		.base_addr     = 0x00576000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_2LANE_1_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_slave_2lane_1_mpu_xpu4),
		.rg           = {cnoc_pcie_slave_2lane_1_mpu_xpu4},
	},
	{	/* CNOC_PMU_CFG_MPU_XPU4 */
		.base_addr     = 0x0056a000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_PMU_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pmu_cfg_mpu_xpu4),
		.rg           = {cnoc_pmu_cfg_mpu_xpu4},
	},
	{	/* CNOC_QDSS_CFG_MPU_XPU4 */
		.base_addr     = 0x00568000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_QDSS_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_qdss_cfg_mpu_xpu4),
		.rg           = {cnoc_qdss_cfg_mpu_xpu4},
	},
	{	/* CNOC_WCSS_SLV_MPU_XPU4 */
		.base_addr     = 0x00578000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_WCSS_SLV_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_wcss_slv_mpu_xpu4),
		.rg           = {cnoc_wcss_slv_mpu_xpu4},
	},
	{	/* CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU_XPU4 */
		.base_addr     = 0x0057c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_usbphy_usb2_core_phy_cfg_mpu_xpu4),
		.rg           = {cnoc_usbphy_usb2_core_phy_cfg_mpu_xpu4},
	},
	{	/* DDR_PHY_XPU_CFG_XPU4 */
		.base_addr     = 0x0002c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_DDR_PHY_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(ddr_phy_xpu_cfg_xpu4),
		.rg           = {ddr_phy_xpu_cfg_xpu4},
	},
	{	/* GCC_RPU_XPU4 */
		.base_addr     = 0x01880000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_GCC_RPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(gcc_rpu_xpu4),
		.rg           = {gcc_rpu_xpu4},
	},
	{	/* GEMNOC_CFG_XPU4 */
		.base_addr     = 0x00a82000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_GEMNOC_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(gemnoc_cfg_xpu4),
		.rg           = {gemnoc_cfg_xpu4},
	},
	{	/* LLCC_LLCC_MPU_XPU4 */
		.base_addr     = 0x00830000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MACHX_LLCC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(llcc_llcc_mpu_xpu4),
		.rg           = {llcc_llcc_mpu_xpu4},
	},
	{	/* LLCC_LLCC_SCID_XPU4 */
		.base_addr     = 0x00880000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MACHX_LLCC_SCID_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(llcc_llcc_scid_xpu4),
		.rg           = {llcc_llcc_scid_xpu4},
	},
	{	/* TLMM_XPU4 */
		.base_addr     = 0x01300000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_TLMM,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(tlmm_xpu4),
		.rg           = {tlmm_xpu4},
	},
	{	/* WCSS_XPU_CFG_XPU4 */
		.base_addr     = 0x00028000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_WCSS_XPU_CFG,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(wcss_xpu_cfg_xpu4),
		.rg           = {wcss_xpu_cfg_xpu4},
	},
	{	/* MPM2_MPM_APU_XPU4 */
		.base_addr     = 0x004ac000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MPM2_MPM_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(mpm2_mpm_apu_xpu4),
		.rg           = {mpm2_mpm_apu_xpu4},
	},
	{	/* OCIMEM_APU_XPU4 */
		.base_addr     = 0x00052000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_IMEM_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(ocimem_apu_xpu4),
		.rg           = {ocimem_apu_xpu4},
	},
	{	/* OCIMEM_MPU_XPU4 */
		.base_addr     = 0x00054000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_IMEM_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(ocimem_mpu_xpu4),
		.rg           = {ocimem_mpu_xpu4},
	},
	{	/* PRNG_RNG_APU_XPU4 */
		.base_addr     = 0x004d0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_PRNG_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(prng_rng_apu_xpu4),
		.rg           = {prng_rng_apu_xpu4},
	},
	{	/* Q6_CFG_XPU_XPU4 */
		.base_addr     = 0x0002a000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_Q6_CFG_XPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(q6_cfg_xpu_xpu4),
		.rg           = {q6_cfg_xpu_xpu4},
	},
	{	/* QDSS_CFG_DCC_DCC_XPU4 */
		.base_addr     = 0x060e0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QDSS_CFG_DCC_DCC,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(qdss_cfg_dcc_dcc_xpu4),
		.rg           = {qdss_cfg_dcc_dcc_xpu4},
	},
	{	/* QPIC_QPIC_MPU_XPU4 */
		.base_addr     = 0x079b1000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QPIC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(qpic_qpic_mpu_xpu4),
		.rg           = {qpic_qpic_mpu_xpu4},
	},
	{	/* QPIC_XPU4 */
		.base_addr     = 0x07981000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QPIC_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(qpic_xpu4),
		.rg           = {qpic_xpu4},
	},
	{	/* SDC1_SDCC_ICE_XPU4 */
		.base_addr     = 0x0780c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_SDC1_SDCC_ICE,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(sdc1_sdcc_ice_xpu4),
		.rg           = {sdc1_sdcc_ice_xpu4},
	},
	{	/* SDCC_XPU_CFG_XPU4 */
		.base_addr     = 0x00026000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_SDCC_XPU_CFG,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(sdcc_xpu_cfg_xpu4),
		.rg           = {sdcc_xpu_cfg_xpu4},
	},
	{	/* SEC_CTRL_APU_XPU4 */
		.base_addr     = 0x000ae000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_SEC_CTRL_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(sec_ctrl_apu_xpu4),
		.rg           = {sec_ctrl_apu_xpu4},
	},
	{	/* TCSR_XPU4 */
		.base_addr     = 0x01934000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_TCSR_REGS,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(tcsr_xpu4),
		.rg           = {tcsr_xpu4},
	},
	{	/* USB_XPU_CFG_XPU4 */
		.base_addr     = 0x00024000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_USB_CFG_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(usb_xpu_cfg_xpu4),
		.rg           = {usb_xpu_cfg_xpu4},
	},
	{	/* XPU4 */
		.base_addr     = 0x01a00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QUPV3_5_GSI_TOP,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(xpu4),
		.rg           = {xpu4},
	},
};
const uint32_t g_ac_xpu_cfg_count = ARRAY_SIZE(g_ac_xpu_cfg);

/*MD5:3d47bf8503d68ee3106f95e42d4eb24c*/
