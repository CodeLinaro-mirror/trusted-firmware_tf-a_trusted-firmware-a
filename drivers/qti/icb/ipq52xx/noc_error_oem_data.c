/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error.h"
#include "noc_error_target.h"

/*============================================================================
 *			TARGET SPECIFIC DATA
 *============================================================================
 */

// FIXME_IPQ52xx
// TODO: Update NOC TMO Ratio values once H/W team shares frequency details
/*
 * Note:
 * We have configured timeout as approx. 1.4s for all NOCs in IPQ52xx.
 *
 * For a single transaction, this is sufficient time & hence this should be helpful
 * to identify real NOC Timeout issues.
 *
 * CONFIG_NOC time_scaling = 4096, for other NOCs its 1024.
 * In IPQ52xx, all NOCs time scaling was 1024.
 *
 * Ratio values are updated accordingly for CONFIG_NOC in IPQ52xx.
 */
/* OEM NOC Configuration Data*/
nocerr_info_type_oem nocerr_cfgdata_oem[] = {
	[0] = {
		.name        = "CONFIG_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0xfffffbd3,
				.faultin_en0_high = 0x1c25dbf,
				.faultin_en1_low = 0xf74 }, /* enabling Fault & Timeout bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1,
				.faultin_en0_high = 0x0,
				.faultin_en1_low = 0x0 }, /* enabling Fault bits */
		},
		.to_reg_vals = (uint32_t[]) { 0x0, /* MAINCTL_LOW */
					      0x10D, /* REFCTL0_LOW */
					      0x10D, /* REFCTL1_LOW */
					      0x10D, /* REFCTL2_LOW */
					      0x10D, /* REFCTL3_LOW */
					      0x10D, /* REFCTL4_LOW */
					      0x10D, /* REFCTL5_LOW */
					      0x10D, /* REFCTL6_LOW */
					      0x10D, /* REFCTL7_LOW */
					      0x10D, /* REFCTL8_LOW */
					      0x10D, /* REFCTL9_LOW */
					      0x10D, /* REFCTL10_LOW */
					      0x10D, /* REFCTL11_LOW */
					      0x10D, /* REFCTL12_LOW */
					      0x10D, /* REFCTL13_LOW */
					      0x10D, /* REFCTL14_LOW */
					      0x10D, /* REFCTL15_LOW */
					      0x10D, /* REFCTL16_LOW */
					      0x10D, /* REFCTL17_LOW */
					      0x10D, /* REFCTL18_LOW */
					      0x10D, /* REFCTL19_LOW */
					      0x1, /* MAINCTL_LOW */ },
	},

	[1] = {
		.name        = "MEM_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x7 }, /* enabling Fault & Timeout bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x5 }, /* enabling Fault bits */
		},
		.to_reg_vals = (uint32_t[]) { 0x0, /* MAINCTL_LOW */
					      0x10F, /* REFCTL0_LOW */
					      0x1, /* MAINCTL_LOW */ },
		},

	[2] = {
		.name        = "SYSTEM_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1 }, /* enabling Fault bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1 }, /* enabling Fault bits */
		},
		.to_reg_vals = NULL,
	},


	[3] = {
		.name        = "NSS_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x73f }, /* enabling Fault & Timeout bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1 }, /* enabling Fault bits */
		},
		/* NSS NOC Timeout clock is fixed at 6 MHz, cannot be configured */
		.to_reg_vals = NULL,
	},

};

uint32_t clock_reg_vals[] = {
	[0] = 0x1, /* Enable GCC_GEMNOC_XO_DBG_CBCR */
	[1] = 0x1, /* Enable GCC_SNOC_XO_DBG_CBCR */
	[2] = 0x1, /* Enable GCC_PCNOC_XO_DBG_CBCR */
	[3] = 0x1, /* Enable GCC_DDRSS_AHB_CBCR */
	[4] = 0x1, /* Enable GCC_DDRSS_ATB_CBCR */
	[5] = 0x1, /* Enable GCC_NSSCFG_CBCR */
	[6] = 0x1, /* Enable GCC_NSSNOC_TIMEOUT_REF_CBCR */
};

nocerr_config_info_type_oem nocerr_config_info_oem = {
	.num_configs = 1,
	.configs = (nocerr_propdata_type_oem[]) {
		/* Target info: IPQ52xx */
		[0] = {
			.family          = CHIPINFO_FAMILY_IPQ52xx,
			.match           = false,
			.version         = CHIPINFO_VERSION(1, 0),

			.len              = sizeof(nocerr_cfgdata_oem)/sizeof(nocerr_info_type_oem),
			.noc_info_oem_list = nocerr_cfgdata_oem,
			.clock_reg_vals   = clock_reg_vals,
			.filters          = NULL,
		},
	},
};
