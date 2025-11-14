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
		.start        = 0x00580000,
		.end          = 0x00581000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
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
		.start        = 0x01d0a000,
		.end          = 0x01d0b000,
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
	{
		.rg_num        = 35,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 36,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 37,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 38,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 39,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 40,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 41,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 42,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 43,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 44,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 45,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 46,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 48,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 49,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 50,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 51,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 52,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 53,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 54,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 55,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 56,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 57,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 58,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
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
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 62,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 63,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 64,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 65,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 66,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 67,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 68,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 69,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 70,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 71,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 73,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 74,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 75,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 76,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 77,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 78,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 79,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 80,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 81,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 82,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 83,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 84,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 85,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 86,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 87,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 88,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 89,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 90,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 91,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 92,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 93,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 94,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 95,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 96,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 97,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 98,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 99,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 100,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 101,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 102,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 103,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 104,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 105,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 106,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 107,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 108,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 109,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 110,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 111,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 112,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 113,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 114,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 115,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 116,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 117,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 118,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 119,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 120,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 121,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 122,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 123,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 124,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 125,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 126,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 127,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 128,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 129,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 130,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 131,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 132,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 133,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 134,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 135,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 136,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 137,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 138,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 139,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 140,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 141,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 142,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 143,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 144,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 145,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 146,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 147,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 148,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 149,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 150,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 151,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 152,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 153,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 154,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 155,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 156,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 157,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 158,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 159,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 160,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 161,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 162,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 163,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 164,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 165,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 166,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 167,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 168,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 169,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 170,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 171,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 172,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 173,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 174,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 175,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 176,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 177,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 178,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 179,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 180,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 181,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 182,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 183,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 184,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 185,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 186,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 187,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 188,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 189,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 190,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 191,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 192,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 193,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 194,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 195,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 196,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 197,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 198,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 199,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
};

/* GEMNOC_CFG_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg gemnoc_cfg_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x04000000,
		.end          = 0x04001000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x04071000,
		.end          = 0x04072000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT
	},
	{
		.rg_num        = 2,
		.start        = 0x040b0000,
		.end          = 0x040b2000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 3,
		.start        = 0x040b0000,
		.end          = 0x040b2000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_APPS_SEC_BIT
	},
};

/* MACHX_LLCC_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg machx_llcc_mpu_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x8a600000,
		.end          = 0x8a680000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x8a680000,
		.end          = 0x8a900000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
};

/* MACHX_LLCC_SCID_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg machx_llcc_scid_xpu4[] = {
	{
		.rg_num        = 0,
		.start        = 0x00000000,
		.end          = 0x00000001,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT
	},
	{
		.rg_num        = 1,
		.start        = 0x00000004,
		.end          = 0x00000005,
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
};

/* OCIMEM_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg ocimem_apu_xpu4[] = {
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
};

/* OCIMEM_MPU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_mpu_rg ocimem_mpu_xpu4[] = {
	{
		.rg_num        = 10,
		.start        = 0x08600000,
		.end          = 0x08601000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT
	},
	{
		.rg_num        = 11,
		.start        = 0x08601000,
		.end          = 0x08607000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 12,
		.start        = 0x08607000,
		.end          = 0x0860A000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 13,
		.start        = 0x0860A000,
		.end          = 0x0860D000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_APPS_SEC_BIT
	},
	{
		.rg_num        = 14,
		.start        = 0x0860D000,
		.end          = 0x0861f000,
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

/* SEC_CTRL_APU_XPU4 */
__section(".ac_static_cfg_data.xpu") static const ac_rpu_rg sec_ctrl_apu_xpu4[] = {
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
	{
		.rg_num        = 35,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 36,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 37,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 38,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 39,
		.profile_flags = AC_PROFILE_DEFAULT,
		.read_qads     = AC_DOMAIN_AP_NS_BIT,
		.write_qads    = AC_DOMAIN_AP_NS_BIT
	},
	{
		.rg_num        = 40,
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
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_TME_ROM_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_CMN_PLL_CFG_MPU_XPU4 */
		.base_addr     = 0x00566000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_CMN_PLL_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_LPASS_CFG_MPU_XPU4 */
		.base_addr     = 0x0056c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_LPASS_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_NOC_CFG_MPU_XPU4 */
		.base_addr     = 0x00560000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_NOC_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(cnoc_noc_cfg_mpu_xpu4),
		.rg           = {cnoc_noc_cfg_mpu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_NSSNOC_MPU_XPU4 */
		.base_addr     = 0x0056e000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_NSSNOC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_PCIE_CFG_MPU_XPU4 */
		.base_addr     = 0x00562000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_PCIE_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_PROCESSOR_CFG_MPU_XPU4 */
		.base_addr     = 0x00564000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_PROCESSOR_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_QDSS_CFG_MPU_XPU4 */
		.base_addr     = 0x00568000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_QDSS_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_SDCC_CFG_MPU_XPU4 */
		.base_addr     = 0x00572000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_SDCC_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_TCU_CFG_MPU_XPU4 */
		.base_addr     = 0x0057e000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_TCU_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_USBCTL_CFG_MPU_XPU4 */
		.base_addr     = 0x00570000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_USBCTL_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU_XPU4 */
		.base_addr     = 0x0057a000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* GCC_RPU_XPU4 */
		.base_addr     = 0x01880000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_GCC_RPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(gcc_rpu_xpu4),
		.rg           = {gcc_rpu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* GEMNOC_CFG_XPU4 */
		.base_addr     = 0x042e0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_GEMNOC_CFG_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(gemnoc_cfg_xpu4),
		.rg           = {gemnoc_cfg_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* MACHX_LLCC_MPU_XPU4 */
		.base_addr     = 0x04060000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MACHX_LLCC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(machx_llcc_mpu_xpu4),
		.rg           = {machx_llcc_mpu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.rg_ignored    = NULL,
	},
	{	/* MACHX_LLCC_SCID_XPU4 */
		.base_addr     = 0x040b0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MACHX_LLCC_SCID_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(machx_llcc_scid_xpu4),
		.rg           = {machx_llcc_scid_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* MPM2_MPM_APU_XPU4 */
		.base_addr     = 0x004ac000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_MPM2_MPM_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(mpm2_mpm_apu_xpu4),
		.rg           = {mpm2_mpm_apu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* OCIMEM_APU_XPU4 */
		.base_addr     = 0x00052000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_IMEM_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(ocimem_apu_xpu4),
		.rg           = {ocimem_apu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* OCIMEM_MPU_XPU4 */
		.base_addr     = 0x00054000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_IMEM_MPU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(ocimem_mpu_xpu4),
		.rg           = {ocimem_mpu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.rg_ignored    = NULL,
	},
	{	/* QDSS_CFG_DCC_DCC_XPU4 */
		.base_addr     = 0x060e0000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QDSS_CFG_DCC_DCC,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(qdss_cfg_dcc_dcc_xpu4),
		.rg           = {qdss_cfg_dcc_dcc_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* QMIP_XPU_CFG_XPU4 */
		.base_addr     = 0x00060000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QMIP_XPU_CFG,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* QPIC_QPIC_MPU_XPU4 */
		.base_addr     = 0x079b1000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QPIC_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* QPIC_XPU4 */
		.base_addr     = 0x07981000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QPIC_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(qpic_xpu4),
		.rg           = {qpic_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* SDC1_SDCC_ICE_XPU4 */
		.base_addr     = 0x0780c000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_SDC1_SDCC_ICE,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(sdc1_sdcc_ice_xpu4),
		.rg           = {sdc1_sdcc_ice_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* SEC_CTRL_APU_XPU4 */
		.base_addr     = 0x000ae000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_SEC_CTRL_APU,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(sec_ctrl_apu_xpu4),
		.rg           = {sec_ctrl_apu_xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* TCSR_XPU4 */
		.base_addr     = 0x01984000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_TCSR_REGS,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.rg_ignored    = NULL,
	},
	{	/* TLMM_XPU4 */
		.base_addr     = 0x01300000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_TLMM,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_APPS_SEC_BIT | AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_TME_ROM_BIT | AC_DOMAIN_TME_FW_BIT | AC_DOMAIN_AP_QC_BL_BIT | AC_DOMAIN_MSA_BIT | AC_DOMAIN_PRIME_BIT | AC_DOMAIN_RESERVED_7_BIT | AC_DOMAIN_RESERVED_8_BIT | AC_DOMAIN_RESERVED_9_BIT | AC_DOMAIN_RESERVED_10_BIT | AC_DOMAIN_RESERVED_11_BIT,
		.rg_ignored    = NULL,
	},
	{	/* XPU4 */
		.base_addr     = 0x01a00000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_QUPV3_5_GSI_TOP,
		.status       = XPU_ENABLED,
		.nrg          = ARRAY_SIZE(xpu4),
		.rg           = {xpu4},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT | AC_DOMAIN_APPS_SEC_BIT,
		.rg_ignored    = NULL,
	},
	{	/* XS_PCIE_1LANE_0_MPU_XPU4 */
		.base_addr     = 0x00620000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_1LANE_0_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* XS_PCIE_1LANE_1_MPU_XPU4 */
		.base_addr     = 0x00622000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_1LANE_1_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* XS_PCIE_2LANE_0_MPU_XPU4 */
		.base_addr     = 0x00624000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_2LANE_0_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* XS_PCIE_2LANE_1_MPU_XPU4 */
		.base_addr     = 0x00626000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_2LANE_1_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
	{	/* XS_PCIE_2LANE_2_MPU_XPU4 */
		.base_addr     = 0x00628000,
		.profile_flags = AC_PROFILE_DEFAULT,
		.xpu_id        = HAL_XPU2_XS_PCIE_2LANE_2_MPU,
		.status       = XPU_ENABLED,
		.nrg          = 0,
		.rg           = {NULL},
		.cfg_owner     = QAD_APPS,
		.umr_perm      = AC_DOMAIN_AP_NS_BIT,
		.rg_ignored    = NULL,
	},
};
const uint32_t g_ac_xpu_cfg_count = ARRAY_SIZE(g_ac_xpu_cfg);

/*MD5:268a32511c0eae1c5287b8dd94bd0b4f*/
