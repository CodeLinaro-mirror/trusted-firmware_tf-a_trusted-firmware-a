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
#include <common/debug.h>

/* Magic values for buffer state management */
#define DIAG_MAGIC		0x47414944 /* Buffer initialized and ready */
#define DIAG_MAGIC_FAILED	0xDEADBEEF /* Initialization failed */
#define DIAG_MAGIC_DLOAD	0xD15AB1ED /* DLOAD mode - logging disabled */

/*
 * struct diag - Main diagnostic region structure
 * @magic:       Magic identifier to validate the region
 * @ring_off:    Offset to the ring buffer from the start of the region
 * @ring_len:    Length of the ring buffer
 * @wrap:        Number of times the buffer has wrapped around
 * @offset:      Current offset in the buffer
 * @log_buf:     Flexible array for the actual log data
 */
struct diag {
	uint32_t magic;
	uint32_t ring_off;
	uint32_t ring_len;
	uint32_t wrap;
	uint32_t offset;
	uint8_t log_buf[];
};

extern uint8_t __DIAG_REGION_START__;
extern bool is_dload_magic_set(void);

/* Pointer to the diagnostics table in IMEM */
struct diag *diag_reg = (struct diag *)&__DIAG_REGION_START__;

static qti_console_diag_t qti_diag;

/*
 * diag_init() - Initialize the diagnostic log buffer
 *
 * Sets up a shared ring buffer for diagnostic logging. In download mode,
 * preserves existing logs by setting magic to DIAG_MAGIC_DLOAD. Otherwise,
 * always reinitializes the buffer for fresh boot logging.
 */
void diag_init(void)
{
	uint32_t **shared_imem_ptr = (uint32_t **)DIAG_BASE;

	*shared_imem_ptr = (uint32_t *)diag_reg;

	dsb();

	if (is_dload_magic_set()) {
		if (diag_reg->magic == DIAG_MAGIC) {
			NOTICE("DIAG LOG: DLOAD mode - disabling logging\n");
			diag_reg->magic = DIAG_MAGIC_DLOAD;
			dsb();
		}
		return;
	}

	/* Register console for this boot */
	qti_diag_register(&qti_diag, __DIAG_REGION_START__);
	console_set_scope(&qti_diag.console, qti_diag.console.flags | CONSOLE_FLAG_RUNTIME);

	/* Always reinitialize buffer in normal boot */
	memset(diag_reg, 0, DIAG_LEN);

	diag_reg->ring_off = offsetof(struct diag, log_buf);
	diag_reg->ring_len = (DIAG_LEN - diag_reg->ring_off) & (~0xF);

	if (DIAG_LEN <= diag_reg->ring_off || diag_reg->ring_len == 0) {
		diag_reg->magic = DIAG_MAGIC_FAILED;
		return;
	}

	diag_reg->magic = DIAG_MAGIC;

	NOTICE("DIAG LOG: Initialized (ring buffer: %u bytes)\n", diag_reg->ring_len);

	mmio_write_32(DIAG_LOG_START_INFO, (uint32_t)DIAG_BASE);
	mmio_write_32(DIAG_LOG_START_INFO + sizeof(uint32_t), DIAG_LEN);

	dsb();
}

/*
 * log_put() - Write a character to the diagnostic log buffer
 * @c: Character to write
 *
 * Writes character to shared ring buffer for external diagnostic tools.
 */
void log_put(const char c)
{
	uint32_t offset;

	if (diag_reg == NULL || diag_reg->magic != DIAG_MAGIC)
		return;

	offset = diag_reg->offset;

	diag_reg->log_buf[offset] = (uint8_t)c;

	offset++;
	if (offset >= diag_reg->ring_len) {
		offset = 0;
		if (diag_reg->wrap < UINT32_MAX)
			diag_reg->wrap++;
	}
	diag_reg->offset = offset;

	dsb();
}
