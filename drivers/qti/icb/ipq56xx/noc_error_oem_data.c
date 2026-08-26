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

// FIXME_IPQ56xx
// TODO: Update NOC TMO Ratio values once H/W team shares frequency details
/*
 * Note:
 * We have configured timeout as approx. 1.4s for all NOCs in IPQ56xx.
 *
 * For a single transaction, this is sufficient time & hence this should be
 * helpful to identify real NOC Timeout issues.
 */
/* OEM NOC Configuration Data*/
nocerr_info_type_oem nocerr_cfgdata_oem[] = {
	[0] = {
		.name        = "CONFIG_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0xfffffff9,
				.faultin_en0_high = 0x85ff7fff,
				.faultin_en1_low = 0x8644 }, /* enabling Fault & Timeout bits */
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
		.name        = "SYSTEM_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1 }, /* enabling Fault bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1 }, /* enabling Fault bits */
		},
		/* num_tos = 3 in noc_error_data.c */
		.to_reg_vals = (uint32_t[]) { 0x0, /* MAINCTL_LOW */
					      0x10F, /* REFCTL0_LOW */
					      0x1, /* MAINCTL_LOW */ },
	},

	[2] = {
		.name        = "AGGR_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x7c04 }, /* enabling Fault and Timeout bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x4 }, /* enabling Fault bits */
		},
		/* num_tos = 7 in noc_error_data.c */
		.to_reg_vals = (uint32_t[]) { 0x0, /* MAINCTL_LOW */
					      0x10F, /* REFCTL0_LOW */
					      0x10F, /* REFCTL1_LOW */
					      0x10F, /* REFCTL2_LOW */
					      0x10F, /* REFCTL3_LOW */
					      0x10F, /* REFCTL4_LOW */
					      0x1, /* MAINCTL_LOW */ },
	},
	[3] = {
		.name        = "NSS_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0xf3f }, /* enabling Fault & Timeout bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1 }, /* enabling Fault bits */
		},
		/* NSS NOC Timeout clock is fixed at 6 MHz, cannot be configured */
		.to_reg_vals = NULL,
	},
	[4] = {
		.name        = "GEM_NOC",
		.intr_enable = true,
		.error_fatal = false,
		.sbms        = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0x1ff }, /* enabling Fault & Timeout bits */
		},
		.obs_mask    = (nocerr_sbm_info_type_oem[]) {
			[0] = { .faultin_en0_low = 0xF }, /* enabling Fault bits */
		},
		.to_reg_vals = (uint32_t[]) { 0x0, /* MAINCTL_LOW */
					      0x10F, /* REFCTL0_LOW */
					      0x10F, /* REFCTL1_LOW */
					      0x10F, /* REFCTL2_LOW */
					      0x10F, /* REFCTL3_LOW */
					      0x10F, /* REFCTL4_LOW */
					      0x10F, /* REFCTL5_LOW */
					      0x10F, /* REFCTL6_LOW */
					      0x10F, /* REFCTL7_LOW */
					      0x10F, /* REFCTL8_LOW */
					      0x10F, /* REFCTL9_LOW */
					      0x10F, /* REFCTL10_LOW */
					      0x10F, /* REFCTL11_LOW */
					      0x10F, /* REFCTL12_LOW */
					      0x10F, /* REFCTL13_LOW */
					      0x10F, /* REFCTL14_LOW */
					      0x1, /* MAINCTL_LOW */ },
		.poc         = (nocerr_pos_info_type_oem[]) {
			[0] = { .enable = true }, /* QNS_LLCC0 */
			[1] = { .enable = true }, /* QNS_LLCC1 */
			[2] = { .enable = true }, /* QNS_PCNOC */
			[3] = { .enable = true }, /* QNS_PCIE */
		},
	},
};

uint32_t clock_reg_vals[] = {
	[0] = 0x1, /* Enable GEMNOC_ANOC_PCIE clock */
	[1] = 0x1, /* Enable SNOC_XO_DBG refgen timeout unit reference clock */
	[2] = 0x1, /* Enable ANOC0_AXI clock */
	[3] = 0x1, /* Enable ANOC_XO_DBG refgen timeout unit reference clock */
	[4] = 0x1, /* Enable PCNOC_XO_DBG refgen timeout unit reference clock */
	[5] = 0x1, /* Enable DDRSS_AHB clock */
	[6] = 0x1, /* Enable DDRSS_ATB clock */
	[7] = 0x1, /* Enable CNOC_TURING_AHBS clock */
	[8] = 0x1, /* Enable NSSCFG clock */
	[9] = 0x1, /* Enable NSSNOC_TIMEOUT_REF clock */
};

nocerr_config_info_type_oem nocerr_config_info_oem = {
	.num_configs = 1,
	.configs = (nocerr_propdata_type_oem[]) {
		/* Target info: IPQ56xx*/
		[0] = {
			.family          = CHIPINFO_FAMILY_IPQ56xx,
			.match           = false,
			.version         = CHIPINFO_VERSION(1, 0),

			.len              = sizeof(nocerr_cfgdata_oem)/sizeof(nocerr_info_type_oem),
			.noc_info_oem_list = nocerr_cfgdata_oem,
			.clock_reg_vals   = clock_reg_vals,
			.filters          = NULL,
		},
	},
};