/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <common/debug.h>
#include <lib/spinlock.h>
#include <lib/utils_def.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/firme_gpt_policy.h>

#include "firme_gm_private.h"

#define FIRME_GM_OPS_MAX		U(64)
#define FIRME_GM_RANGES_PER_OP		U(2)

struct firme_gm_operation_slot {
	bool active;
	size_t range_count;
	struct firme_gm_range ranges[FIRME_GM_RANGES_PER_OP];
};

static spinlock_t firme_gm_operations_lock;
static struct firme_gm_operation_slot firme_gm_operations[FIRME_GM_OPS_MAX];
static spinlock_t firme_gm_mapping_lock;

#pragma weak plat_firme_get_static_root_mapping
int plat_firme_get_static_root_mapping(uint64_t pa, size_t size,
				       uintptr_t *va)
{
	(void)pa;
	(void)size;
	(void)va;
	return -ENOENT;
}

static bool firme_gm_range_conflicts_locked(const struct firme_gm_range *range,
					    uint32_t ignored_slot)
{
	for (uint32_t i = 0U; i < FIRME_GM_OPS_MAX; i++) {
		if (!firme_gm_operations[i].active || (i == ignored_slot)) {
			continue;
		}

		for (size_t j = 0U;
		     j < firme_gm_operations[i].range_count; j++) {
			if (firme_gm_ranges_overlap(
				    range, &firme_gm_operations[i].ranges[j])) {
				return true;
			}
		}
	}

	return false;
}

int firme_gm_reserve_ranges(const struct firme_gm_range *ranges,
			    size_t range_count,
			    struct firme_gm_reservation *reservation)
{
	uint32_t free_slot = FIRME_GM_INVALID_RESERVATION;
	int rc = 0;

	if ((ranges == NULL) || (reservation == NULL) ||
	    (range_count == 0U) ||
	    (range_count > FIRME_GM_RANGES_PER_OP)) {
		return -EINVAL;
	}

	for (size_t i = 0U; i < range_count; i++) {
		uint64_t end;

		if (!firme_gm_range_end(ranges[i].base, ranges[i].size, &end)) {
			return -EINVAL;
		}
	}

	reservation->slot = FIRME_GM_INVALID_RESERVATION;
	spin_lock(&firme_gm_operations_lock);

	for (uint32_t i = 0U; i < FIRME_GM_OPS_MAX; i++) {
		if (!firme_gm_operations[i].active) {
			free_slot = i;
			break;
		}
	}

	if (free_slot == FIRME_GM_INVALID_RESERVATION) {
		rc = -EBUSY;
		goto out;
	}

	for (size_t i = 0U; i < range_count; i++) {
		if (firme_gm_range_conflicts_locked(
			    &ranges[i], FIRME_GM_INVALID_RESERVATION)) {
			rc = -EINPROGRESS;
			goto out;
		}
	}

	firme_gm_operations[free_slot].active = true;
	firme_gm_operations[free_slot].range_count = range_count;
	memcpy(firme_gm_operations[free_slot].ranges, ranges,
	       range_count * sizeof(ranges[0]));
	reservation->slot = free_slot;

out:
	spin_unlock(&firme_gm_operations_lock);
	return rc;
}

int firme_gm_reservation_extend(struct firme_gm_reservation *reservation,
				const struct firme_gm_range *range)
{
	struct firme_gm_operation_slot *operation;
	uint64_t end;
	int rc = 0;

	if ((reservation == NULL) || (range == NULL) ||
	    (reservation->slot >= FIRME_GM_OPS_MAX) ||
	    !firme_gm_range_end(range->base, range->size, &end)) {
		return -EINVAL;
	}

	spin_lock(&firme_gm_operations_lock);
	operation = &firme_gm_operations[reservation->slot];

	if (!operation->active ||
	    (operation->range_count >= FIRME_GM_RANGES_PER_OP)) {
		rc = -EINVAL;
		goto out;
	}

	if (firme_gm_range_conflicts_locked(range, reservation->slot)) {
		rc = -EINPROGRESS;
		goto out;
	}

	operation->ranges[operation->range_count++] = *range;

out:
	spin_unlock(&firme_gm_operations_lock);
	return rc;
}

void firme_gm_release_reservation(struct firme_gm_reservation *reservation)
{
	if ((reservation == NULL) ||
	    (reservation->slot == FIRME_GM_INVALID_RESERVATION)) {
		return;
	}

	spin_lock(&firme_gm_operations_lock);
	assert(reservation->slot < FIRME_GM_OPS_MAX);
	assert(firme_gm_operations[reservation->slot].active);
	memset(&firme_gm_operations[reservation->slot], 0,
	       sizeof(firme_gm_operations[reservation->slot]));
	spin_unlock(&firme_gm_operations_lock);

	reservation->slot = FIRME_GM_INVALID_RESERVATION;
}

int firme_gm_map_root(uint64_t pa, size_t size,
			struct firme_gm_mapping *mapping)
{
	int rc;

	if ((mapping == NULL) || (size == 0U)) {
		return -EINVAL;
	}

	mapping->va = 0U;
	mapping->size = 0U;
	mapping->dynamic = false;
	rc = plat_firme_get_static_root_mapping(pa, size, &mapping->va);
	if (rc == 0) {
		if (mapping->va == 0U) {
			return -EINVAL;
		}
		mapping->size = size;
		return 0;
	}
	if (rc != -ENOENT) {
		mapping->va = 0U;
		return rc;
	}
	mapping->va = 0U;

#if defined(PLAT_XLAT_TABLES_DYNAMIC) && PLAT_XLAT_TABLES_DYNAMIC
	spin_lock(&firme_gm_mapping_lock);
	rc = mmap_add_dynamic_region_alloc_va(
		pa, &mapping->va, size,
		MT_MEMORY | MT_RW | MT_ROOT | MT_EXECUTE_NEVER);
	spin_unlock(&firme_gm_mapping_lock);
	if (rc != 0) {
		return rc;
	}

	mapping->size = size;
	mapping->dynamic = true;
	return 0;
#else
	/* Preserve the legacy identity-mapped GPT access path. */
	mapping->va = (uintptr_t)pa;
	mapping->size = size;
	return 0;
#endif
}

void firme_gm_unmap_root(struct firme_gm_mapping *mapping)
{
	int rc;

	if ((mapping == NULL) || (mapping->size == 0U)) {
		return;
	}

#if defined(PLAT_XLAT_TABLES_DYNAMIC) && PLAT_XLAT_TABLES_DYNAMIC
	if (mapping->dynamic) {
		spin_lock(&firme_gm_mapping_lock);
		rc = mmap_remove_dynamic_region(mapping->va, mapping->size);
		spin_unlock(&firme_gm_mapping_lock);
		if (rc != 0) {
			ERROR("FIRME: failed to remove Root mapping (%d)\n", rc);
			panic();
		}
	}
#else
	(void)rc;
#endif

	mapping->va = 0U;
	mapping->size = 0U;
	mapping->dynamic = false;
}
