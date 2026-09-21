/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/extensions/rme.h>
#include <lib/utils_def.h>
#include <services/firme_svc.h>

#include "firme_gm_private.h"

int firme_gm_l1_gpt_create(uint64_t par_base, uint64_t l1_base)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	struct firme_gm_reservation reservation = {
		.slot = FIRME_GM_INVALID_RESERVATION
	};
	struct firme_gm_mapping mapping = { 0 };
	struct firme_gm_l1_donor_ranges donor;
	struct firme_gm_range ranges[2];
	uint64_t descriptor;
	uint64_t transitioned = 0U;
	uint8_t source_gpi;
	bool donor_is_root = false;
	int rc;
	int status;

	status = firme_gm_l1_prepare_target(par_base, &ranges[0]);
	if (status != FIRME_SUCCESS) {
		return status;
	}

	rc = firme_gm_l1_prepare_donor(l1_base, &donor);
	if (rc != 0) {
		return FIRME_INVALID_PARAMETERS;
	}
	if (firme_gm_ranges_overlap(&ranges[0], &donor.object)) {
		return FIRME_NOT_FOUND;
	}

	ranges[1] = donor.reservation;
	rc = firme_gm_reserve_ranges(ranges, ARRAY_SIZE(ranges),
				     &reservation);
	if (rc != 0) {
		return firme_errno_from_generic_errno(rc);
	}

	rc = firme_gm_l0_read(par_base, &descriptor);
	if (rc != 0) {
		status = FIRME_INVALID_PARAMETERS;
		goto out;
	}
	if (GPT_L0_TYPE(descriptor) == GPT_L0_TYPE_TBL_DESC) {
		status = FIRME_ALREADY_EXISTS;
		goto out;
	}
	if (GPT_L0_TYPE(descriptor) != GPT_L0_TYPE_BLK_DESC) {
		status = FIRME_INVALID_PARAMETERS;
		goto out;
	}

	source_gpi = (uint8_t)GPT_L0_BLKD_GPI(descriptor);
	if (!firme_gm_is_gpi_valid(source_gpi)) {
		status = FIRME_INVALID_PARAMETERS;
		goto out;
	}

	rc = firme_gm_gpi_range_has_state(l1_base,
					  geometry->l1_granule_count,
					  GPT_GPI_NS);
	if (rc != 0) {
		status = (rc == -ENOENT) ? FIRME_NOT_FOUND :
			 firme_errno_from_generic_errno(rc);
		goto out;
	}

	rc = firme_gm_map_root(l1_base, geometry->l1_table_size, &mapping);
	if (rc != 0) {
		status = FIRME_NO_MEMORY;
		goto out;
	}

	rc = firme_gm_gpi_set_reserved(l1_base,
				       geometry->l1_granule_count,
				       GPT_GPI_ROOT, &transitioned);
	if (rc != 0) {
		if (transitioned != 0U) {
			uint64_t rolled_back = 0U;

			if (firme_gm_gpi_set_reserved(l1_base, transitioned,
						      GPT_GPI_NS,
						      &rolled_back) != 0 ||
			    rolled_back != transitioned) {
				panic();
			}
		}
		status = firme_errno_from_generic_errno(rc);
		goto out;
	}
	donor_is_root = true;

	for (uint64_t i = 0U; i < geometry->l1_entry_count; i++) {
		((uint64_t *)mapping.va)[i] = GPT_BUILD_L1_DESC(source_gpi);
	}
	flush_dcache_range(mapping.va, mapping.size);
	dsboshst();
	firme_gm_unmap_root(&mapping);

	rc = firme_gm_l0_publish(par_base, descriptor,
				 GPT_L0_TBL_DESC(l1_base));
	if (rc != 0) {
		uint64_t rolled_back = 0U;

		if (firme_gm_gpi_set_reserved(l1_base,
					      geometry->l1_granule_count,
					      GPT_GPI_NS, &rolled_back) != 0 ||
		    rolled_back != geometry->l1_granule_count) {
			panic();
		}
		donor_is_root = false;
		status = firme_errno_from_generic_errno(rc);
		goto out;
	}

	donor_is_root = false;
	status = FIRME_SUCCESS;

out:
	if (mapping.size != 0U) {
		firme_gm_unmap_root(&mapping);
	}
	if (donor_is_root) {
		uint64_t rolled_back = 0U;

		if (firme_gm_gpi_set_reserved(l1_base,
					      geometry->l1_granule_count,
					      GPT_GPI_NS, &rolled_back) != 0 ||
		    rolled_back != geometry->l1_granule_count) {
			panic();
		}
	}
	firme_gm_release_reservation(&reservation);
	return status;
}
