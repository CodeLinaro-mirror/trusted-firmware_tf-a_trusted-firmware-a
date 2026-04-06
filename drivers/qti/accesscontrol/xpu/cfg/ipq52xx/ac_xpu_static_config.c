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
#define QAD_AP_QC_BL	(4)	/* Reserved QAD in Hermosa */
#define QAD_MSA		(5)	/* Reserved QAD in Hermosa */
#define QAD_PRIME	(6)	/* Reserved QAD in Hermosa */

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


#define AC_DOMAIN_ALL_ROT_BITS  AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_DEBUG_BIT

/* BOOT_ROM_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg boot_rom_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00300000,
		.end          = 0x00400000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
};

/* CNOC_APSS_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_apss_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x0b000000,
		.end          = 0x0b188000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x0b188000,
		.end          = 0x0b19a000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x0b19a000,
		.end          = 0x0b200000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_CMN_PLL_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_cmn_pll_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00090000,
		.end          = 0x00098000,
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
		.start        = 0x07a00000,
		.end          = 0x07a30000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_GEPHY_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_gephy_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x01400000,
		.end          = 0x01600000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_NONE,
		.write_qads    = AC_DOMAIN_NONE
	},
};

/* CNOC_LPASS_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_lpass_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS,
	}
};

/* CNOC_NOC_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_noc_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00500000,
		.end          = 0x00502000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00580000,
		.end          = 0x00581000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x00586000,
		.end          = 0x00587000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x01d04000,
		.end          = 0x01d05000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x01d10000,
		.end          = 0x01d11000,
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
		.end          = 0x40000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_SLAVE_1LANE_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_slave_1lane_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x70000000,
		.end          = 0x80000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_SLAVE_2LANE_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_slave_2lane_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x50000000,
		.end          = 0x60000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* CNOC_PCIE_USBPHY_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pcie_usbphy_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00078000,
		.end          = 0x00080000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00080000,
		.end          = 0x00088000,
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
};

/* CNOC_PON_MAC_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_pon_mac_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x01c80000,
		.end          = 0x01d00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_NONE,
		.write_qads    = AC_DOMAIN_NONE
	},
};

/* CNOC_QDSS_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg cnoc_qdss_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS,
	}
};

/* DDR_PHY_XPU_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg ddr_phy_xpu_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x02000000,
		.end          = 0x02020000,
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
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 11,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 12,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 13,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 14,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 17,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 18,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 19,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 23,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 25,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 28,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 32,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 33,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 34,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 36,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 39,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 40,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 41,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 42,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 43,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 44,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 45,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 46,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 48,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 49,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 50,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 51,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 52,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 53,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 54,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 55,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 56,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 57,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 58,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
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
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 62,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 63,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 64,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 65,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 66,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 67,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 68,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 69,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 70,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 71,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 73,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* MEMNOC_CFG_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg memnoc_cfg_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00a80000,
		.end          = 0x00a83000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00a83000,
		.end          = 0x00a85000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x00a85000,
		.end          = 0x00a90000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x00a90000,
		.end          = 0x00a91000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x00a91000,
		.end          = 0x00b00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* MEMNOC_SCH_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg memnoc_sch_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x80000000,
		.end          = 0x000100000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 1,
		.start        = 0x000800000000,
		.end          = 0x000880000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 2,
		.start        = 0x87d00000,
		.end          = 0x87d80000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
		{
		.rg_num        = 3,
		.start        = 0x87d80000,
		.end          = 0x87e00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
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
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
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
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS
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
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x08605000,
		.end          = 0x08608000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 4,
		.start        = 0x08608000,
		.end          = 0x0860e000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
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
		.end          = 0x08618000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
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
};

/* QPIC_QPIC_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg qpic_qpic_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_ALL_ROT_BITS,
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

/* QRNG_RNG_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg qrng_rng_apu_xpu4[] = {
	{
		.rg_num        = 0,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
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
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
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
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 1,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 2,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 3,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 4,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 5,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 6,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 7,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 8,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 9,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 10,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_ALL_ROT_BITS,
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
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
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
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 25,
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
};

/* TLMM_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg tlmm_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x01000000,
		.end          = 0x01400000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
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
	{	/* CNOC_APSS_MPU_XPU4 */
		.base_addr     = 0x0057a000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_APSS_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_apss_mpu_xpu4),
		.rg           = {cnoc_apss_mpu_xpu4},
	},
	{	/* CNOC_CMN_PLL_CFG_MPU_XPU4 */
		.base_addr     = 0x00566000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_CMN_PLL_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_cmn_pll_cfg_mpu_xpu4),
		.rg           = {cnoc_cmn_pll_cfg_mpu_xpu4},
	},
	{	/* CNOC_GEPHY_MPU_XPU4 */
		.base_addr     = 0x0057c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_GEPHY_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_gephy_mpu_xpu4),
		.rg           = {cnoc_gephy_mpu_xpu4},
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
	{	/* CNOC_PCIE_SLAVE_1LANE_MPU_XPU4 */
		.base_addr     = 0x00574000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_1LANE_0_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_slave_1lane_mpu_xpu4),
		.rg           = {cnoc_pcie_slave_1lane_mpu_xpu4},
	},
	{	/* CNOC_PCIE_SLAVE_2LANE_MPU_XPU4 */
		.base_addr     = 0x00576000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_2LANE_0_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_slave_2lane_mpu_xpu4),
		.rg           = {cnoc_pcie_slave_2lane_mpu_xpu4},
	},
	{	/* CNOC_PCIE_USBPHY_CFG_MPU_XPU4 */
		.base_addr     = 0x00562000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_PCIE_USBPHY_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pcie_usbphy_cfg_mpu_xpu4),
		.rg           = {cnoc_pcie_usbphy_cfg_mpu_xpu4},
	},
	{	/* CNOC_PON_MAC_MPU_XPU4 */
		.base_addr     = 0x00570000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_PON_MAC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_pon_mac_mpu_xpu4),
		.rg           = {cnoc_pon_mac_mpu_xpu4},
	},
	{	/* CNOC_QDSS_CFG_MPU_XPU4 */
		.base_addr     = 0x00568000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_QDSS_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_qdss_cfg_mpu_xpu4),
		.rg           = {cnoc_qdss_cfg_mpu_xpu4},
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
	{	/* MEMNOC_CFG_MPU_XPU4 */
		.base_addr     = 0x00ac2000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MEMNOC_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(memnoc_cfg_mpu_xpu4),
		.rg           = {memnoc_cfg_mpu_xpu4},
	},
	{	/* MEMNOC_SCH_MPU_XPU4 */
		.base_addr     = 0x00ac0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MEMNOC_SCH_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(memnoc_sch_mpu_xpu4),
		.rg           = {memnoc_sch_mpu_xpu4},
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
	{	/* QRNG_RNG_APU_XPU4 */
		.base_addr     = 0x004d0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_PRNG_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(qrng_rng_apu_xpu4),
		.rg           = {qrng_rng_apu_xpu4},
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
	{	/* TLMM_XPU4 */
		.base_addr     = 0x01300000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_TLMM,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(tlmm_xpu4),
		.rg           = {tlmm_xpu4},
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

/*MD5:dcc468a42a855128a9612b2f5721565f*/
