/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error_decode.h"
#include "noc_error_hwio.h"
#include <common/debug.h>

noc_decode_data_type noc_decode_info;

const char * const noc_initiator[4][50] = {
	{   /* CNOC */
		"qhm_tic",
		"qnm_gemnoc_pcie",
		"qnm_gemnoc_sys",
		"xm_qdss_dap",
	},
	{   /* SNOC */
		"qhm0",
		"qhm_pcie_ep",
		"qhm_snoc_cfg",
		"qhm_tme",
		"qnm_cnoc",
		"qxm_lpass",
		"xm_pcie_1lane",
		"xm_pcie_2lane",
		"xm_qdss_etr",
		"xm_sdcc",
		"xm_tme",
		"xm_usb",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* MEM_NOC */
		"qhm_memnoc_cfg",
		"qnm_memnoc_sys0",
		"qnm_memnoc_sys1",
		"qnm_nss",
		"xm_app0",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* NSSNOC */
		"qhm_nssnoc_cfg",
		"qxm_nssnoc_snoc",
		"xm_eip196a",
		"xm_ppe",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
};

const char * const noc_target[4][50] = {
	{   /* CNOC */
		"qhs10",
		"qhs11",
		"qhs1",
		"qhs2",
		"qhs3",
		"qhs4",
		"qhs5",
		"qhs6",
		"qhs7",
		"qhs8",
		"qhs9",
		"qhs_apss_cfg",
		"qhs_lpass_cfg",
		"qhs_tme_cfg",
		"qhs_usb_cfg",
		"qns_nssnoc",
		"qns_snoc",
		"qps_pon_mac",
		"qxs_imem",
		"srvc_cnoc",
		"xs_pcie_1lane",
		"xs_pcie_2lane",
		"xs_pcie_ep_cfg",
		"xs_qdss_stm",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* SNOC */
		"qhs_pcie_1lane_xpu_cfg",
		"qhs_pcie_2lane_xpu_cfg",
		"qns_memnoc_sys0",
		"qns_memnoc_sys1",
		"srvc_snoc",
	},
	{   /* MEM_NOC */
		"dbhn_sch0",
		"dtb_reg",
		"qhs_memnoc_mpu_sch0_cfg",
		"qhs_memnoc_xpu_cfg",
		"qns_pcie",
		"qns_pcnoc",
		"srvc_memnoc",
		"RESERVED",
	},
	{   /* NSSNOC */
		"qhs_nss_csr",
		"qhs_nsscc_cfg",
		"qhs_ppe_cfg",
		"qxs_nssnoc_snoc",
		"srvc_nssnoc",
		"xs_eip196a_cfg",
		"xs_ppe_boot_mem",
		"RESERVED",
	},
};

const char * const noc_errcode[] = {
	"Slave error",
	"Decode error",
	"Unsupported access error",
	"Disconnect error",
	"Security error",
	"Hidden security error",
	"Time-out error",
	"Tag match operation failed",
	"Data error",
	"Internal error",
};

void noc_decode_id(uint32_t errlog1_high, char *noc_type)
{
	noc_decode_info.bid = (((errlog1_high) >> 13) & (0x7)); /* 13-15 bits*/
	noc_decode_info.pid = (((errlog1_high) >> 8) & (0x1F)); /* 8-12 bits*/
	noc_decode_info.mid = ((errlog1_high) & (0xFF));    /* 0-7 bits*/

	ERROR("%s ERROR Decode : BID = 0x%x, PID = 0x%x, MID = 0x%x\n",
	      noc_type,
	      noc_decode_info.bid,
	      noc_decode_info.pid,
	      noc_decode_info.mid);
}

void noc_decode_target_initiator(uint32_t errlog1_low, char *noc_type)
{

	if (!strcmp(noc_type, "CONFIG_NOC")) {
		noc_decode_info.target_index = ((errlog1_low) & (0x1F)); /* 0-4 bits*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 5) & (0x3)); /* 5-6 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[0][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[0][noc_decode_info.target_index]);
	} else if (!strcmp(noc_type, "SYSTEM_NOC")) {
		noc_decode_info.target_index = ((errlog1_low) & (0x7)); /* 0-2 bits*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 3) & (0xF)); /* 3-6 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[1][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[1][noc_decode_info.target_index]);
	} else if (!strcmp(noc_type, "MEM_NOC")) {
		noc_decode_info.target_index = ((errlog1_low) & (0x7)); /* 0-2 bits*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 3) & (0x7)); /* 3-5 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[2][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[2][noc_decode_info.target_index]);
	} else if (!strcmp(noc_type, "NSS_NOC")) {
		noc_decode_info.target_index = ((errlog1_low) & (0x7)); /* 0-2 bits*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 3) & (0x3)); /* 3-4 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[3][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[3][noc_decode_info.target_index]);
	}
}

void noc_decode_errcode(uint32_t errlog0_low, char *noc_type)
{
	noc_decode_info.errcode = (((errlog0_low) >> 8) & (0x7));   /* 8-10 bits */
	noc_decode_info.log_info_vld = ((errlog0_low) & (0x1));     /* bit 0 */
	if (!noc_decode_info.log_info_vld) {
		ERROR("%s ERROR Decode : Target Type = Minimal Debug Target\n",
			noc_type);
	} else {
		ERROR("%s ERROR Decode : ErrCode = %s\n",
			noc_type,
			noc_errcode[noc_decode_info.errcode]);
	}
}

void noc_decode_violation_addr(uint32_t errlog2_high, uint32_t errlog2_low,
			       char *noc_type)
{
	ERROR("%s ERROR Decode : Address of Violation = 0x%x%08x\n",
	      noc_type,
	      errlog2_high,
	      errlog2_low);
}

void qti_noc_error_decode(nocerr_info_type *nocerr_info)
{
	if (nocerr_info == NULL)
		return;

	memset(&noc_decode_info, 0, sizeof(noc_decode_data_type));

	noc_decode_id(nocerr_info->syndrome.ERRLOG1_HIGH,
		      nocerr_info->name);

	noc_decode_target_initiator(nocerr_info->syndrome.ERRLOG1_LOW,
				    nocerr_info->name);

	noc_decode_errcode(nocerr_info->syndrome.ERRLOG0_LOW,
			   nocerr_info->name);

	noc_decode_violation_addr(nocerr_info->syndrome.ERRLOG2_HIGH,
				  nocerr_info->syndrome.ERRLOG2_LOW,
				  nocerr_info->name);
}
