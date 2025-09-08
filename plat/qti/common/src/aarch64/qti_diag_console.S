/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <asm_macros.S>
#include <drivers/console.h>
#include <console_macros.S>

	.globl	qti_diag_register

func qti_diag_register
	finish_console_register diag putc=1, getc=0, flush=0
endfunc qti_diag_register

func console_diag_putc
	stp     x29, x30, [sp,#-16]!
	mov     x29, sp
	bl log_put
	mov	w0, #0x0
	ldp     x29, x30, [sp],#16
	ret
endfunc console_diag_putc
