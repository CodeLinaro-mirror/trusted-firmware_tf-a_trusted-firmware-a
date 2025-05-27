/*
 * Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __QTISECLIB_DEFS_PLAT_H__
#define __QTISECLIB_DEFS_PLAT_H__

#define QTISECLIB_PLAT_CLUSTER_COUNT	1
#define QTISECLIB_PLAT_CORE_COUNT	4

/* Chipset specific secure interrupt number/ID defs. */
#define QTISECLIB_INT_ID_SEC_WDOG_BARK			(0x135)
#define QTISECLIB_INT_ID_NON_SEC_WDOG_BITE		(0x24)
#define QTISECLIB_INT_ID_RESET_SGI			(0xf)
#define QTISECLIB_INT_ID_CPU_WAKEUP_SGI			(0x8)

#define QTISECLIB_INT_ID_VMIDMT_ERR_CLT_SEC		(0x1A6)
#define QTISECLIB_INT_ID_VMIDMT_ERR_CLT_NONSEC		(0x1A7)
#define QTISECLIB_INT_ID_VMIDMT_ERR_CFG_SEC		(0x1A8)
#define QTISECLIB_INT_ID_VMIDMT_ERR_CFG_NONSEC		(0x1A9)

#define QTISECLIB_INT_ID_XPU_SEC			(0x1A5)
#define QTISECLIB_INT_ID_XPU_NON_SEC			(0x69)
#define QTISECLIB_INT_ID_XPU_MSA			(0x6A)

//NOC Interrupt
#define QTISECLIB_INT_ID_CONFIG_NOC_ERROR_1		(0x6E)
#define QTISECLIB_INT_ID_CONFIG_NOC_ERROR_2		(0x6F)
#define QTISECLIB_INT_ID_GEM_NOC_ERROR		        (0x62)
#define QTISECLIB_INT_ID_SYSTEM_NOC_ERROR		(0xDD)
#define QTISECLIB_INT_ID_AGGR_NOC_ERROR			(0xDE)
#define QTISECLIB_INT_ID_NSS_NOC_ERROR			(0xE0)
#define QTISECLIB_INT_ID_TME_IPC                        (0xEA)

#define QTISECLIB_INT_ID_XPU_VIOLATION                  (0x1A5)

#define	QTISECLIB_INT_INVALID_INT_NUM			(0xFFFFFFFFU)

#endif /* __QTISECLIB_DEFS_PLAT_H__ */
