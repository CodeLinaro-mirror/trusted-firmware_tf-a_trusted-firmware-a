/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef QTI_RNG_H
#define QTI_RNG_H

#include <stdint.h>

/*
 * IMEM Address for random data that is stored by SBL
 */
#define QTI_IMEM_RANDOM_BASE_ADDR	0x8600900

/*
 * qti_rng_get_data - Get random data from IMEM with round-robin mechanism
 *
 * @out: Pointer to output buffer where random data will be written
 * @out_len: Number of random bytes to read (typically 8 bytes for ARM64)
 *
 * This function reads pre-generated random data from IMEM (Internal Memory)
 * using a persistent round-robin mechanism. The random data is expected to
 * be stored at QTI_IMEM_RANDOM_BASE_ADDR by SBL).
 *
 * Round-robin behavior:
 * - Maintains persistent offset across function calls
 * - 32-byte random data pool (offsets 0-31)
 * - Automatically wraps around at 32-byte boundary
 * - Each call continues from where the previous call ended
 *
 * Example sequence:
 * - Call 1 (8 bytes): reads offsets 0-7, next starts at 8
 * - Call 2 (8 bytes): reads offsets 8-15, next starts at 16
 * - Call 3 (8 bytes): reads offsets 16-23, next starts at 24
 * - Call 4 (8 bytes): reads offsets 24-31, next wraps to 0
 * - Call 5 (12 bytes): reads offsets 0-11, next starts at 12
 *
 * Return: 0 on success, -1 on error (NULL pointer or zero length)
 */
int qti_rng_get_data(uint8_t *out, uint32_t out_len);

#endif /* QTI_RNG_H */
