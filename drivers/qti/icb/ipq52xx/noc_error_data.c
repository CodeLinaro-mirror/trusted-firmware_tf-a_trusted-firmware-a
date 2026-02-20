/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error.h"
#include "noc_error_hwio.h"
#include <lib/utils_def.h>
#include "noc_hw_type.h"
#include "noc_error_target.h"

/*============================================================================
 *			DEVICE CONFIG PROPERTY DATA
 *============================================================================
 */

/* Internal NOC Configuration Data*/
static nocerr_info_type nocerr_cfgdata[] = {
	[0] = {
		.name            = "CONFIG_NOC",
		.hw              = &QNOC_4_0,
		.base_addr       = (void *)HWIO_ADDR(CONFIG_NOC_CNOC_ERL_SWID_LOW),
		.intr_vector     = 59,
		.num_sbms        = 1,
		.sb_hw           = (noc_sideband_hw_type*[]) { &QNOC_SB_4_0_2L },
		.sb_base_addrs   = (void *[]){
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_SBM_SWID_LOW) },
		.num_tos         = 22,
		.to_addrs        = (void *[]){
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_MAINCTL_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL0_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL1_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL2_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL3_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL4_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL5_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL6_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL7_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL8_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL9_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL10_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL11_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL12_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL13_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL14_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL15_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL16_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL17_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL18_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_REFCTL19_LOW),
			(void *)HWIO_ADDR(CONFIG_NOC_CNOC_REFGEN_MAINCTL_LOW), },
		.syndrome        = { .sbms = (nocerr_sbm_syndrome_type[]) { {0, 0, 0, 0} }, },
	},

	[1] = {
	.name            = "MEM_NOC",
	.hw              = &QNOC_4_0,
	.base_addr       = (void *)HWIO_ADDR(MEM_NOC_MEM_NOC_ERL_SWID_LOW),
	.intr_vector     = 58,
	.num_sbms        = 1,
	.sb_hw           = (noc_sideband_hw_type*[]) { &QNOC_SB_4_0_L },
	.sb_base_addrs   = (void *[]) {
		(void *)HWIO_MEM_NOC_MEM_NOC_MEM_NOC_FAULT_SBM_SWID_LOW_ADDR },
	.num_tos         = 3,
	.to_addrs        = (void *[]) { (void *)HWIO_MEM_NOC_MEM_NOC_REFGEN_MAINCTL_LOW_ADDR,
					(void *)HWIO_MEM_NOC_MEM_NOC_REFGEN_REFCTL0_LOW_ADDR,
					(void *)HWIO_MEM_NOC_MEM_NOC_REFGEN_MAINCTL_LOW_ADDR, },
	.syndrome        = { .sbms = (nocerr_sbm_syndrome_type[]) { {0, 0} }, },
	},

	[2] = {
		.name            = "SYSTEM_NOC",
		.hw              = &QNOC_4_0,
		.base_addr       = (void *)HWIO_ADDR(SYSTEM_NOC_SYSTEM_NOC_ERL_SWID_LOW),
		.intr_vector     = 157, /* snoc_sbm_Fault */
		.num_sbms        = 1,
		.sb_hw           = (noc_sideband_hw_type*[]) { &QNOC_SB_4_0_L },
		.sb_base_addrs   = (void *[]){
			(void *)HWIO_ADDR(SYSTEM_NOC_SYSTEM_NOC_SBM_SWID_LOW) },
		.syndrome        = { .sbms = (nocerr_sbm_syndrome_type[]) { {0, 0, 0, 0} }, },
	},

	[3] = {
		.name            = "NSS_NOC",
		.hw              = &QNOC_4_0,
		.base_addr       = (void *)HWIO_ADDR(NSS_NOC_ERL_SWID_LOW),
		.intr_vector     = 242,
		.num_sbms        = 1,
		.sb_hw           = (noc_sideband_hw_type*[]) { &QNOC_SB_4_0_L },
		.sb_base_addrs   = (void *[]){
			(void *)HWIO_ADDR(NSS_NOC_SBM_SWID_LOW), },
		.num_tos         = 0,
		.to_addrs        = NULL,
		.syndrome        = { .sbms = (nocerr_sbm_syndrome_type[]) { {0, 0, 0, 0} }, },
	}
};

static void *clock_reg_addrs[] = {
	(void *)HWIO_ADDR(GCC_GEMNOC_XO_DBG_CBCR),
	(void *)HWIO_ADDR(GCC_SNOC_XO_DBG_CBCR),
	(void *)HWIO_ADDR(GCC_PCNOC_XO_DBG_CBCR),
	(void *)HWIO_ADDR(GCC_DDRSS_AHB_CBCR),
	(void *)HWIO_ADDR(GCC_DDRSS_ATB_CBCR),
	(void *)HWIO_ADDR(GCC_NSSCFG_CBCR),
	(void *)HWIO_ADDR(GCC_NSSNOC_TIMEOUT_REF_CBCR),
};

nocerr_config_info_type nocerr_config_info = {
	.num_configs = 1,
	.configs = (nocerr_propdata_type[]) {
		/* Target info: IPQ52xx */
		[0] = {
			.family          = CHIPINFO_FAMILY_IPQ52xx,
			.match           = false,
			.version         = CHIPINFO_VERSION(1, 0),

			/* NoC info. */
			.len             = ARRAY_SIZE(nocerr_cfgdata),
			.noc_info_list   = nocerr_cfgdata,
			.num_clock_regs  = ARRAY_SIZE(clock_reg_addrs),
			.clock_reg_addrs = clock_reg_addrs,
		},
	},
};
