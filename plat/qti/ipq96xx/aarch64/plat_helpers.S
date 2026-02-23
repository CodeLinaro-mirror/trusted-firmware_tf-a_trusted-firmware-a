/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <arch.h>
#include <asm_macros.S>
#include <platform_def.h>

/*--------------------------------------------------------------------
 * Platform-specific per-core initialization for LLCC support
 * Configures ACTLR_EL3, ACTLR_EL2 on all cores
 * Called during both cold and warm boot on each core
 * Uses x7 (caller-saved register) - safe for all contexts
 *--------------------------------------------------------------------*/
.global qti_plat_per_core_init
.type qti_plat_per_core_init, %function
func qti_plat_per_core_init
	mov	x7, x30

	/* Configure ACTLR_EL3 - SMEN and TSIDEN bits */
	bl	qti_configure_actlr_el3

	/* Configure ACTLR_EL2 - TSIDEN bit */
	bl	qti_configure_actlr_el2

	ret	x7
endfunc qti_plat_per_core_init

/*--------------------------------------------------------------------
 * Configure ACTLR_EL3 for LLCC support
 * Sets SMEN and TSIDEN bits to enable cluster control register access
 *--------------------------------------------------------------------*/
.global qti_configure_actlr_el3
.type qti_configure_actlr_el3, %function
func qti_configure_actlr_el3
	mrs	x6, ACTLR_EL3
	orr	x6, x6, #ACTLR_TSIDEN_BIT
	orr	x6, x6, #ACTLR_SMEN_BIT
	msr	ACTLR_EL3, x6
	isb
	ret
endfunc qti_configure_actlr_el3

/*--------------------------------------------------------------------
 * Configure ACTLR_EL2 for LLCC support
 * Sets TSIDEN bit to enable thread scheme ID register access
 *--------------------------------------------------------------------*/
.global qti_configure_actlr_el2
.type qti_configure_actlr_el2, %function
func qti_configure_actlr_el2
	mrs	x6, ACTLR_EL2
	orr	x6, x6, #ACTLR_TSIDEN_BIT
	msr	ACTLR_EL2, x6
	isb
	ret
endfunc qti_configure_actlr_el2

/*--------------------------------------------------------------------
 * Configure CLUSTERBUSQOS_EL1 for LLCC support
 * Sets QoS values: Scheme ID 1=QoS2, Scheme ID 2=QoS1
 *--------------------------------------------------------------------*/
.global qti_configure_clusterbusqos
.type qti_configure_clusterbusqos, %function
func qti_configure_clusterbusqos
	mov	x6, xzr
	orr	x6, x6, #CLUSTERBUSQOS_SCID2_QOS1
	orr	x6, x6, #CLUSTERBUSQOS_SCID1_QOS2
	msr	s3_0_c15_c4_4, x6
	isb
	ret
endfunc qti_configure_clusterbusqos

/*--------------------------------------------------------------------
 * Configure CLUSTERTHREADSID_EL1 for non-secure world
 * Regular cores (0-3) use scheme ID 0, reserved core (4) uses ID 1
 *--------------------------------------------------------------------*/
.global qti_configure_clusterthreadsid_for_nsworld
.type qti_configure_clusterthreadsid_for_nsworld, %function
func qti_configure_clusterthreadsid_for_nsworld
	cmp	x0, 0x1
	beq	configure_reserved_core_scid

	mov	x6, xzr
	msr	s3_0_c15_c4_0, x6
	isb
	ret

configure_reserved_core_scid:
	mov	x6, xzr
	orr	x6, x6, #0x1
	msr	s3_0_c15_c4_0, x6
	isb
	ret
endfunc qti_configure_clusterthreadsid_for_nsworld

/*--------------------------------------------------------------------
 * Configure CLUSTERTHREADSID_EL1 for secure world
 * All cores use scheme ID 2
 *--------------------------------------------------------------------*/
.global qti_configure_clusterthreadsid_for_secureworld
.type qti_configure_clusterthreadsid_for_secureworld, %function
func qti_configure_clusterthreadsid_for_secureworld
	mov	x6, xzr
	orr	x6, x6, #0x2
	msr	s3_0_c15_c4_0, x6
	isb
	ret
endfunc qti_configure_clusterthreadsid_for_secureworld
