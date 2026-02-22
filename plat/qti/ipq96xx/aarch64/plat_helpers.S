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
 * Configures CLUSTERBUSQOS_EL1 on boot core (core 0) only
 * Called during both cold and warm boot on each core
 *--------------------------------------------------------------------*/
.global qti_plat_per_core_init
.type qti_plat_per_core_init, %function
func qti_plat_per_core_init
	mov	x19, x30

	/* Configure ACTLR_EL3 - SMEN and TSIDEN bits */
	bl	qti_configure_actlr_el3

	/* Configure ACTLR_EL2 - TSIDEN bit */
	bl	qti_configure_actlr_el2

	/*
	 * Configure CLUSTERBUSQOS_EL1 on boot core only.
	 * g_qti_bl31_cold_booted is 0 only during cold boot of core 0.
	 */
	ldr	w0, g_qti_bl31_cold_booted
	cbnz	w0, skip_busqos_config
	bl	qti_configure_clusterbusqos
skip_busqos_config:
	ret	x19
endfunc qti_plat_per_core_init

/*--------------------------------------------------------------------
 * Configure ACTLR_EL3 for LLCC support
 * - ACTLR_EL3.SMEN (bit 11): Enable S-EL1 write access to cluster control registers
 * - ACTLR_EL3.TSIDEN (bit 10): Enable thread scheme ID register access
 * Called during cold/warm boot of all ARM cores
 *--------------------------------------------------------------------*/
.global qti_configure_actlr_el3
.type qti_configure_actlr_el3, %function
func qti_configure_actlr_el3
	mrs	x0, ACTLR_EL3
	/* Set ACTLR_EL3.TSIDEN (bit 10) and ACTLR_EL3.SMEN (bit 11) */
	orr	x0, x0, #ACTLR_TSIDEN_BIT	/* TSIDEN bit */
	orr	x0, x0, #ACTLR_SMEN_BIT		/* SMEN bit */
	msr	ACTLR_EL3, x0
	ret
endfunc qti_configure_actlr_el3

/*--------------------------------------------------------------------
 * Configure ACTLR_EL2 for LLCC support
 * - ACTLR_EL2.TSIDEN (bit 10): Enable thread scheme ID register access
 * Called during cold/warm boot of all ARM cores
 *--------------------------------------------------------------------*/
.global qti_configure_actlr_el2
.type qti_configure_actlr_el2, %function
func qti_configure_actlr_el2
	mrs	x0, ACTLR_EL2
	/* Set ACTLR_EL2.TSIDEN (bit 10) for thread scheme ID access */
	orr	x0, x0, #ACTLR_TSIDEN_BIT
	msr	ACTLR_EL2, x0
	ret
endfunc qti_configure_actlr_el2

/*--------------------------------------------------------------------
 * Configure CLUSTERBUSQOS_EL1 for LLCC support
 * Sets QoS values for different scheme IDs
 * Final register value: 0x120
 *--------------------------------------------------------------------*/
.global qti_configure_clusterbusqos
.type qti_configure_clusterbusqos, %function
func qti_configure_clusterbusqos
	mov	x0, xzr				/* Initialize all QoS fields to 0 */
	orr	x0, x0, #CLUSTERBUSQOS_SCID2_QOS1	/* Scheme ID 2: QoS=1 (bits 8-11) */
	orr	x0, x0, #CLUSTERBUSQOS_SCID1_QOS2	/* Scheme ID 1: QoS=2 (bits 4-7) */
	msr	s3_0_c15_c4_4, x0		/* Write to CLUSTERBUSQOS_EL1 */
	ret
endfunc qti_configure_clusterbusqos

/*--------------------------------------------------------------------
 * Configure CLUSTERTHREADSID_EL1 for non-secure world context switch
 * Maps CPU cores to LLCC scheme IDs:
 * - Regular cores (0-3): Scheme ID 0
 * - Reserved core (4): Scheme ID 1
 * void qti_configure_clusterthreadsid_for_nsworld(is_reserved_cpu_core)
 *--------------------------------------------------------------------*/
.global qti_configure_clusterthreadsid_for_nsworld
.type qti_configure_clusterthreadsid_for_nsworld, %function
func qti_configure_clusterthreadsid_for_nsworld
	cmp	x0, 0x1			/* Check if reserved CPU core */
	beq	configure_reserved_core_scid

	/* Regular cores: Set scheme ID 0 (bits 0-2, others RAZ) */
	mov	x0, xzr
	msr	s3_0_c15_c4_0, x0	/* Write to CLUSTERTHREADSID_EL1 */
	ret

configure_reserved_core_scid:
	/* Reserved core: Set scheme ID 1 (bits 0-2, others RAZ) */
	mov	x0, xzr
	orr	x0, x0, #0x1
	msr	s3_0_c15_c4_0, x0	/* Write to CLUSTERTHREADSID_EL1 */
	ret
endfunc qti_configure_clusterthreadsid_for_nsworld

/*--------------------------------------------------------------------
 * Configure CLUSTERTHREADSID_EL1 for secure world context switch
 * All cores use scheme ID 2
 * void qti_configure_clusterthreadsid_for_secureworld(void)
 *--------------------------------------------------------------------*/
.global qti_configure_clusterthreadsid_for_secureworld
.type qti_configure_clusterthreadsid_for_secureworld, %function
func qti_configure_clusterthreadsid_for_secureworld
	mov	x0, xzr
	/* Set scheme ID 2 for secure world (bits 0-2, others RAZ) */
	orr	x0, x0, #0x2
	msr	s3_0_c15_c4_0, x0	/* Write x0 into CLUSTERTHREADSID_EL1 */
	ret
endfunc qti_configure_clusterthreadsid_for_secureworld
