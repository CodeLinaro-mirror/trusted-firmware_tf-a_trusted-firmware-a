/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <lib/mmio.h>
#include <qti_rng.h>

/*
 * qti_rng_get_data() - Get random data for stack canary generation
 *
 * Reads random data from IMEM populated by XBL at boot time. Uses a simple
 * one-time read approach optimized for stack canary generation.
 *
 * Algorithm:
 * - First call: Read from IMEM, use static fallback if all zeros
 * - Mark IMEM as consumed if valid data was used
 * - Subsequent calls: Always return static fallback
 *
 * @out:     Output buffer for random data
 * @out_len: Number of bytes requested (4 or 8 bytes only)
 *
 * Return: 0 on success, -1 on invalid parameters
 */
int qti_rng_get_data(uint8_t *out, uint32_t out_len)
{
	static bool first_call = true;
	uint32_t i;
	uint64_t static_canary = 0xCAFEBABEDEADD00DULL;
	uint8_t *static_bytes = (uint8_t *)&static_canary;
	uint32_t consumed_marker = 0xDEADD00D;
	bool all_zeros = true;

	/* Validate input parameters */
	if (out == NULL || (out_len != 4 && out_len != 8))
		return -1;

	/* First call: attempt to read from IMEM */
	if (first_call) {
		first_call = false;

		/* Read requested bytes from IMEM */
		for (i = 0; i < out_len; i++) {
			out[i] = mmio_read_8(QTI_IMEM_RANDOM_BASE_ADDR + i);

			/* Check for non-zero data */
			if (out[i] != 0)
				all_zeros = false;
		}

		/* If IMEM contains valid data, mark as consumed and return */
		if (!all_zeros) {
			mmio_write_32(QTI_IMEM_RANDOM_BASE_ADDR,
				      consumed_marker);
			return 0;
		}
	}

	/* Use static fallback for zero IMEM or subsequent calls */
	for (i = 0; i < out_len; i++)
		out[i] = static_bytes[i];

	return 0;
}
