/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <drivers/console.h>

typedef struct {
	console_t console;
	uintptr_t base;
} qti_console_diag_t;

int qti_diag_register(qti_console_diag_t *console, uintptr_t diag_base_addr);

void diag_init(void);
