/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "icb_error.h"
#include "noc_error.h"
#include "qtiseclib_interface.h"
#include "noc_error_platform.h"
#include "noc_error_target.h"
#include "string.h"
#include <common/debug.h>
#include <lib/spinlock.h>
#include <plat/common/platform.h>
#include <platform_def.h>

#define NOC_OUT8(addr, data)   (*((volatile uint8_t *)(addr)) = ((uint8_t)(data)))
#define NOC_OUT16(addr, data)  (*((volatile uint16_t *)(addr)) = ((uint16_t)(data)))
#define NOC_OUT32(addr, data)  (*((volatile uint32_t *)(addr)) = ((uint32_t)(data)))

#define NOC_OUTM32(addr, mask, data) NOC_OUT32(addr, (NOC_IN32(addr) & ~(mask)) | (data))

#define NOC_IN8(addr)   ((uint8_t)(*((volatile uint8_t *)(addr))))
#define NOC_IN16(addr)  ((uint16_t)(*((volatile uint16_t *)(addr))))
#define NOC_IN32(addr)  ((uint32_t)(*((volatile uint32_t *)(addr))))

#define NOC_REG_ADDR(base, offset)   (((uint8_t *)(base)) + (offset))

#define REGISTER_VALID(offs) ((offs) != REGISTER_NOT_APPLICABLE)

extern spinlock_t isr_log_sync_lock;

extern nocerr_config_info_type nocerr_config_info;
extern nocerr_config_info_type_oem nocerr_config_info_oem;
static nocerr_propdata_type *nocerr_propdata;
static nocerr_propdata_type_oem *nocerr_propdata_oem;
static nocerr_info_type *noc_info_list;
static nocerr_info_type_oem *noc_info_oem_list;

#ifdef ENABLE_NOC_DECODE
extern void qti_noc_error_decode(nocerr_info_type *nocerr_info);
#endif

/*
 * IPC fault notification - raises interrupt to HLOS
 */
static void noc_ipc_fault(void)
{
	VERBOSE("NOC IPC fault - raising interrupt %d to HLOS\n",
	     NOTIFY_KERNEL_IRQ);
	plat_ic_set_interrupt_pending(NOTIFY_KERNEL_IRQ);
}

/*
 * qti_noc_error_platform_get_propdata() - Get platform property data
 *
 * Return: Pointer to internal property data structure
 */
nocerr_propdata_type *qti_noc_error_platform_get_propdata(void)
{
	nocerr_propdata_type *noc_propdata_ptr = NULL;

	if (nocerr_config_info.num_configs > 0) {
		noc_propdata_ptr = &nocerr_config_info.configs[0];
	}

	return noc_propdata_ptr;
}

/*
 * qti_noc_error_platform_get_propdata_oem() - Get OEM property data
 *
 * Return: Pointer to OEM property data structure
 */
nocerr_propdata_type_oem *qti_noc_error_platform_get_propdata_oem(void)
{
	nocerr_propdata_type_oem *noc_propdata_oem_ptr = NULL;

	if (nocerr_config_info_oem.num_configs > 0) {
		noc_propdata_oem_ptr = &nocerr_config_info_oem.configs[0];
	}

	return noc_propdata_oem_ptr;
}

/*
 * is_qti_noc_error_isr_registered() - Check if ISR is already registered
 * @idx: noc_info_list array index for the NoC to be registered
 *
 * Return: true if previously registered, false otherwise
 */
bool is_qti_noc_error_isr_registered(uint32_t idx)
{
	bool registered = false;
	uintptr_t intr_vector = noc_info_list[idx].intr_vector;

	/*
	 * We use NO_INTERRUPT as a flag for no interrupt needed.
	 * Only look for previous registrations if this is something else.
	 */
	if (intr_vector != NO_INTERRUPT) {
		/*
		 * Search through the array looking for previous registrations
		 * with the same interrupt vector.
		 */
		for (uint32_t noc_idx = 0;
		     noc_idx < idx && noc_idx < nocerr_propdata->len;
		     noc_idx++) {
			if (noc_info_list[noc_idx].intr_vector == intr_vector) {
				registered = true;
				break;
			}
		}
	} else {
		/*
		 * No interrupt to register, so just tell the caller we've been
		 * registered already so they don't bother to do it.
		 */
		registered = true;
	}

	return registered;
}

/*
 * qti_noc_error_log_obs() - Log OBS registers
 * @noc_info: Pointer to the NoC to log
 */
void qti_noc_error_log_obs(nocerr_info_type *noc_info)
{
	if (REGISTER_VALID(noc_info->hw->errlog0_low))
		noc_info->syndrome.ERRLOG0_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog0_low));

	if (REGISTER_VALID(noc_info->hw->errlog0_high))
		noc_info->syndrome.ERRLOG0_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog0_high));

	if (REGISTER_VALID(noc_info->hw->errlog1_low))
		noc_info->syndrome.ERRLOG1_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog1_low));

	if (REGISTER_VALID(noc_info->hw->errlog1_high))
		noc_info->syndrome.ERRLOG1_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog1_high));

	if (REGISTER_VALID(noc_info->hw->errlog2_low))
		noc_info->syndrome.ERRLOG2_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog2_low));

	if (REGISTER_VALID(noc_info->hw->errlog2_high))
		noc_info->syndrome.ERRLOG2_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog2_high));

	if (REGISTER_VALID(noc_info->hw->errlog3_low))
		noc_info->syndrome.ERRLOG3_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog3_low));

	if (REGISTER_VALID(noc_info->hw->errlog3_high))
		noc_info->syndrome.ERRLOG3_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog3_high));

	if (REGISTER_VALID(noc_info->hw->errlog2_1_low))
		noc_info->syndrome.ERRLOG2_1_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog2_1_low));

	if (REGISTER_VALID(noc_info->hw->errlog2_1_high))
		noc_info->syndrome.ERRLOG2_1_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog2_1_high));

	if (REGISTER_VALID(noc_info->hw->errlog4_3_low))
		noc_info->syndrome.ERRLOG4_3_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog4_3_low));

	if (REGISTER_VALID(noc_info->hw->errlog4_3_high))
		noc_info->syndrome.ERRLOG4_3_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog4_3_high));

	if (REGISTER_VALID(noc_info->hw->errlog6_5_low))
		noc_info->syndrome.ERRLOG6_5_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog6_5_low));

	if (REGISTER_VALID(noc_info->hw->errlog6_5_high))
		noc_info->syndrome.ERRLOG6_5_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog6_5_high));

	if (REGISTER_VALID(noc_info->hw->errlog8_high))
		noc_info->syndrome.ERRLOG8_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
					      noc_info->hw->errlog8_high));

	ERROR("%x %s %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",
	      NOC_ERR_FATAL_SYNDROME_REG, noc_info->name,
	      noc_info->syndrome.ERRLOG0_LOW,
	      noc_info->syndrome.ERRLOG0_HIGH,
	      noc_info->syndrome.ERRLOG1_LOW,
	      noc_info->syndrome.ERRLOG1_HIGH,
	      noc_info->syndrome.ERRLOG2_LOW,
	      noc_info->syndrome.ERRLOG2_HIGH,
	      noc_info->syndrome.ERRLOG3_LOW,
	      noc_info->syndrome.ERRLOG3_HIGH,
	      noc_info->syndrome.ERRLOG2_1_LOW,
	      noc_info->syndrome.ERRLOG2_1_HIGH,
	      noc_info->syndrome.ERRLOG4_3_LOW,
	      noc_info->syndrome.ERRLOG4_3_HIGH,
	      noc_info->syndrome.ERRLOG6_5_LOW,
	      noc_info->syndrome.ERRLOG6_5_HIGH,
	      noc_info->syndrome.ERRLOG8_HIGH);
}

bool qti_noc_error_scan_faultin(uint32_t num_sbms,
				nocerr_sbm_syndrome_type *sbms,
				nocerr_sbm_info_type_oem *obs_mask)
{
	bool is_fault = true;
	uint32_t fault_mask = 0;

	/* Scan the sideband managers for the bits we're looking for */
	for (uint32_t idx = 0; idx < num_sbms; idx++) {
		fault_mask |= sbms[idx].FAULTINSTATUS0_LOW &
			      ~obs_mask[idx].faultin_en0_low;
		fault_mask |= sbms[idx].FAULTINSTATUS0_HIGH &
			      ~obs_mask[idx].faultin_en0_high;
		fault_mask |= sbms[idx].FAULTINSTATUS1_LOW &
			      ~obs_mask[idx].faultin_en1_low;
		fault_mask |= sbms[idx].FAULTINSTATUS1_HIGH &
			      ~obs_mask[idx].faultin_en1_high;
	}

	is_fault = fault_mask != 0;

	return is_fault;
}

/*
 * qti_noc_error_handle_filter() - Handle common filter code
 * @noc_info: NoC info structure for the current error
 * @noc_info_oem: NoC OEM info structure for the current error
 * @obs_err_valid: OBS error valid flag
 * @delay_crash: If fatal, delay crashing
 *
 * Return: true if error fatal is allowed, false otherwise
 */
bool qti_noc_error_handle_filter(nocerr_info_type *noc_info,
				 nocerr_info_type_oem *noc_info_oem,
				 bool obs_err_valid,
				 bool *delay_crash)
{
	bool is_fatal_allowed = true, is_fault = true;

	/* Avoid input schenanigans */
	if (nocerr_propdata == NULL ||
	    nocerr_propdata_oem == NULL ||
	    noc_info == NULL ||
	    noc_info_oem == NULL ||
	    delay_crash == NULL)
		return true;

	/* Set up default response. */
	*delay_crash = false;

	/* Make sure we have any filters first. */
	if (nocerr_propdata->filters != NULL &&
	    nocerr_propdata->num_filters != 0) {
		/* Check for non-obs faults, these are always fatal. */
		is_fault = qti_noc_error_scan_faultin(noc_info->num_sbms,
						      noc_info->syndrome.sbms,
						      noc_info_oem->obs_mask);

		/* Now process obs faults */
		if (obs_err_valid) {
			bool matched = false;
			/* ignore MID */
			uint32_t extid = noc_info->syndrome.ERRLOG1_HIGH & 0xFFFFFF00;
			uint32_t errcode = (noc_info->syndrome.ERRLOG0_LOW >> 8) & 0x7;
			nocerr_filter_type *filter = NULL;
			nocerr_filter_type_oem *oem_filter = NULL;

			/* See if we can find a matching filter. */
			for (uint32_t fidx = 0;
			     fidx < nocerr_propdata->num_filters;
			     fidx++) {
				filter = &nocerr_propdata->filters[fidx];
				oem_filter = &nocerr_propdata_oem->filters[fidx];

				/* Check the list of supported extids for this filter */
				for (uint32_t extidx = 0;
				     extidx < filter->num_extids;
				     extidx++) {
					if (extid == filter->extids[extidx]) {
						matched = true;
						break;
					}
				}

				/* If we have a match, check to see if we have an action to take. */
				if (matched) {
					/* Should we mask a fatal error? */
					if (oem_filter->enable && filter->non_fatal) {
						uint32_t erridx;

						for (erridx = 0;
						     erridx < filter->num_errcodes;
						     erridx++) {
							/*
							 * If we have an errcode match,
							 * prevent a fatal error.
							 */
							if (errcode ==
							    filter->errcodes[erridx]) {
								is_fatal_allowed = false;
								break;
							}
						}
					}
					/*
					 * If we're still going to error fatal,
					 * check to see if delay_crash is allowed.
					 */
					if (is_fatal_allowed)
						*delay_crash = filter->delay_fatal &&
							       oem_filter->delay_fatal;

					/*
					 * We had an extid match,
					 * so no further filters to look at.
					 */
					break;
				}
			}
		}
	}

	return is_fatal_allowed || is_fault;
}

/*
 * qti_noc_error_log_safety() - Log Safety registers
 * @noc_info: Pointer to the NoC to log
 * @idx: Safety controller index
 */
void qti_noc_error_log_safety(nocerr_info_type *noc_info,
			      uint32_t idx)
{
	if (REGISTER_VALID(noc_info->sfty_ctl_hw[idx]->cflta_low))
		noc_info->syndrome.sfty_ctl[idx].CFLTA_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->sfty_ctl_addrs[idx],
					      noc_info->sfty_ctl_hw[idx]->cflta_low));

	if (REGISTER_VALID(noc_info->sfty_ctl_hw[idx]->cflta_high))
		noc_info->syndrome.sfty_ctl[idx].CFLTA_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->sfty_ctl_addrs[idx],
					      noc_info->sfty_ctl_hw[idx]->cflta_high));

	if (REGISTER_VALID(noc_info->sfty_ctl_hw[idx]->uflta_low))
		noc_info->syndrome.sfty_ctl[idx].UFLTA_LOW =
			NOC_IN32(NOC_REG_ADDR(noc_info->sfty_ctl_addrs[idx],
					      noc_info->sfty_ctl_hw[idx]->uflta_low));

	if (REGISTER_VALID(noc_info->sfty_ctl_hw[idx]->uflta_high))
		noc_info->syndrome.sfty_ctl[idx].UFLTA_HIGH =
			NOC_IN32(NOC_REG_ADDR(noc_info->sfty_ctl_addrs[idx],
					      noc_info->sfty_ctl_hw[idx]->uflta_high));

	ERROR("%x %s %x %x %x %x %x\n",
	      NOC_SFTY_NAME_SYNDROME_REG, noc_info->name, idx,
	      noc_info->syndrome.sfty_ctl[idx].CFLTA_LOW,
	      noc_info->syndrome.sfty_ctl[idx].CFLTA_HIGH,
	      noc_info->syndrome.sfty_ctl[idx].UFLTA_LOW,
	      noc_info->syndrome.sfty_ctl[idx].UFLTA_HIGH);
}

/*
 * qti_noc_error_handle_interrupt() - Handle NOC error interrupt
 * @intnum: Interrupt number
 * @handle: Handle
 * @arg: Context data for client
 *
 * Return: arg
 */
void *qti_noc_error_handle_interrupt(uint32_t intnum, void *handle, void *arg)
{
	uint32_t noc_idx, idx, val = 0u;
	uint32_t intr_vector = (uintptr_t)arg;
	nocerr_info_type *noc_info = NULL;
	nocerr_info_type_oem *noc_info_oem = NULL;
	bool fatal_fault_detected = false;
	bool delay_fatal = false;
	bool noc_timeout = false;

	spin_lock(&isr_log_sync_lock);

	/* Validate global data structures early */
	if (nocerr_propdata == NULL || noc_info_list == NULL || noc_info_oem_list == NULL) {
		ERROR("NOC error handler not properly initialized\n");
		spin_unlock(&isr_log_sync_lock);
		return arg;
	}

	/*
	 * Find which NOC generated interrupt
	 * Potentially multiple on a single IRQ.
	 */
	for (noc_idx = 0; noc_idx < nocerr_propdata->len; noc_idx++) {
		bool fault_detected = false;
		bool target_delay_fatal = false;
		bool filter_delay_fatal = true;
		bool obs_err_valid = false;

		if (noc_info_list[noc_idx].intr_vector == intr_vector) {
			noc_info = &noc_info_list[noc_idx];
			noc_info_oem = &noc_info_oem_list[noc_idx];
		} else if (!strcmp(noc_info_list[noc_idx].name, "CONFIG_NOC") &&
			   (noc_info_list[noc_idx].intr_vector + 1) == intr_vector) {
			/* CNOC split across 2 interrupts since there are many ports in IPQ96xx */
			noc_info = &noc_info_list[noc_idx];
			noc_info_oem = &noc_info_oem_list[noc_idx];
		} else {
			/* No match, go on to the next. */
			continue;
		}

		/* Validate pointers before use */
		if (noc_info == NULL || noc_info_oem == NULL) {
			continue;
		}

		if (noc_info->base_addr != NULL) {
			/* Log OBS block syndrome registers if fault detected */
			if (NOC_IN32(NOC_REG_ADDR(noc_info->base_addr,
						  noc_info->hw->err_valid_low))) {
				fault_detected = true;
				obs_err_valid = true;
				qti_noc_error_log_obs(noc_info);
			}

			/* Check for safety controller errors, if present. */
			for (idx = 0; idx < noc_info_list[noc_idx].num_sfty_ctl; idx++) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info_list[noc_idx].sfty_ctl_addrs[idx],
					noc_info_list[noc_idx].sfty_ctl_hw[idx]->status_low));
				noc_info->syndrome.sfty_ctl[idx].STATUS_LOW = val;
				/* Only crash for uncorrectable faults */
				if ((val & 0x2) != 0)
					fault_detected = true;

				qti_noc_error_log_safety(noc_info, idx);
			}
		}

		/* Log Sideband Manager syndrome info if fault detected */
		for (idx = 0; idx < noc_info->num_sbms; idx++) {
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status0_low)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status0_low));
				noc_info->syndrome.sbms[idx].FAULTINSTATUS0_LOW = val;
				if (val != 0) {
					/* Timeout detection for low register */
					if (val & ~(noc_info_oem->obs_mask[idx].faultin_en0_low))
						noc_timeout = true;
					fault_detected = true;
				}
			}
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status0_high)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status0_high));
				noc_info->syndrome.sbms[idx].FAULTINSTATUS0_HIGH = val;
				if (val != 0) {
					/* Timeout detection for high register */
					if (val & ~(noc_info_oem->obs_mask[idx].faultin_en0_high))
						noc_timeout = true;
					fault_detected = true;
				}
			}
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status1_low)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status1_low));
				noc_info->syndrome.sbms[idx].FAULTINSTATUS1_LOW = val;
				if (val != 0) {
					/* Timeout detection for status1_low */
					if (val & ~(noc_info_oem->obs_mask[idx].faultin_en1_low))
						noc_timeout = true;
					fault_detected = true;
				}
			}
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status1_high)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status1_high));
				noc_info->syndrome.sbms[idx].FAULTINSTATUS1_HIGH = val;
				if (val != 0) {
					/* Timeout detection for status1_high */
					if (val & ~(noc_info_oem->obs_mask[idx].faultin_en1_high))
						noc_timeout = true;
					fault_detected = true;
				}
			}

			if (fault_detected) {
				ERROR("%x %s %x %x %x %x %x\n", NOC_FAULT_NAME_SBMS,
				      noc_info->name, idx,
				      noc_info->syndrome.sbms[idx].FAULTINSTATUS0_LOW,
				      noc_info->syndrome.sbms[idx].FAULTINSTATUS0_HIGH,
				      noc_info->syndrome.sbms[idx].FAULTINSTATUS1_LOW,
				      noc_info->syndrome.sbms[idx].FAULTINSTATUS1_HIGH);
			}
		}

		/* Log Point-of-Serialization faults */
		for (idx = 0; idx < noc_info->num_pos; idx++) {
			if (REGISTER_VALID(noc_info->pos_hw[idx]->errlog_low)) {
				val = NOC_IN32(NOC_REG_ADDR(noc_info->pos_base_addrs[idx],
							    noc_info->pos_hw[idx]->errlog_low));
				noc_info->syndrome.pos[idx].ERRLOG_LOW = val;

				if ((val & 0x2) != 0) {
					fault_detected = true;
				} else {
					continue;
				}
			}
			if (REGISTER_VALID(noc_info->pos_hw[idx]->errlog_high))
				noc_info->syndrome.pos[idx].ERRLOG_HIGH =
					NOC_IN32(NOC_REG_ADDR(noc_info->pos_base_addrs[idx],
							      noc_info->pos_hw[idx]->errlog_high));

			ERROR("%x %s %x %x %x\n", NOC_POS_NAME_SYNDROME_REG,
			      noc_info->name, idx,
			      noc_info->syndrome.pos[idx].ERRLOG_LOW,
			      noc_info->syndrome.pos[idx].ERRLOG_HIGH);
		}

		for (idx = 0; idx < noc_info->num_poc; idx++) {
			if (REGISTER_VALID(noc_info->poc_hw[idx]->errstatus_low)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errstatus_low));

				if ((val & 0x1) != 0) {
					noc_info->syndrome.poc[idx].ERRLOGSTATUS_LOW = val;
					fault_detected = true;
				} else {
					continue;
				}
			}
			if (REGISTER_VALID(noc_info->poc_hw[idx]->errlogmain_low)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errlogmain_low));
				noc_info->syndrome.poc[idx].ERRLOGMAIN_LOW = val;
			}
			if (REGISTER_VALID(noc_info->poc_hw[idx]->errlogmain_high)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errlogmain_high));
				noc_info->syndrome.poc[idx].ERRLOGMAIN_HIGH = val;
			}

			if (REGISTER_VALID(noc_info->poc_hw[idx]->errlogaddr_low)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errlogaddr_low));
				noc_info->syndrome.poc[idx].ERRLOGADDR_LOW = val;
			}
			if (REGISTER_VALID(noc_info->poc_hw[idx]->errlogaddr_high)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errlogaddr_high));
				noc_info->syndrome.poc[idx].ERRLOGADDR_HIGH = val;
			}

			if (REGISTER_VALID(noc_info->poc_hw[idx]->errloguser_low)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errloguser_low));
				noc_info->syndrome.poc[idx].ERRLOGUSER_LOW = val;
			}
			if (REGISTER_VALID(noc_info->poc_hw[idx]->errloguser_high)) {
				val = NOC_IN32(NOC_REG_ADDR(
					noc_info->poc_base_addrs[idx],
					noc_info->poc_hw[idx]->errloguser_high));
				noc_info->syndrome.poc[idx].ERRLOGUSER_HIGH = val;
			}

			ERROR("%x %s %x %x %x %x %x %x %x %x\n",
			      NOC_POC_NAME_SYNDROME_REG, noc_info->name, idx,
			      noc_info->syndrome.poc[idx].ERRLOGSTATUS_LOW,
			      noc_info->syndrome.poc[idx].ERRLOGMAIN_LOW,
			      noc_info->syndrome.poc[idx].ERRLOGMAIN_HIGH,
			      noc_info->syndrome.poc[idx].ERRLOGADDR_LOW,
			      noc_info->syndrome.poc[idx].ERRLOGADDR_HIGH,
			      noc_info->syndrome.poc[idx].ERRLOGUSER_LOW,
			      noc_info->syndrome.poc[idx].ERRLOGUSER_HIGH);
		}

#ifdef ENABLE_NOC_DECODE
		qti_noc_error_decode(noc_info);
#endif

		if (noc_info->base_addr != NULL) {
			NOC_OUT32(NOC_REG_ADDR(noc_info->base_addr,
					       noc_info->hw->err_clear_low), 0x1);
		}

		for (idx = 0; idx < noc_info->num_pos; idx++) {
			if (REGISTER_VALID(noc_info->pos_hw[idx]->errlogclr_low))
				NOC_OUT32(NOC_REG_ADDR(noc_info->pos_base_addrs[idx],
						       noc_info->pos_hw[idx]->errlogclr_low),
					  0x1);
		}

		for (idx = 0; idx < noc_info->num_poc; idx++) {
			if (REGISTER_VALID(noc_info->poc_hw[idx]->errack_low))
				NOC_OUT32(NOC_REG_ADDR(noc_info->poc_base_addrs[idx],
						       noc_info->poc_hw[idx]->errack_low),
					  0x1);
		}

		for (idx = 0; idx < noc_info->num_sbms; idx++) {
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status0_low)) {
				val = NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status0_low));
				NOC_OUT32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en0_low),
					(NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en0_low)) &
					(~val | noc_info_oem->obs_mask[idx].faultin_en0_low)));
			}
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status0_high)) {
				val = NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status0_high));

				NOC_OUT32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en0_high),
					(NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en0_high)) &
					(~val | noc_info_oem->obs_mask[idx].faultin_en0_high)));
			}
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status1_low)) {
				val = NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status1_low));

				NOC_OUT32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en1_low),
					(NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en1_low)) &
					(~val |
					noc_info_oem->obs_mask[idx].faultin_en1_low)));
			}
			if (REGISTER_VALID(noc_info->sb_hw[idx]->faultin_status1_high)) {
				val = NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_status1_high));

				NOC_OUT32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en1_high),
					(NOC_IN32(NOC_REG_ADDR(noc_info->sb_base_addrs[idx],
					noc_info->sb_hw[idx]->faultin_en1_high)) &
					(~val |
					noc_info_oem->obs_mask[idx].faultin_en1_high)));
			}
		}

		if (fault_detected && noc_info_oem->error_fatal) {
			if (qti_noc_error_handle_filter(noc_info, noc_info_oem,
							obs_err_valid,
							&filter_delay_fatal) &&
			    qti_noc_error_handle_target(noc_info, noc_info_oem,
							&target_delay_fatal)) {
				fatal_fault_detected = true;
				delay_fatal = delay_fatal || filter_delay_fatal ||
					      target_delay_fatal;
			}
		}
	}
	if (fatal_fault_detected) {
		ERROR("Fatal NOC error detected!\n");
		dbg_err_fatal(DBG_ERR_FATAL_NOC_ERROR);
	} else if (noc_timeout) {
		ERROR("NOC timeout detected!\n");
		dbg_err_fatal(DBG_ERR_FATAL_NOC_ERROR);
	} else if (noc_info != NULL && noc_info_oem != NULL) {
		noc_ipc_fault();
	} else {
		ERROR("NOC Invalid Interrupt Vector!\n");
	}
	spin_unlock(&isr_log_sync_lock);

	return arg;
}

/*
 * qti_icb_error_init() - Performs initialization for NOC Error Handler
 *
 * It enables interrupts required to handle NOC errors.
 *
 * Return: None
 */
void qti_icb_error_init(void)
{
	uint32_t i, idx;

	nocerr_propdata = qti_noc_error_platform_get_propdata();
	if (nocerr_propdata == NULL)
		return;

	nocerr_propdata_oem = qti_noc_error_platform_get_propdata_oem();
	if (nocerr_propdata_oem == NULL)
		return;

	noc_info_list = nocerr_propdata->noc_info_list;
	if (noc_info_list == NULL)
		return;

	noc_info_oem_list = nocerr_propdata_oem->noc_info_oem_list;
	if (noc_info_oem_list == NULL)
		return;

	qti_noc_error_init_target(noc_info_list, nocerr_propdata->len, noc_info_oem_list);

	for (i = 0; i < nocerr_propdata->num_clock_regs; i++)
		NOC_OUT32(nocerr_propdata->clock_reg_addrs[i],
			  nocerr_propdata_oem->clock_reg_vals[i]);

	for (i = 0; i < nocerr_propdata->len; i++) {
		if (!is_qti_noc_error_isr_registered(i)) {
			if (int_svc_register_isr(noc_info_list[i].intr_vector,
						 noc_info_list[i].name,
						 sizeof(noc_info_list[i].name),
						 qti_noc_error_handle_interrupt,
						 (void *)((uint64_t)
						 (noc_info_list[i].intr_vector)))
						 != 0) {
				ERROR("NOC init error !\n");
				return;
			}

			if (!strcmp(noc_info_list[i].name, "CONFIG_NOC")) {
				if (int_svc_register_isr(noc_info_list[i].intr_vector + 1,
							 noc_info_list[i].name,
							 sizeof(noc_info_list[i].name),
							 qti_noc_error_handle_interrupt,
							 (void *)((uint64_t)
							 (noc_info_list[i].intr_vector + 1)))
							 != 0) {
					ERROR("NOC init error !\n");
					return;
				}
			}
		}

		if (noc_info_oem_list[i].intr_enable) {
			for (idx = 0; idx < noc_info_list[i].num_sbms; idx++) {
				if (REGISTER_VALID(noc_info_list[i].sb_hw[idx]->faultin_en0_low)) {
					/* AGGR NOC sbm register access via Secure IO IPC */
					NOC_OUT32(NOC_REG_ADDR(
						noc_info_list[i].sb_base_addrs[idx],
						noc_info_list[i].sb_hw[idx]->faultin_en0_low),
						noc_info_oem_list[i].sbms[idx].faultin_en0_low);
				}
				if (REGISTER_VALID(noc_info_list[i].sb_hw[idx]->faultin_en0_high))
					NOC_OUT32(NOC_REG_ADDR(
						noc_info_list[i].sb_base_addrs[idx],
						noc_info_list[i].sb_hw[idx]->faultin_en0_high),
						noc_info_oem_list[i].sbms[idx].faultin_en0_high);

				if (REGISTER_VALID(noc_info_list[i].sb_hw[idx]->faultin_en1_low))
					NOC_OUT32(NOC_REG_ADDR(
						noc_info_list[i].sb_base_addrs[idx],
						noc_info_list[i].sb_hw[idx]->faultin_en1_low),
						noc_info_oem_list[i].sbms[idx].faultin_en1_low);

				if (REGISTER_VALID(noc_info_list[i].sb_hw[idx]->faultin_en1_high))
					NOC_OUT32(NOC_REG_ADDR(
						noc_info_list[i].sb_base_addrs[idx],
						noc_info_list[i].sb_hw[idx]->faultin_en1_high),
						noc_info_oem_list[i].sbms[idx].faultin_en1_high);
			}

			for (idx = 0; idx < noc_info_list[i].num_pos; idx++) {
				if (REGISTER_VALID(noc_info_list[i].pos_hw[idx]->errlog_low))
					NOC_OUT32(NOC_REG_ADDR(
						noc_info_list[i].pos_base_addrs[idx],
						noc_info_list[i].pos_hw[idx]->errlog_low),
						noc_info_oem_list[i].pos[idx].enable ? 1 : 0);
			}

			for (idx = 0; idx < noc_info_list[i].num_poc; idx++) {
				if (REGISTER_VALID(noc_info_list[i].poc_hw[idx]->errset_low)) {
					uint32_t mask = 0x1;

					NOC_OUTM32(NOC_REG_ADDR(
						noc_info_list[i].poc_base_addrs[idx],
						noc_info_list[i].poc_hw[idx]->errset_low), mask,
						(noc_info_oem_list[i].poc[idx].enable ? 1 : 0));
				}
			}

			for (idx = 0; idx < noc_info_list[i].num_tos; idx++)
				NOC_OUT32(noc_info_list[i].to_addrs[idx],
					  noc_info_oem_list[i].to_reg_vals[idx]);

			for (idx = 0; idx < noc_info_list[i].num_sfty_ctl; idx++) {
				if (REGISTER_VALID(noc_info_list[i].sfty_ctl_hw[idx]->outen_low))
					NOC_OUT32(NOC_REG_ADDR(
						noc_info_list[i].sfty_ctl_addrs[idx],
						noc_info_list[i].sfty_ctl_hw[idx]->outen_low),
						noc_info_oem_list[i].sfty_ctl[idx].outen_low);
			}

			if (noc_info_list[i].base_addr != NULL) {
				NOC_OUT32(NOC_REG_ADDR(noc_info_list[i].base_addr,
						       noc_info_list[i].hw->main_ctl_low),
					  0x3);
			}
		}
	}
}
