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
 *
 * CRITICAL:
 * This function is called BEFORE plat_set_my_stack() in plat_reset_handler.
 * At this stage, no EL3 stack is available and SP is not yet initialized.
 *
 * Stack-based operations (stp/ldp) are NOT SAFE here.
 * The return address must be preserved across BL calls using a general-
 * purpose register.
 *
 * x19 is used intentionally for early-boot only. This is safe because:
 *  - execution is single-entry, non-reentrant
 *  - no ABI-preserving caller exists at this stage
 *  - control returns directly to early EL3 assembly
 *
 * Do NOT convert this to stack-based save/restore.
 *--------------------------------------------------------------------*/
.global qti_plat_per_core_llcc_init
.type qti_plat_per_core_llcc_init, %function
func qti_plat_per_core_llcc_init
	mov	x19, x30

	/* Configure ACTLR_EL3 - SMEN and TSIDEN bits */
	bl	qti_configure_actlr_el3

	/* Configure ACTLR_EL2 - TSIDEN bit */
	bl	qti_configure_actlr_el2

	ret	x19
endfunc qti_plat_per_core_llcc_init

/*--------------------------------------------------------------------
 * Configure ACTLR_EL3 for LLCC support
 * Sets SMEN, TSIDEN, and CLUSTERPMUEN bits to enable cluster control
 * register access
 *--------------------------------------------------------------------*/
.global qti_configure_actlr_el3
.type qti_configure_actlr_el3, %function
func qti_configure_actlr_el3
	mrs	x6, ACTLR_EL3
	orr	x6, x6, #ACTLR_TSIDEN_BIT
	orr	x6, x6, #ACTLR_SMEN_BIT
	orr	x6, x6, #ACTLR_CLUSTERPMUEN_BIT
	msr	ACTLR_EL3, x6
	isb
	ret
endfunc qti_configure_actlr_el3

/*--------------------------------------------------------------------
 * Configure ACTLR_EL2 for LLCC support
 * Sets TSIDEN and CLUSTERPMUEN bits to enable thread scheme ID
 * register access
 *--------------------------------------------------------------------*/
.global qti_configure_actlr_el2
.type qti_configure_actlr_el2, %function
func qti_configure_actlr_el2
	mrs	x6, ACTLR_EL2
	orr	x6, x6, #ACTLR_TSIDEN_BIT
	orr	x6, x6, #ACTLR_CLUSTERPMUEN_BIT
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
 * Regular cores use scheme ID 0, reserved core(last core) uses ID 1
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
