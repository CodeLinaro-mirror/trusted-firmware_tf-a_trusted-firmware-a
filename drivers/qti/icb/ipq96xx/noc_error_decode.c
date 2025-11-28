/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error_decode.h"
#include "noc_error_hwio.h"
#include <common/debug.h>

/* Maximum value for NDUMPTMO field (4-bit field, max value 15) */
#define MAX_NDUMPTMO 15

static uint32_t prev_faultinstatus0_low;

noc_decode_data_type noc_decode_info;

const char * const gemnoc_target[] = {
	"qns_llcc",
	"qns_pcnoc",
	"qns_pcie",
};

const char * const noc_initiator[5][50] = {
	{   /* CNOC */
		"qhm_tic",
		"qnm_gemnoc_sys",
		"xm_qdss_dap",
		"RESERVED",
	},
	{   /* GEMNOC */
		"RESERVED",
		"alm_tcu",
		"qhm_gemnoc_cfg",
		"qnm_aggr_noc_0",
		"qnm_aggr_noc_1",
		"qnm_nsp0",
		"qnm_nsp1",
		"qnm_nss",
		"qnm_snoc",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* SNOC */
		"qhm0",
		"qhm_pcie_ep",
		"qhm_prime",
		"qhm_snoc_cfg",
		"qhm_tme",
		"qnm_cnoc",
		"qxm_lpass",
		"qxm_prime",
		"xm_qdss_etr",
		"xm_sdcc",
		"xm_tme",
		"xm_usb",
		"xm_usb2",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* ANOC */
		"qhm_aggrnoc_cfg",
		"qnm_gemnoc_pcie",
		"xm_pcie_1lane_0",
		"xm_pcie_1lane_1",
		"xm_pcie_2lane_0",
		"xm_pcie_2lane_1",
		"xm_pcie_2lane_2",
		"RESERVED",
	},
	{   /* NSSNOC */
		"qhm_nssnoc_cfg",
		"qnm_nssnoc_cnoc",
		"xm_ce",
		"xm_eip197b",
		"xm_ppe",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
};

const char * const noc_target[5][50] = {
	{   /* CNOC */
		"qhs10",
		"qhs11",
		"qhs1",
		"qhs2",
		"qhs3",
		"qhs4",
		"qhs5",
		"qhs6",
		"qhs8",
		"qhs_apss_cfg",
		"qhs_gemnoc",
		"qhs_lpass_cfg",
		"qhs_prime_cfg",
		"qhs_tme_cfg",
		"qhs_turing_cfg",
		"qhs_usb_cfg",
		"qns_nssnoc",
		"qns_snoc",
		"qsi_group",
		"qxs_imem",
		"srvc_cnoc",
		"xs_pcie_ep_cfg",
		"xs_qdss_stm",
		"xs_tcu",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* GEMNOC */
		"apbs_mc_phy_csr_cfg",
		"qhs_danger_cfg",
		"qhs_gemnoc_xpu_cfg",
		"qhs_llcc_msi_cfg",
		"qns_llcc",
		"qns_pcie",
		"qns_pcnoc",
		"qsi_throttle_cfg",
		"srvc_gemnoc",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* SNOC */
		"qns_gemnoc",
		"srvc_snoc",
		"RESERVED",
		"RESERVED",
	},
	{   /* ANOC */
		"qhs_xm_pcie_danger_cfg",
		"qhs_xs_pcie_xpu_cfg",
		"qns_aggr_gemnoc_0",
		"qns_aggr_gemnoc_1",
		"qsi_throttle_cfg",
		"srvc_aggrnoc",
		"xs_pcie_1lane_0",
		"xs_pcie_1lane_1",
		"xs_pcie_2lane_0",
		"xs_pcie_2lane_1",
		"xs_pcie_2lane_2",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
	},
	{   /* NSSNOC */
		"qhs_ce_cfg",
		"qhs_nss_csr",
		"qhs_nsscc_cfg",
		"qhs_ppe_cfg",
		"qns_nssnoc_gemnoc",
		"srvc_nssnoc",
		"xs_eip197b_cfg",
		"xs_ppe_boot_mem",
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

void noc_decode_id_errcode_gemnoc_poc(uint32_t errlog_low, uint32_t errlog_high,
				      char *noc_type)
{
	noc_decode_info.errcode = (((errlog_low) >> 4) & (0xf));

	ERROR("%s ERROR Decode : ErrCode = %s\n",
	      noc_type,
	      noc_errcode[noc_decode_info.errcode]);

	/*
	 * Only 16 bits of ExtId is used for GEMNOC POC (out of 24 bits avail)
	 */
	noc_decode_info.initiator_type = (errlog_low & 0x1);
	/* initiator_type ---> If Socket=0: QCI, else If Socket=1: NTTP */

	if (noc_decode_info.initiator_type) {
		noc_decode_info.bid = (((errlog_low) >> 21) & (0x7)); /* 21-23 bits*/
		noc_decode_info.pid = (((errlog_low) >> 16) & (0x1F)); /* 16-20 bits*/
		noc_decode_info.mid = (((errlog_low) >> 8) & (0xFF));    /* 8-15 bits*/

		ERROR("%s ERROR Decode : BID = 0x%x, PID = 0x%x, MID = 0x%x\n",
		      noc_type,
		      noc_decode_info.bid,
		      noc_decode_info.pid,
		      noc_decode_info.mid);
	} else {
		noc_decode_info.bid = (((errlog_low) >> 21) & (0x7)); /* 21-23 bits*/
		noc_decode_info.pid = (((errlog_low) >> 16) & (0x1F)); /* 16-20 bits*/
		noc_decode_info.srcid = (((errlog_low) >> 14) & (0x3)); /* 14-15 bits*/
		noc_decode_info.lpid = (((errlog_low) >> 8) & (0x1F)); /* 8-12 bits*/

		ERROR("%s ERROR Decode : BID = 0x%x, PID = 0x%x, SRCID = 0x%x, LPID = 0x%x\n",
		      noc_type,
		      noc_decode_info.bid,
		      noc_decode_info.pid,
		      noc_decode_info.srcid,
		      noc_decode_info.lpid);
	}
}

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
	// FIXME_IPQ96xx
	// TODO: Update NSPNOC Initiator/Target info
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
		noc_decode_info.target_index = ((errlog1_low) & (0x1)); /* 0 bit*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 1) & (0xF)); /* 1-4 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[2][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[2][noc_decode_info.target_index]);
	} else if (!strcmp(noc_type, "AGGR_NOC")) {
		noc_decode_info.target_index = ((errlog1_low) & (0xF)); /* 0-3 bits*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 4) & (0x7)); /* 4-6 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[3][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[3][noc_decode_info.target_index]);
	} else if (!strcmp(noc_type, "NSS_NOC")) {
		noc_decode_info.target_index = ((errlog1_low) & (0x7)); /* 0-2 bits*/
		noc_decode_info.initiator_index = (((errlog1_low) >> 3) & (0x7)); /* 3-5 bits*/
		ERROR("%s ERROR Decode : Initiator = 0x%x - %s, Target = 0x%x - %s\n",
			noc_type,
			noc_decode_info.initiator_index,
			noc_initiator[4][noc_decode_info.initiator_index],
			noc_decode_info.target_index,
			noc_target[4][noc_decode_info.target_index]);
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

void gemnoc_qns_pcnoc_tmo_poc_dbg_dump(void)
{
	uint32_t ndump_tmo = HWIO_INF(GEMNOC_GEMNOC_QNS_PCNOC_POC_DBG_PARAMS_HIGH, NDUMPTMO);
	uint32_t next_dump_tmo_status = 0, i = 0, ext_id = 0;
	uint64_t addr_of_violation = 0;
	uint32_t stream_low[MAX_NDUMPTMO], stream_high[MAX_NDUMPTMO];

	/* Validate ndump_tmo against array bounds */
	if (ndump_tmo > MAX_NDUMPTMO) {
		ERROR("GEMNOC QNS PCNOC TMO: Invalid ndump_tmo value %u, clamping to %u\n",
		      ndump_tmo, MAX_NDUMPTMO);
		ndump_tmo = MAX_NDUMPTMO;
	}

	ERROR("GEMNOC QNS PCNOC TMO: POC_DBG Reg Dump\n");
	ERROR("DumpBrief_Low = 0x%08x\n",
	      HWIO_IN(GEMNOC_GEMNOC_QNS_PCNOC_POC_DBG_DUMPBRIEF_LOW));
	while (i < ndump_tmo) {
		next_dump_tmo_status =
			HWIO_IN(GEMNOC_GEMNOC_QNS_PCNOC_POC_DBG_DUMPTMOSTATUS_LOW);
		while (next_dump_tmo_status == 0) {
			next_dump_tmo_status =
				HWIO_IN(GEMNOC_GEMNOC_QNS_PCNOC_POC_DBG_DUMPTMOSTATUS_LOW);
		}
		ERROR("Turn: %u, NextDumpTmoStatus is %u\n", i, next_dump_tmo_status);
		stream_low[i] = HWIO_IN(GEMNOC_GEMNOC_QNS_PCNOC_POC_DBG_DUMPTMOSTREAM_LOW);
		stream_high[i] = HWIO_IN(GEMNOC_GEMNOC_QNS_PCNOC_POC_DBG_DUMPTMOSTREAM_HIGH);
		ERROR("DumpTmoStream%u_Low = 0x%08x\n", i, stream_low[i]);
		ERROR("DumpTmoStream%u_High = 0x%08x\n", i, stream_high[i]);
		i++;
	}

	noc_decode_info.initiator_type = ((stream_low[0] & 0x40) >> 6);

	if (noc_decode_info.initiator_type) {
		ext_id = ((stream_high[2] >> 7) & 0xFFFF);

		noc_decode_info.bid = (((ext_id) >> 13) & (0x7)); /* 13-15 bits*/
		noc_decode_info.pid = (((ext_id) >> 8) & (0x1F)); /* 8-12 bits*/
		noc_decode_info.mid = ((ext_id) & (0xFF));    /* 0-7 bits*/

		ERROR("GEMNOC QNS PCNOC TMO: ERROR Decode : BID = 0x%x, PID = 0x%x, MID = 0x%x\n",
		      noc_decode_info.bid,
		      noc_decode_info.pid,
		      noc_decode_info.mid);
	} else {
		noc_decode_info.tgtid = ((stream_low[2] & 0x3800000) >> 23);
		noc_decode_info.srcid = ((stream_low[2] & 0x1C000000) >> 26);
		noc_decode_info.lpid = ((stream_high[2] & 0xF80000000) >> 27);

	ERROR("GEMNOC QNS PCNOC TMO: ERROR Decode : TGTID = 0x%x, SRCID = 0x%x, LPID = 0x%x\n",
		      noc_decode_info.tgtid, noc_decode_info.srcid,
		      noc_decode_info.lpid);
	}

	addr_of_violation = ((stream_low[0] + ((uint64_t)stream_high[0] << 32)) >> 52) +
		      ((((stream_low[1] + ((uint64_t)stream_high[1] << 32)) & 0xFFFFFF)) << 12);

	ERROR("GEMNOC QNS PCNOC TMO: Address of violation = 0x%016" PRIx64 "\n",
	      addr_of_violation);
}

void gemnoc_qns_pcie_tmo_poc_dbg_dump(void)
{
	uint32_t ndump_tmo = HWIO_INF(GEMNOC_GEMNOC_QNS_PCIE_POC_DBG_PARAMS_HIGH, NDUMPTMO);
	uint32_t next_dump_tmo_status = 0, i = 0, ext_id = 0;
	uint64_t addr_of_violation = 0;
	uint32_t stream_low[MAX_NDUMPTMO], stream_high[MAX_NDUMPTMO];

	/* Validate ndump_tmo against array bounds */
	if (ndump_tmo > MAX_NDUMPTMO) {
		ERROR("GEMNOC QNS PCIE TMO: Invalid ndump_tmo value %u, clamping to %u\n",
		      ndump_tmo, MAX_NDUMPTMO);
		ndump_tmo = MAX_NDUMPTMO;
	}

	ERROR("GEMNOC QNS PCIE TMO: POC_DBG Reg Dump\n");
	ERROR("DumpBrief_Low = 0x%08x\n",
	      HWIO_IN(GEMNOC_GEMNOC_QNS_PCIE_POC_DBG_DUMPBRIEF_LOW));

	while (i < ndump_tmo) {
		next_dump_tmo_status =
			HWIO_IN(GEMNOC_GEMNOC_QNS_PCIE_POC_DBG_DUMPTMOSTATUS_LOW);
		while (next_dump_tmo_status == 0) {
			next_dump_tmo_status =
				HWIO_IN(GEMNOC_GEMNOC_QNS_PCIE_POC_DBG_DUMPTMOSTATUS_LOW);
		}
		ERROR("Turn: %u, NextDumpTmoStatus is %u\n", i, next_dump_tmo_status);
		stream_low[i] = HWIO_IN(GEMNOC_GEMNOC_QNS_PCIE_POC_DBG_DUMPTMOSTREAM_LOW);
		stream_high[i] = HWIO_IN(GEMNOC_GEMNOC_QNS_PCIE_POC_DBG_DUMPTMOSTREAM_HIGH);
		ERROR("DumpTmoStream%u_Low = 0x%08x\n", i, stream_low[i]);
		ERROR("DumpTmoStream%u_High = 0x%08x\n", i, stream_high[i]);
		i++;
	}

	noc_decode_info.initiator_type = ((stream_low[0] & 0x40) >> 6);

	if (noc_decode_info.initiator_type) {
		ext_id = (((stream_low[2] & 0x80000000) >> 31) +
			((stream_high[2] & 0x7FFF) << 1));

		noc_decode_info.bid = (((ext_id) >> 13) & (0x7)); /* 13-15 bits*/
		noc_decode_info.pid = (((ext_id) >> 8) & (0x1F)); /* 8-12 bits*/
		noc_decode_info.mid = ((ext_id) & (0xFF));    /* 0-7 bits*/

	ERROR("GEMNOC QNS PCIE TMO: ERROR Decode : BID = 0x%x, PID = 0x%x, MID = 0x%x\n",
		      noc_decode_info.bid,
		      noc_decode_info.pid,
		      noc_decode_info.mid);
	} else {
		noc_decode_info.tgtid = ((stream_low[2] & 0x38000) >> 15);
		noc_decode_info.srcid = ((stream_low[2] & 0x1C0000) >> 18);
		noc_decode_info.lpid = ((stream_high[2] & 0xF80000) >> 19);

	ERROR("GEMNOC QNS PCIE TMO: ERROR Decode : TGTID = 0x%x, SRCID = 0x%x, LPID = 0x%x\n",
		      noc_decode_info.tgtid, noc_decode_info.srcid,
		      noc_decode_info.lpid);
	}

	addr_of_violation = ((stream_low[0] + ((uint64_t)stream_high[0] << 32)) >> 46) +
		      ((((stream_low[1] + ((uint64_t)stream_high[1] << 32)) & 0x3FFFF)) << 18);

	ERROR("GEMNOC QNS PCIE TMO: Address of violation = 0x%016" PRIx64 "\n",
	      addr_of_violation);
}

void gemnoc_llcc_tmo_poc_dbg_dump(void)
{
	uint32_t ndump_tmo = HWIO_INF(GEMNOC_GEMNOC_QNS_LLCC_POC_DBG_PARAMS_HIGH, NDUMPTMO);
	uint32_t next_dump_tmo_status = 0, i = 0, ext_id = 0;
	uint64_t addr_of_violation = 0;
	uint32_t stream_low[MAX_NDUMPTMO], stream_high[MAX_NDUMPTMO];

	/* Validate ndump_tmo against array bounds */
	if (ndump_tmo > MAX_NDUMPTMO) {
		ERROR("GEMNOC LLCC TMO: Invalid ndump_tmo value %u, clamping to %u\n",
		      ndump_tmo, MAX_NDUMPTMO);
		ndump_tmo = MAX_NDUMPTMO;
	}

	ERROR("GEMNOC LLCC TMO POC_DBG Reg Dump\n");
	ERROR("DumpBrief_Low = 0x%08x\n",
	      HWIO_IN(GEMNOC_GEMNOC_QNS_LLCC_POC_DBG_DUMPBRIEF_LOW));

	while (i < ndump_tmo) {
		next_dump_tmo_status =
			HWIO_IN(GEMNOC_GEMNOC_QNS_LLCC_POC_DBG_DUMPTMOSTATUS_LOW);
		while (next_dump_tmo_status == 0) {
			next_dump_tmo_status =
				HWIO_IN(GEMNOC_GEMNOC_QNS_LLCC_POC_DBG_DUMPTMOSTATUS_LOW);
		}
		ERROR("Turn: %u, NextDumpTmoStatus is %u\n", i, next_dump_tmo_status);
		stream_low[i] = HWIO_IN(GEMNOC_GEMNOC_QNS_LLCC_POC_DBG_DUMPTMOSTREAM_LOW);
		stream_high[i] = HWIO_IN(GEMNOC_GEMNOC_QNS_LLCC_POC_DBG_DUMPTMOSTREAM_HIGH);
		ERROR("DumpTmoStream%u_Low = 0x%08x\n", i, stream_low[i]);
		ERROR("DumpTmoStream%u_High = 0x%08x\n", i, stream_high[i]);
		i++;
	}

	noc_decode_info.initiator_type = ((stream_low[0] & 0x100) >> 8);

	if (noc_decode_info.initiator_type) {
		ext_id = ((stream_high[2] >> 9) & 0xFFFF);

		noc_decode_info.bid = (((ext_id) >> 13) & (0x7)); /* 13-15 bits*/
		noc_decode_info.pid = (((ext_id) >> 8) & (0x1F)); /* 8-12 bits*/
		noc_decode_info.mid = ((ext_id) & (0xFF));    /* 0-7 bits*/

		ERROR("GEMNOC LLCC TMO: ERROR Decode : BID = 0x%x, PID = 0x%x, MID = 0x%x\n",
		      noc_decode_info.bid,
		      noc_decode_info.pid,
		      noc_decode_info.mid);
	} else {
		noc_decode_info.tgtid = ((stream_low[2] & 0xE000000) >> 25);
		noc_decode_info.srcid = ((stream_low[2] & 0x70000000) >> 28);
		noc_decode_info.lpid = ((stream_high[2] & 0xE0000000) >> 29) +
			       ((stream_low[3] & 0x3) << 3);

		ERROR("GEMNOC LLCC TMO: ERROR Decode : TGTID = 0x%x, SRCID = 0x%x, LPID = 0x%x\n",
		      noc_decode_info.tgtid,
		      noc_decode_info.srcid,
		      noc_decode_info.lpid);
	}

	addr_of_violation = ((stream_low[0] + ((uint64_t)stream_high[0] << 32)) >> 54) +
		      ((((stream_low[1] + ((uint64_t)stream_high[1] << 32)) & 0x3FFFFFF)) << 10);

	ERROR("GEMNOC LLCC TMO: Address of violation = 0x%016" PRIx64 "\n",
	      addr_of_violation);
}

void qti_noc_error_decode(nocerr_info_type *nocerr_info)
{
	uint32_t idx = 0;
	uint32_t curr_faultinstatus0_low = 0;

	if (nocerr_info == NULL)
		return;

	memset(&noc_decode_info, 0, sizeof(noc_decode_data_type));

	if (strcmp(nocerr_info->name, "GEM_NOC")) { /* if it's not GEMNOC, we need to use ERRLOG */
		noc_decode_id(nocerr_info->syndrome.ERRLOG1_HIGH,
			      nocerr_info->name);

		noc_decode_target_initiator(nocerr_info->syndrome.ERRLOG1_LOW,
					    nocerr_info->name);

		noc_decode_errcode(nocerr_info->syndrome.ERRLOG0_LOW,
				   nocerr_info->name);

		noc_decode_violation_addr(nocerr_info->syndrome.ERRLOG2_HIGH,
					  nocerr_info->syndrome.ERRLOG2_LOW,
					  nocerr_info->name);
	} else {
		/* if it's GEMNOC, we need to use POC */
		for (idx = 0; idx < nocerr_info->num_poc; idx++) {
			if (nocerr_info->syndrome.poc[idx].ERRLOGSTATUS_LOW) {
				/* poc_idx identified */
				noc_decode_info.target_index = idx;
				break;
			}
		}

		if (idx == nocerr_info->num_poc) {
			/* if poc idx is not valid */
			ERROR("ERRORLOGSTATUS_LOW not set for any of the 3 POCs in GEMNOC\n");

			/*
			 * If its GEMNOC Error & if valid bit is not set for any of the POCs,
			 * then its a GEMNOC TO Error, dump the POC_DBG Registers in this
			 * case to debug further.
			 *
			 * In MSM below POC_DBG Unit is dumped using DCC. For simplicity,
			 * dumping these reg directly here for now.
			 *
			 * Note: Only 1 SBM Unit is avail in GEMNOC, so using index 0
			 */

			curr_faultinstatus0_low =
				(nocerr_info->syndrome.sbms[0].FAULTINSTATUS0_LOW &
				 (~prev_faultinstatus0_low));

			if (curr_faultinstatus0_low &
			    HWIO_GEMNOC_GEMNOC_GEMNOC_FAULT_SBM_FAULTINSTATUS0_LOW_PORT6_BMSK)
				gemnoc_qns_pcnoc_tmo_poc_dbg_dump();
			if (curr_faultinstatus0_low &
			    HWIO_GEMNOC_GEMNOC_GEMNOC_FAULT_SBM_FAULTINSTATUS0_LOW_PORT5_BMSK)
				gemnoc_qns_pcie_tmo_poc_dbg_dump();
			if (curr_faultinstatus0_low &
			    HWIO_GEMNOC_GEMNOC_GEMNOC_FAULT_SBM_FAULTINSTATUS0_LOW_PORT4_BMSK)
				gemnoc_llcc_tmo_poc_dbg_dump();

			prev_faultinstatus0_low =
				nocerr_info->syndrome.sbms[0].FAULTINSTATUS0_LOW;
			return;
		}

		/* decode if its a valid poc idx */
		/* target info can be found using the POC IDX and its not captured separately */

		ERROR("Target = 0x%x - %s\n",
		      noc_decode_info.target_index,
		      gemnoc_target[noc_decode_info.target_index]);

		/* Initiator BID, PID, MID & Error Code details */
		noc_decode_id_errcode_gemnoc_poc(
			nocerr_info->syndrome.poc[noc_decode_info.target_index].ERRLOGMAIN_LOW,
			nocerr_info->syndrome.poc[noc_decode_info.target_index].ERRLOGMAIN_HIGH,
			nocerr_info->name);

		ERROR("Current access initiated from NS: %d\n",
		      ((nocerr_info->syndrome.poc[noc_decode_info.target_index].ERRLOGADDR_HIGH
		      & 0x40000000) >> 30));

		noc_decode_violation_addr(
			nocerr_info->syndrome.poc[noc_decode_info.target_index].ERRLOGADDR_HIGH
			& 0xFFFFFF, /* 0-23 bits */
			nocerr_info->syndrome.poc[noc_decode_info.target_index].ERRLOGADDR_LOW,
			nocerr_info->name);
	}
}
