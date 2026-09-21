/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FIRME_GM_PRIVATE_H
#define FIRME_GM_PRIVATE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <lib/extensions/rme.h>

/* This value comes from read_gptbr_el3. */
#define GPT_L0BASE	((uintptr_t)((read_gptbr_el3() >> GPTBR_BADDR_SHIFT) & \
			 GPTBR_BADDR_MASK) << GPTBR_BADDR_VAL_SHIFT)

/*
 * Lookup table used by the GPI transition engine to associate GPIs with
 * descriptor encodings, PAS selector bits, and permitted transitions.
 */
typedef struct {
	uint64_t desc;
	uint8_t nse;
	uint8_t nse2;
	uint16_t policy[3];
} firme_gpi_config_t;

typedef struct {
	uint64_t gpt_l1_desc;
	uint64_t *gpt_l1_addr;
	unsigned int idx;
	unsigned int gpi_shift;
	unsigned int gpi;
} firme_gpi_info_t;

typedef struct {
	uint8_t pps_bits;
	uint8_t pgs_bits;
	uint8_t l1_cnt_2mb;
} firme_gpt_config_t;

struct firme_gpt_geometry {
	uintptr_t l0_base;
	uint64_t pps_size;
	uint64_t par_size;
	size_t granule_size;
	size_t l1_table_size;
	uint64_t l1_entry_count;
	uint64_t l1_granule_count;
	uint64_t par_granule_count;
	uint64_t l1_index_mask;
	uint8_t pps_encoding;
	uint8_t pgs_encoding;
	uint8_t l0gptsz_encoding;
	uint8_t pps_bits;
	uint8_t pgs_bits;
	uint8_t par_bits;
	uint16_t l1_cnt_2mb;
	bool l1_lifecycle_supported;
};

typedef void (*gpt_shatter_func)(uintptr_t base,
				 const firme_gpi_info_t *gpi_info,
				 uint64_t l1_desc);
typedef void (*gpt_tlbi_func)(uintptr_t base);

typedef struct {
	gpt_tlbi_func function;
	size_t mask;
} gpt_tlbi_lookup_t;

struct firme_gm_range {
	uint64_t base;
	uint64_t size;
};

struct firme_gm_l1_donor_ranges {
	struct firme_gm_range object;
	struct firme_gm_range reservation;
};

struct firme_gm_reservation {
	uint32_t slot;
};

#define FIRME_GM_INVALID_RESERVATION	UINT32_MAX

struct firme_gm_mapping {
	uintptr_t va;
	size_t size;
	bool dynamic;
};

int firme_gm_geometry_init(void);
const struct firme_gpt_geometry *firme_gm_get_geometry(void);
uint64_t firme_gm_geometry_feature_register(void);
bool firme_gm_l1_lifecycle_is_supported(void);
bool firme_gm_par_is_host_managed(uint64_t par_base);
bool firme_gm_range_end(uint64_t base, uint64_t size, uint64_t *end);
bool firme_gm_ranges_overlap(const struct firme_gm_range *a,
			     const struct firme_gm_range *b);
int firme_gm_validate_granule_range(uint64_t base, uint64_t count);
int firme_gm_validate_par(uint64_t par_base);
int firme_gm_validate_l1_object(uint64_t l1_base);
int firme_gm_transition_reservation_range(uint64_t base, uint64_t size,
					  struct firme_gm_range *range);

int firme_gm_reserve_ranges(const struct firme_gm_range *ranges,
			    size_t range_count,
			    struct firme_gm_reservation *reservation);
int firme_gm_reservation_extend(struct firme_gm_reservation *reservation,
				const struct firme_gm_range *range);
void firme_gm_release_reservation(struct firme_gm_reservation *reservation);

int firme_gm_map_root(uint64_t pa, size_t size,
			struct firme_gm_mapping *mapping);
void firme_gm_unmap_root(struct firme_gm_mapping *mapping);

void firme_gm_gpi_init(void);
int firme_gm_gpi_set(uint64_t base, uint64_t gcnt, uint64_t attrs,
		     uint64_t flags, uint64_t *gcnt_ret);
int firme_gm_gpi_set_reserved(uint64_t base, uint64_t gcnt,
			      uint8_t target_gpi, uint64_t *gcnt_ret);
int firme_gm_gpi_range_has_state(uint64_t base, uint64_t gcnt,
				 uint8_t expected_gpi);
bool firme_gm_is_gpi_valid(uint8_t gpi);

int firme_gm_l1_prepare_target(uint64_t par_base,
			       struct firme_gm_range *target);
int firme_gm_l1_prepare_donor(uint64_t l1_base,
			      struct firme_gm_l1_donor_ranges *donor);
int firme_gm_l0_read(uint64_t par_base, uint64_t *descriptor);
int firme_gm_l0_publish(uint64_t par_base, uint64_t expected,
			uint64_t replacement);

#endif /* FIRME_GM_PRIVATE_H */
