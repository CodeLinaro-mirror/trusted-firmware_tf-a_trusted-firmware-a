#ifndef __NOC_HW_TYPE_H__
#define __NOC_HW_TYPE_H__

/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

static noc_hw_type QNOC_4_0 = {
	.swid_low       = 0x0,
	.swid_high      = 0x4,
	.main_ctl_low   = 0x8,
	.err_valid_low  = 0x10,
	.err_clear_low  = 0x18,
	.errlog0_low    = 0x20,
	.errlog0_high   = 0x24,
	.errlog1_low    = 0x28,
	.errlog1_high   = 0x2C,
	.errlog2_low    = 0x30,
	.errlog2_high   = 0x34,
	.errlog3_low    = 0x38,
	.errlog3_high   = 0x3C,
	.errlog2_1_low  = REGISTER_NOT_APPLICABLE,
	.errlog2_1_high = REGISTER_NOT_APPLICABLE,
	.errlog4_3_low  = REGISTER_NOT_APPLICABLE,
	.errlog4_3_high = REGISTER_NOT_APPLICABLE,
	.errlog6_5_low  = REGISTER_NOT_APPLICABLE,
	.errlog6_5_high = REGISTER_NOT_APPLICABLE,
	.errlog8_high   = REGISTER_NOT_APPLICABLE,
};


/* NOC SidebandManager HW Register Offsets */
static noc_sideband_hw_type QNOC_SB_4_0_L = {
	.swid_low             = 0x0,
	.swid_high            = 0x4,
	.faultin_en0_low      = 0x40,
	.faultin_en0_high     = REGISTER_NOT_APPLICABLE,
	.faultin_status0_low  = 0x48,
	.faultin_status0_high = REGISTER_NOT_APPLICABLE,
	.faultin_en1_low      = REGISTER_NOT_APPLICABLE,
	.faultin_en1_high     = REGISTER_NOT_APPLICABLE,
	.faultin_status1_low  = REGISTER_NOT_APPLICABLE,
	.faultin_status1_high = REGISTER_NOT_APPLICABLE,
};

static noc_sideband_hw_type QNOC_SB_4_0 __attribute__((unused)) = {
	.swid_low             = 0x0,
	.swid_high            = 0x4,
	.faultin_en0_low      = 0x40,
	.faultin_en0_high     = 0x44,
	.faultin_status0_low  = 0x48,
	.faultin_status0_high = 0x4C,
	.faultin_en1_low      = REGISTER_NOT_APPLICABLE,
	.faultin_en1_high     = REGISTER_NOT_APPLICABLE,
	.faultin_status1_low  = REGISTER_NOT_APPLICABLE,
	.faultin_status1_high = REGISTER_NOT_APPLICABLE,
};

static noc_sideband_hw_type QNOC_SB_4_0_2L = {
	.swid_low             = 0x0,
	.swid_high            = 0x4,
	.faultin_en0_low      = 0x40,
	.faultin_en0_high     = 0x44,
	.faultin_status0_low  = 0x48,
	.faultin_status0_high = 0x4C,
	.faultin_en1_low      = 0x50,
	.faultin_en1_high     = REGISTER_NOT_APPLICABLE,
	.faultin_status1_low  = 0x58,
	.faultin_status1_high = REGISTER_NOT_APPLICABLE,
};

static noc_sideband_hw_type QNOC_SB_4_0_2 __attribute__((unused)) = {
	.swid_low             = 0x0,
	.swid_high            = 0x4,
	.faultin_en0_low      = 0x40,
	.faultin_en0_high     = 0x44,
	.faultin_status0_low  = 0x48,
	.faultin_status0_high = 0x4C,
	.faultin_en1_low      = 0x50,
	.faultin_en1_high     = 0x54,
	.faultin_status1_low  = 0x58,
	.faultin_status1_high = 0x5C,
};

static noc_pos_hw_type QNOC_POS_4_0 __attribute__((unused)) = {
	.swid_low      = 0x0,
	.swid_high     = 0x4,
	.errlog_low    = 0x8,
	.errlog_high   = 0xC,
	.errlogclr_low = 0x10,
};

static noc_poc_hw_type QNOC_POC_1_0 = {
	.swid_low        = 0x00,
	.swid_high       = 0x04,
	.errset_low      = 0x08,
	.errstatus_low   = 0x10,
	.errack_low      = 0x18,
	.errlogmain_low  = 0x20,
	.errlogmain_high = 0x24,
	.errlogaddr_low  = 0x28,
	.errlogaddr_high = 0x2C,
	.errloguser_low  = 0x30,
	.errloguser_high = 0x34,
};

#endif /* __NOC_HW_TYPE_H__ */
