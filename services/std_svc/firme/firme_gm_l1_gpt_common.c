/*
 * Copyright (c) 2026, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include <arch_helpers.h>
#include <lib/extensions/rme.h>
#include <plat/common/common_def.h>
#include <services/firme_svc.h>

#include "firme_gm_private.h"

int firme_gm_l1_prepare_target(uint64_t par_base,
			       struct firme_gm_range *target)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();

	if (!firme_gm_l1_lifecycle_is_supported()) {
		return FIRME_NOT_SUPPORTED;
	}
	if ((target == NULL) || (firme_gm_validate_par(par_base) != 0)) {
		return FIRME_INVALID_PARAMETERS;
	}
	if (!firme_gm_par_is_host_managed(par_base)) {
		return FIRME_DENIED;
	}

	*target = (struct firme_gm_range) {
		.base = par_base,
		.size = geometry->par_size,
	};

	return FIRME_SUCCESS;
}

int firme_gm_l1_prepare_donor(uint64_t l1_base,
			      struct firme_gm_l1_donor_ranges *donor)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();

	if ((donor == NULL) ||
	    (firme_gm_validate_l1_object(l1_base) != 0)) {
		return -EINVAL;
	}

	donor->object = (struct firme_gm_range) {
		.base = l1_base,
		.size = geometry->l1_table_size,
	};

	return firme_gm_transition_reservation_range(
		l1_base, geometry->l1_table_size, &donor->reservation);
}

int firme_gm_l0_read(uint64_t par_base, uint64_t *descriptor)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	uint64_t *l0;
	uint64_t index;

	if ((descriptor == NULL) || (firme_gm_validate_par(par_base) != 0)) {
		return -EINVAL;
	}

	index = par_base >> geometry->par_bits;
	if (index >= (geometry->pps_size / geometry->par_size)) {
		return -EINVAL;
	}

	l0 = (uint64_t *)geometry->l0_base;
	*descriptor = __atomic_load_n(&l0[index], __ATOMIC_ACQUIRE);
	return 0;
}

int firme_gm_l0_publish(uint64_t par_base, uint64_t expected,
			uint64_t replacement)
{
	const struct firme_gpt_geometry *geometry = firme_gm_get_geometry();
	uint64_t *descriptor;
	u_register_t tlbi_size;

	if ((firme_gm_validate_par(par_base) != 0) ||
	    ((GPT_L0_TYPE(expected) != GPT_L0_TYPE_BLK_DESC) &&
	     (GPT_L0_TYPE(expected) != GPT_L0_TYPE_TBL_DESC)) ||
	    ((GPT_L0_TYPE(replacement) != GPT_L0_TYPE_BLK_DESC) &&
	     (GPT_L0_TYPE(replacement) != GPT_L0_TYPE_TBL_DESC))) {
		return -EINVAL;
	}

	switch (geometry->par_size) {
	case SZ_1G:
		tlbi_size = TLBI_SZ_1G;
		break;
	case (SZ_1G << 4):
		tlbi_size = TLBI_SZ_16G;
		break;
	case (SZ_1G << 6):
		tlbi_size = TLBI_SZ_64G;
		break;
	case (SZ_1G << 9):
		tlbi_size = TLBI_SZ_512G;
		break;
	default:
		return -EINVAL;
	}

	descriptor = &((uint64_t *)geometry->l0_base)
		[par_base >> geometry->par_bits];
	if (__atomic_load_n(descriptor, __ATOMIC_ACQUIRE) != expected) {
		return -ESTALE;
	}

	__atomic_store_n(descriptor, replacement, __ATOMIC_RELEASE);
	flush_dcache_range((uintptr_t)descriptor, sizeof(*descriptor));
	dsboshst();
	TLBIRPAOS(par_base, tlbi_size);
	dsbosh();

	return 0;
}
