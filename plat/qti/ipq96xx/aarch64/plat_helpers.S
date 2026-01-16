/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <arch.h>
#include <asm_macros.S>

#include <platform_def.h>

/*--------------------------------------------------------------------
* Configure ACTLR_EL3.SMEN (Scheme Management Register write access control)
* during cold/warm boot of the ARM cores
*--------------------------------------------------------------------*/
	.global mon_configure_actlr_el3
	.type mon_configure_actlr_el3,%function
func mon_configure_actlr_el3
	/* Set ACTLR_EL3.SMEN to 1 */
	mrs	x0, ACTLR_EL3
	orr	x0, x0, #0x800
	msr	ACTLR_EL3, x0
	mrs	x0, ACTLR_EL3
	ret
endfunc mon_configure_actlr_el3

/*--------------------------------------------------------------------
* Configure ACTLR_EL2.SMEN (Scheme Management Register write access control)
* during cold/warm boot of the ARM cores
*--------------------------------------------------------------------*/
	.global mon_configure_actlr_el2
	.type mon_configure_actlr_el2,%function
func mon_configure_actlr_el2
	/* Set ACTLR_EL2.SMEN to 1 */
	mrs	x0, ACTLR_EL2
	orr	x0, x0, #0x800
	msr	ACTLR_EL2, x0
	mrs	x0, ACTLR_EL2
	ret
endfunc mon_configure_actlr_el2

/*--------------------------------------------------------------------
* Configure CLUSTERBUSQOS_EL1 as 0x120 during cold/warm boot of the ARM cores
*--------------------------------------------------------------------*/
	.global mon_configure_clusterbusqos
	.type mon_configure_clusterbusqos,%function
func mon_configure_clusterbusqos
	mov	x0, xzr	/* Iniliaize CHI bus QoS field for scheme IDs 0-7 as 0 */
	orr	x0, x0, #0x00000100  /* Set CHI bus QoS field for scheme ID 2 (Bits 4-7) as 1 */
	orr	x0, x0, #0x00000020  /* Set CHI bus QoS field for scheme ID 1 (Bits 0-3) as 2. */
	msr	s3_0_c15_c4_4, x0  /* Write x0 into CLUSTERBUSQOS_EL1. */
	mrs	x0, s3_0_c15_c4_4

	ret
endfunc mon_configure_clusterbusqos

/*--------------------------------------------------------------------
* When jumping to NS World (NS = 1),
* Configure CLUSTERTHREADSID_EL1 as 0x0 for cores 0,1,2,3 (USED_CPU_CORES).
* Configure CLUSTERTHREADSID_EL1 as 0x1 for core 4 (RESERVED_CPU_CORE).
* void mon_configure_clusterthreadsid_for_nsworld(is_reserved_cpu_core)
*--------------------------------------------------------------------*/
	.global mon_configure_clusterthreadsid_for_nsworld
	.type mon_configure_clusterthreadsid_for_nsworld,%function
func mon_configure_clusterthreadsid_for_nsworld
	cmp	x0, 0x1  /* check if curr cpu is reserved cpu core */
	beq	reserved_core_config_scid
	/* set scheme ID as 0x0 for this thread. scheme ID bits are 0-2,
	 * 3-31 is RAZ
	 */
	mov	x0, xzr
	msr	s3_0_c15_c4_0, x0  /* Write x0 into CLUSTERTHREADSID_EL1 */
	mrs	x0, s3_0_c15_c4_0
	ret
reserved_core_config_scid:
	mov	x0, xzr
	/* set scheme ID as 0x1 for this thread. scheme ID bits are 0-2,
	 * 3-31 is RAZ
	 */
	orr	x0, x0, #0x1
	msr	s3_0_c15_c4_0, x0  /* Write x0 into CLUSTERTHREADSID_EL1 */
	mrs	x0, s3_0_c15_c4_0
	ret
endfunc mon_configure_clusterthreadsid_for_nsworld

/*--------------------------------------------------------------------
* When entering to Secure World (NS = 0),
* Configure CLUSTERTHREADSID_EL1 as 0x2 for all cores.
* void mon_configure_clusterthreadsid_for_secureworld(void)
*--------------------------------------------------------------------*/
	.global mon_configure_clusterthreadsid_for_secureworld
	.type mon_configure_clusterthreadsid_for_secureworld,%function
func mon_configure_clusterthreadsid_for_secureworld
	mov	x6, xzr
	/* set scheme ID as 0x2 for this thread. scheme ID bits are 0-2,
	 * 3-31 is RAZ
	 */
	orr	x6, x6, #0x2
	msr	s3_0_c15_c4_0, x6  /* Write x6 into CLUSTERTHREADSID_EL1 */
	ret
endfunc mon_configure_clusterthreadsid_for_secureworld
