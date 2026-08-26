/*
 * Copyright (c) 2018-2021, The Linux Foundation. All rights reserved.
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

#define QTISECLIB_PLAT_CLUSTER_COUNT	PLAT_CLUSTER_COUNT
#define QTISECLIB_PLAT_CORE_COUNT	PLATFORM_CORE_COUNT

#define BL31_BASE						0x8A600000
#define BL31_SIZE						0x00080000

/* Chipset specific secure interrupt number/ID defs. */
#define QTISECLIB_INT_ID_SEC_WDOG_BARK			(0x36)
#define QTISECLIB_INT_ID_NON_SEC_WDOG_BITE		(0x21)

//NOC Interrupt
#define QTISECLIB_INT_ID_C1_NOC_ERROR			(0xBE)
#define QTISECLIB_INT_ID_C2_NOC_ERROR			(0xBF)
#define QTISECLIB_INT_ID_GEM_NOC_ERROR			(0x6A)
#define QTISECLIB_INT_ID_SNOC_ERROR			(0xC0)
#define QTISECLIB_INT_ID_AGGR_NOC_ERROR			(0xC3)
#define QTISECLIB_INT_ID_NSS_NOC_ERROR			(0xC9)

//TME-L IPC Interrupt
#define QTISECLIB_INT_ID_TME_IPC			(0xA3)

#endif /* __QTISECLIB_DEFS_PLAT_H__ */
