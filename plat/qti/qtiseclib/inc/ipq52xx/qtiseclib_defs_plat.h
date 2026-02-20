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

#define QTISECLIB_PLAT_CLUSTER_COUNT			PLAT_CLUSTER_COUNT
#define QTISECLIB_PLAT_CORE_COUNT			PLATFORM_CORE_COUNT

#define BL31_BASE					0x8A600000
#define BL31_SIZE					0x00080000

/* Chipset specific secure interrupt number/ID defs. */
#define QTISECLIB_INT_ID_SEC_WDOG_BARK			(0x23)
#define QTISECLIB_INT_ID_NON_SEC_WDOG_BITE		(0x136)
#define QTISECLIB_INT_ID_RESET_SGI			(0xf)
#define QTISECLIB_INT_ID_CPU_WAKEUP_SGI			(0x8)
#define QTISECLIB_INT_ID_TME_IPC			(0x64)
#define QTISECLIB_INT_ID_XPU_VIOLATION			(0xB1)

//NOC Interrupt
#define QTISECLIB_INT_ID_C1_NOC_ERROR			(0x3B)
#define QTISECLIB_INT_ID_C2_NOC_ERROR			(0x3C)
#define QTISECLIB_INT_ID_MEMNOC_ERROR			(0x3A)
#define QTISECLIB_INT_ID_SNOC_ERROR			(0x9D)
#define QTISECLIB_INT_ID_NSS_NOC_ERROR			(0xF2)

#define	QTISECLIB_INT_INVALID_INT_NUM			(0xFFFFFFFFU)

#endif /* __QTISECLIB_DEFS_PLAT_H__ */
