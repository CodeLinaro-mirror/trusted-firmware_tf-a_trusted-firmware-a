/*
 * Copyright (c) 2015-2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <arch.h>
#include <asm_macros.S>
#include <assert_macros.S>
#include <cpu_macros.S>
#include <plat_macros.S>
#include <qti_cpu.h>

#define CORTEX_A53_ECTLR_SMP_BIT	(U(1) << 6)
#define CORTEX_A53_ECTLR_EL1		S3_1_C15_C2_1

/* -------------------------------------------------
 * The CPU Ops reset function for Kryo-3 Gold
 * -------------------------------------------------
 */
func qti_a53_reset_func

	mov x19, x30
	bl a53_aarch64_sysini

	mov	x30, x19
	ret

endfunc qti_a53_reset_func

	/* ---------------------------------------------
	 * Disable L1 data cache and unified L2 cache
	 * ---------------------------------------------
	 */
func qti_a53_disable_dcache
	sysreg_bit_clear sctlr_el3, SCTLR_C_BIT
	isb
	ret
endfunc qti_a53_disable_dcache

	/* ---------------------------------------------
	 * Disable intra-cluster coherency
	 * ---------------------------------------------
	 */
func qti_a53_disable_smp
	sysreg_bit_clear CORTEX_A53_ECTLR_EL1, CORTEX_A53_ECTLR_SMP_BIT
	isb
	dsb	sy
	ret
endfunc qti_a53_disable_smp

/* ----------------------------------------------------
 * The CPU Ops core power down function for Kryo-3 Gold
 *
 * This A53/Kryo variant is not hardware-assisted-coherency (no DSU), so unlike
 * ipq96xx's cortex_a78/a55_core_pwr_dwn (which just arm CPUPWRCTLR and let the
 * DSU handle cache/coherency exit autonomously), software here must disable
 * the dcache, flush L1 by set/way, and explicitly leave SMP/coherency before
 * the SPM clamps/powers down the core. Mirrors upstream cortex_a53_core_pwr_dwn.
 * ----------------------------------------------------
 */
func qti_a53_core_pwr_dwn
	mov	x18, x30

	bl	qti_a53_disable_dcache

	mov	x0, #DCCISW
	bl	dcsw_op_level1

	mov	x30, x18
	b	qti_a53_disable_smp
endfunc qti_a53_core_pwr_dwn

/* -------------------------------------------------------
 * The CPU Ops cluster power down function for Kryo-3 Gold
 * -------------------------------------------------------
 */
func qti_a53_cluster_pwr_dwn
	mov	x18, x30

	bl	qti_a53_disable_dcache

	mov	x0, #DCCISW
	bl	dcsw_op_level1

	bl	plat_disable_acp

	mov	x0, #DCCISW
	bl	dcsw_op_level2

	mov	x30, x18
	b	qti_a53_disable_smp
endfunc qti_a53_cluster_pwr_dwn

func qti_a53_errata_report
	ret
endfunc qti_a53_errata_report

/* ---------------------------------------------
 * This function provides kryo3_gold specific
 * register information for crash reporting.
 * It needs to return with x6 pointing to
 * a list of register names in ascii and
 * x8 - x15 having values of registers to be
 * reported.
 * ---------------------------------------------
 */
.section .rodata.qti_a53_regs, "aS"
qti_a53_regs:  /* The ascii list of register names to be reported */
	.asciz	""

func qti_a53_cpu_reg_dump
	adr	x6, qti_a53_regs
	ret
endfunc qti_a53_cpu_reg_dump

declare_cpu_ops	qti_a53, QTI_A53_MIDR,	\
		qti_a53_reset_func,		\
		qti_a53_core_pwr_dwn,	\
		qti_a53_cluster_pwr_dwn
