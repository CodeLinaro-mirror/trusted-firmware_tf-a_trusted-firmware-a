/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <stddef.h>
#include <stdint.h>
#include <lib/mmio.h>
#include <qti_rng.h>

/*
 * qti_rng_get_data - Read random data from IMEM with round-robin mechanism
 *
 * This function reads pre-generated random data from IMEM (Internal Memory)
 * where it has been stored by a previous boot stage (XBL/SBL).
 *
 * Implementation:
 * - Uses persistent static offset to maintain position across calls
 * - Reads 32-bit (4-byte) values from consecutive IMEM addresses
 * - Extracts individual bytes in little-endian order
 * - Automatically wraps around at 32-byte boundary
 * - Provides continuous round-robin reading pattern
 *
 * Example usage:
 * - First call (8 bytes): reads from offset 0-7, next call starts at 8
 * - Second call (8 bytes): reads from offset 8-15, next call starts at 16
 * - After 32 bytes consumed: wraps to offset 0 and continues
 *
 * @out: Output buffer to fill with random data
 * @out_len: Number of bytes to read (typically 8 for ARM64 canary)
 *
 * Return: 0 on success, -1 on error
 */
int qti_rng_get_data(uint8_t *out, uint32_t out_len)
{
	/*
	 * Persistent offset that maintains its value across function calls.
	 * This enables round-robin reading pattern through the 32-byte pool.
	 */
	static uint32_t imem_offset = 0;

	uint32_t tmp_random = 0;
	uint32_t bytes_left = out_len;
	int i = 0;

	/* Validate input parameters */
	if (NULL == out || 0 == out_len) {
		return -1;
	}

	/*
	 * Read random data from IMEM in 4-byte chunks with round-robin.
	 * The random data pool is 32 bytes (0-31), stored sequentially
	 * starting at QTI_IMEM_RANDOM_BASE_ADDR.
	 */
	do {
		uint32_t current_addr = QTI_IMEM_RANDOM_BASE_ADDR +
					(imem_offset & ~3U);

		/* Read 4 bytes from current IMEM address */
		tmp_random = mmio_read_32(current_addr);

		/*
		 * Extract individual bytes from the 32-bit value
		 * in little-endian order (LSB first).
		 * Increment offset per byte consumed to ensure accurate tracking.
		 */
		for (i = imem_offset & 3; i < 4; i++) {
			/* Extract byte i by right-shifting and masking */
			*out = (uint8_t)(tmp_random >> (8 * i));

			/* Advance output pointer */
			out++;
			bytes_left--;

			/* Increment offset per byte consumed */
			imem_offset++;

			/*
			 * Wraparound at 32-byte boundary.
			 * When we reach offset 32, reset to 0 to continue
			 * reading from the beginning of the random data pool.
			 */
			if (imem_offset >= 32) {
				imem_offset = 0;
			}

			/* Stop if we've filled the requested bytes */
			if (bytes_left == 0) {
				break;
			}
		}

	} while (bytes_left != 0);

	return 0;
}
