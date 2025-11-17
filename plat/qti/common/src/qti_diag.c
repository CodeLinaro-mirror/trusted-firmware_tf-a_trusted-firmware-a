/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <string.h>
#include <stdint.h>
#include <platform_def.h>
#include <qti_diag_console.h>
#include <qtiseclib_interface.h>
#include <stdbool.h>
#include <lib/mmio.h>
#include <arch.h>

/**
 * struct diag - Main diagnostic region structure
 * @magic:       Magic identifier to validate the region
 * @in_use:      Flag indicating if the region is currently in use
 * @ring_off:    Offset to the ring buffer from the start of the region
 * @ring_len:    Length of the ring buffer
 * @wrap:        Number of times the buffer has wrapped around
 * @offset:      Current offset in the buffer
 * @log_buf:     Flexible array for the actual log data
 */
struct diag {
	uint32_t magic;
	uint32_t in_use;
	uint32_t ring_off;
	uint32_t ring_len;
	uint32_t wrap;
	uint32_t offset;
	uint8_t log_buf[];
};

extern uint8_t __DIAG_REGION_START__;
extern bool is_dload_magic_set(void);

/* Define a magic number for initialization check */
#define DIAG_MAGIC_NUMBER	0x47414944  /* "DIAG" */
#define DIAG_MAGIC_FAILED	0xDEADBEEF

/* Pointer to the diagnostics table in IMEM */
struct diag *diag_reg = (struct diag *)&__DIAG_REGION_START__;

#if DIAG_LOG
static qti_console_diag_t qti_diag;
#endif

void diag_init(void)
{
	uint32_t **shared_imem_ptr = (uint32_t **)DIAG_BASE;
	*shared_imem_ptr = (uint32_t *)diag_reg;

	if (is_dload_magic_set()) {
		return;
	}

#if DIAG_LOG
	qti_diag_register(&qti_diag, __DIAG_REGION_START__);
	/* Mark DIAG LOG as runtime usable */
	console_set_scope(&qti_diag.console, qti_diag.console.flags | CONSOLE_FLAG_RUNTIME);
#endif

	/* Critical memory barrier added here */
	dsb();

	if (diag_reg->magic == DIAG_MAGIC_NUMBER || diag_reg->in_use == 1) {
		return;
	}

	/* Zeroing out Diag region before initialization */
	memset(diag_reg, 0, DIAG_LEN);

	/* Set in_use flag early during initialization to prevent race
	 * conditions
	 */
	diag_reg->in_use = 1;
	diag_reg->ring_off = offsetof(struct diag, log_buf);
	diag_reg->ring_len = (DIAG_LEN - diag_reg->ring_off) & (~0xF);

	/* Initialize wrap and offset for the ring buffer */
	diag_reg->wrap = 0;
	diag_reg->offset = 0;

	/* Ensure we have a usable buffer */
	if (DIAG_LEN <= diag_reg->ring_off || diag_reg->ring_len == 0) {
		diag_reg->magic = DIAG_MAGIC_FAILED;
		diag_reg->in_use = 0;
		return;
	}

	/* Set magic last - indicates buffer is ready, and clear in_use */
	diag_reg->magic = DIAG_MAGIC_NUMBER;
	diag_reg->in_use = 0;

	mmio_write_32(DIAG_LOG_START_INFO, (uint32_t)DIAG_BASE);
	mmio_write_32(DIAG_LOG_START_INFO + sizeof(uint32_t), DIAG_LEN);

	/* Memory barrier to ensure all initialization is visible to other cores
	 * and hardware
	 */
	dsb();
}

void log_put(const char c)
{
	/* Skip logging in download mode to preserve previous logs for crashdump
	 */
	if (is_dload_magic_set())
		return;

	/* Ensure diag_reg is valid before writing */
	if (diag_reg == NULL) {
		return;
	}

	/* Initialize if not ready, skip if failed */
	if (diag_reg->magic != DIAG_MAGIC_NUMBER) {
		/* Skip if initialization previously failed */
		if (diag_reg->magic == DIAG_MAGIC_FAILED)
			return;

		diag_init();
		/* Skip if initialization just failed */
		if (diag_reg->magic != DIAG_MAGIC_NUMBER) {
			return;
		}
	}

	/* Write the character to the buffer */
	if (diag_reg->offset < diag_reg->ring_len &&
	    diag_reg->ring_len > 0 &&
	    diag_reg->ring_off + diag_reg->offset < DIAG_LEN) {

		diag_reg->log_buf[diag_reg->offset] = (uint8_t)c;

		/* Increment offset and check for wrap-around */
		diag_reg->offset++;
		if (diag_reg->offset >= diag_reg->ring_len) {
			diag_reg->offset = 0;
			if (diag_reg->wrap < UINT32_MAX) {
				diag_reg->wrap++;
			}
		}
	}
}
