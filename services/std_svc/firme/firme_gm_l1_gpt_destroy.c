/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <arch_features.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/extensions/rme.h>
#include <services/firme_svc.h>

#include "firme_gm_private.h"

static int firme_gm_l1_homogeneous_gpi(const uint64_t *table, uint8_t *gpi)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	bool have_gpi = false;
	uint8_t common_gpi = 0U;

	if ((table == NULL) || (gpi == NULL)) {
		return -EINVAL;
	}

	for (uint64_t i = 0U; i < geometry->l1_entry_count; i++) {
		uint64_t descriptor = table[i];

		if ((descriptor & GPT_L1_TYPE_CONT_DESC_MASK) ==
		    GPT_L1_TYPE_CONT_DESC) {
			uint64_t contig = GPT_L1_CONT_CONTIG(descriptor);
			uint8_t entry_gpi = (uint8_t)GPT_L1_CONT_GPI(descriptor);

			if ((contig < GPT_L1_CONTIG_2MB) ||
			    (contig > GPT_L1_CONTIG_512MB) ||
			    !firme_gm_is_gpi_valid(entry_gpi)) {
				return -EACCES;
			}

			if (!have_gpi) {
				common_gpi = entry_gpi;
				have_gpi = true;
			} else if (entry_gpi != common_gpi) {
				return -EACCES;
			}
			continue;
		}

		for (unsigned int nibble = 0U; nibble < 16U; nibble++) {
			uint8_t entry_gpi = (uint8_t)
				((descriptor >> (nibble * 4U)) &
				 GPT_L1_GRAN_DESC_GPI_MASK);

			if (!firme_gm_is_gpi_valid(entry_gpi)) {
				return -EACCES;
			}

			if (!have_gpi) {
				common_gpi = entry_gpi;
				have_gpi = true;
			} else if (entry_gpi != common_gpi) {
				return -EACCES;
			}
		}
	}

	if (!have_gpi) {
		return -EACCES;
	}

	*gpi = common_gpi;
	return 0;
}

static void firme_gm_scrub_l1_object(
	uint64_t l1_base, const struct firme_gm_mapping *mapping)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	uint64_t root_pa = l1_base |
		((uint64_t)GPT_NSE_ROOT << GPT_NSE_SHIFT);

	memset((void *)mapping->va, 0, mapping->size);
	flush_dcache_range(mapping->va, mapping->size);
	dsboshst();
	if (is_feat_mte2_supported()) {
		flush_dcache_to_popa_range_mte2(root_pa,
					       geometry->l1_table_size);
	} else {
		flush_dcache_to_popa_range(root_pa, geometry->l1_table_size);
	}
	dsbosh();
}

int firme_gm_l1_gpt_destroy(uint64_t par_base, uint64_t *l1_base)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	struct firme_gm_reservation reservation = {
		.slot = FIRME_GM_INVALID_RESERVATION
	};
	struct firme_gm_mapping mapping = { 0 };
	struct firme_gm_l1_donor_ranges donor_ranges;
	struct firme_gm_range target;
	uint64_t descriptor;
	uint64_t revalidated_descriptor;
	uint64_t transitioned = 0U;
	uint64_t donor;
	uint8_t gpi;
	int rc;
	int status;

	if (l1_base == NULL) {
		return FIRME_INVALID_PARAMETERS;
	}
	*l1_base = 0U;
	status = firme_gm_l1_prepare_target(par_base, &target);
	if (status != FIRME_SUCCESS) {
		return status;
	}

	rc = firme_gm_reserve_ranges(&target, 1U, &reservation);
	if (rc != 0) {
		return firme_errno_from_generic_errno(rc);
	}

	rc = firme_gm_l0_read(par_base, &descriptor);
	if (rc != 0) {
		status = FIRME_INVALID_PARAMETERS;
		goto out;
	}
	if (GPT_L0_TYPE(descriptor) != GPT_L0_TYPE_TBL_DESC) {
		status = FIRME_NOT_FOUND;
		goto out;
	}

	donor = (uint64_t)(uintptr_t)GPT_L0_TBLD_ADDR(descriptor);
	rc = firme_gm_l1_prepare_donor(donor, &donor_ranges);
	if (rc != 0) {
		status = FIRME_INVALID_PARAMETERS;
		goto out;
	}
	if (firme_gm_ranges_overlap(&target, &donor_ranges.object)) {
		status = FIRME_DENIED;
		goto out;
	}

	rc = firme_gm_reservation_extend(&reservation,
					 &donor_ranges.reservation);
	if (rc != 0) {
		status = firme_errno_from_generic_errno(rc);
		goto out;
	}

	rc = firme_gm_l0_read(par_base, &revalidated_descriptor);
	if ((rc != 0) || (revalidated_descriptor != descriptor)) {
		status = FIRME_OP_CONFLICT;
		goto out;
	}

	rc = firme_gm_map_root(donor, geometry->l1_table_size, &mapping);
	if (rc != 0) {
		status = ((rc == -EPERM) || (rc == -EACCES)) ?
			 FIRME_DENIED : FIRME_BUSY;
		goto out;
	}

	rc = firme_gm_l1_homogeneous_gpi((const uint64_t *)mapping.va, &gpi);
	if (rc != 0) {
		status = FIRME_DENIED;
		goto out;
	}

	rc = firme_gm_gpi_range_has_state(donor,
					  geometry->l1_granule_count,
					  GPT_GPI_ROOT);
	if (rc != 0) {
		status = FIRME_DENIED;
		goto out;
	}

	rc = firme_gm_l0_publish(par_base, descriptor, GPT_L0_BLK_DESC(gpi));
	if (rc != 0) {
		status = firme_errno_from_generic_errno(rc);
		goto out;
	}

	firme_gm_scrub_l1_object(donor, &mapping);
	firme_gm_unmap_root(&mapping);
	rc = firme_gm_gpi_set_reserved(donor,
				       geometry->l1_granule_count,
				       GPT_GPI_NS, &transitioned);
	if ((rc != 0) || (transitioned != geometry->l1_granule_count)) {
		ERROR("FIRME: L1 GPT donor release failed after detach\n");
		panic();
	}

	*l1_base = donor;
	status = FIRME_SUCCESS;

out:
	if (mapping.size != 0U) {
		firme_gm_unmap_root(&mapping);
	}
	firme_gm_release_reservation(&reservation);
	return status;
}
