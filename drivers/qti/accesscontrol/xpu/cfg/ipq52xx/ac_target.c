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


const uint32_t g_ac_enabled = true;

const ac_xpu_protected_range g_ac_xpu_protected_ranges[] = {
	{
		// DDR space - region 1
		.start = 0x80000000,
		.end   = 0x100000000,
		.xpu_id = HAL_XPU2_MEMNOC_SCH_MPU,
	},
	{
		// DDR space - region 2
		.start = 0x800000000,
		.end   = 0x880000000,
		.xpu_id = HAL_XPU2_MEMNOC_SCH_MPU,
	},
};
const uint32_t g_ac_xpu_protected_ranges_count = ARRAY_SIZE(g_ac_xpu_protected_ranges);

const ACXpuProtectedRangeMS g_ac_xpu_protected_ranges_ms[] = {
};
const uint32_t g_ac_xpu_protected_ranges_ms_count = ARRAY_SIZE(g_ac_xpu_protected_ranges_ms);

const ac_xpu_hw_addr_mask_offset g_ac_xpu_hw_addr_mask_offset[] = {
	{HAL_XPU2_BOOT_ROM,         0x3FFFFF  , 0x300000  },
	{HAL_XPU2_CNOC_APSS_MPU,    0xFFFFFFF , 0xB000000 },
	{HAL_XPU2_CNOC_PON_MAC_MPU, 0x01FFFFFF, 0x01C80000},
	{HAL_XPU2_MEMNOC_CFG_MPU,   0xFFFFFF  , 0xA80000  },
	{HAL_XPU2_MEMNOC_SCH_MPU,   0xFFFFFFFF, 0x80000000},
	{HAL_XPU2_IMEM_MPU,         0xFFFFFFFF, 0x8600000 },
	{HAL_XPU2_TLMM,             0x1FFFFFF , 0x1000000 },
};
const uint32_t g_ac_xpu_hw_addr_mask_offset_count = ARRAY_SIZE(g_ac_xpu_hw_addr_mask_offset);

const ac_ra_xpu_info g_ac_ra_xpu_info[] = {
};
const uint32_t g_ac_ra_xpu_info_count = ARRAY_SIZE(g_ac_ra_xpu_info);

const HAL_xpu2_XPU2Type g_ac_ra_xpu_err_pos_to_xpu_map[][RA_XPU_ERR_NUM_PER_REG] = {
};

ac_register_tuple g_ac_xpu_intr_en_reg[XPU_ERR_INT_REG_NUM] = {
	{
		.addr = 0x193A010,
		.mask = 0xFFFFFFFF,
	},
	{
		.addr = 0x193A014,
		.mask = 0x1FF,
	},
};

ac_register_tuple g_ac_xpu_intr_status_reg[XPU_ERR_INT_REG_NUM] = {
	{
		.addr = 0x193A000,
		.mask = 0xFFFFFFFF,
	},
	{
		.addr = 0x193A004,
		.mask = 0x1FF,
	},
};

const ac_xpu_err_mapping_t g_ac_xpu_err_pos_to_xpu_map[XPU_ERR_INT_REG_NUM][XPU_ERR_NUM_PER_REG] = {
	{
		{.xpu_id = HAL_XPU2_QPIC_APU, .name = "HAL_XPU2_QPIC_APU"}, //Bit 0
		{.xpu_id = HAL_XPU2_QPIC_MPU, .name = "HAL_XPU2_QPIC_MPU"}, //Bit 1
		{.xpu_id = HAL_XPU2_QUPV3_5_GSI_TOP, .name = "HAL_XPU2_QUPV3_5_GSI_TOP"}, //Bit 2
		{.xpu_id = HAL_XPU2_TCSR_REGS, .name = "HAL_XPU2_TCSR_REGS"}, //Bit 3
		{.xpu_id = HAL_XPU2_PRNG_APU, .name = "HAL_XPU2_PRNG_APU"}, //Bit 4
		{.xpu_id = HAL_XPU2_GCC_RPU, .name = "HAL_XPU2_GCC_RPU"}, //Bit 5
		{.xpu_id = HAL_XPU2_SDC1_SDCC_ICE, .name = "HAL_XPU2_SDC1_SDCC_ICE"}, //Bit 6
		{.xpu_id = HAL_XPU2_MEMNOC_CFG_MPU, .name = "HAL_XPU2_MEMNOC_CFG_MPU"}, //Bit 7
		{.xpu_id = HAL_XPU2_DDR_PHY_CFG_MPU, .name = "HAL_XPU2_DDR_PHY_CFG_MPU"}, //Bit 8
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 9
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 10
		{.xpu_id = HAL_XPU2_IMEM_APU, .name = "HAL_XPU2_IMEM_APU"}, //Bit 11
		{.xpu_id = HAL_XPU2_IMEM_MPU, .name = "HAL_XPU2_IMEM_MPU"}, //Bit 12
		{.xpu_id = HAL_XPU2_SEC_CTRL_APU, .name = "HAL_XPU2_SEC_CTRL_APU"}, //Bit 13
		{.xpu_id = HAL_XPU2_QDSS_CFG_DCC_DCC, .name = "HAL_XPU2_QDSS_CFG_DCC_DCC"}, //Bit 14
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 15
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 16
		{.xpu_id = HAL_XPU2_MEMNOC_SCH_MPU, .name = "HAL_XPU2_MEMNOC_SCH_MPU"}, //Bit 17
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 18
		{.xpu_id = HAL_XPU2_CNOC_SDCC_CFG_MPU, .name = "HAL_XPU2_CNOC_SDCC_CFG_MPU"}, //Bit 19
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 20
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 21
		{.xpu_id = HAL_XPU2_CNOC_LPASS_CFG_MPU, .name = "HAL_XPU2_CNOC_LPASS_CFG_MPU"}, //Bit 22
		{.xpu_id = HAL_XPU2_CNOC_NSSNOC_MPU, .name = "HAL_XPU2_CNOC_NSSNOC_MPU"}, //Bit 23
		{.xpu_id = HAL_XPU2_CNOC_PON_MAC_MPU, .name = "HAL_XPU2_CNOC_PON_MAC_MPU"}, //Bit 24
		{.xpu_id = HAL_XPU2_XS_PCIE_1LANE_0_MPU, .name = "HAL_XPU2_XS_PCIE_1LANE_0_MPU"}, //Bit 25
		{.xpu_id = HAL_XPU2_XS_PCIE_2LANE_0_MPU, .name = "HAL_XPU2_XS_PCIE_2LANE_0_MPU"}, //Bit 26
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 27
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 28
		{.xpu_id = HAL_XPU2_MPM2_MPM_APU, .name = "HAL_XPU2_MPM2_MPM_APU"}, //Bit 29
		{.xpu_id = HAL_XPU2_CNOC_GEPHY_MPU, .name = "HAL_XPU2_CNOC_GEPHY_MPU"}, //Bit 30
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 31
	},
	{
		{.xpu_id = HAL_XPU2_CNOC_NOC_CFG_MPU, .name = "HAL_XPU2_CNOC_NOC_CFG_MPU"}, //Bit 0
		{.xpu_id = HAL_XPU2_CNOC_QDSS_CFG_MPU, .name = "HAL_XPU2_CNOC_QDSS_CFG_MPU"}, //Bit 1
		{.xpu_id = HAL_XPU2_PCIE_USBPHY_CFG_MPU, .name = "HAL_XPU2_PCIE_USBPHY_CFG_MPU"}, //Bit 2
		{.xpu_id = HAL_XPU2_CNOC_CMN_PLL_CFG_MPU, .name = "HAL_XPU2_CNOC_CMN_PLL_CFG_MPU"}, //Bit 3
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 4
		{.xpu_id = HAL_XPU2_CNOC_APSS_MPU, .name = "HAL_XPU2_CNOC_APSS_MPU"}, //Bit 5
		{.xpu_id = HAL_XPU2_USB_CFG_APU, .name = "HAL_XPU2_USB_CFG_APU"}, //Bit 6
		{.xpu_id = HAL_XPU2_BOOT_ROM, .name = "HAL_XPU2_BOOT_ROM"}, //Bit 7
		{.xpu_id = HAL_XPU2_TLMM, .name = "HAL_XPU2_TLMM"}, //Bit 8
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 9
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 10
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 11
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 12
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 13
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 14
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 15
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 16
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 17
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 18
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 19
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 20
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 21
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 22
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 23
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 24
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 25
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 26
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 27
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 28
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 29
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 30
		{.xpu_id = HAL_XPU2_COUNT, .name = "HAL_XPU2_COUNT"}, //Bit 31
	},
};

const HAL_xpu2_XPU2Type g_xpu_enum_count = HAL_XPU2_COUNT;

const bool g_was_supported = true;


/*MD5:88b287ae4e5148fdfc217717afa7cd66*/
