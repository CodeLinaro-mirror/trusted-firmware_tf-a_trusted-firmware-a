/*
 * Copyright (c) 2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018, 2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */
#include <assert.h>

#include <arch_helpers.h>
#include <bl31/bl31.h>
#include <common/debug.h>
#ifdef PLAT_ipq52xx
#include <drivers/arm/gicv2.h>
#endif
#include <drivers/delay_timer.h>
#include <lib/mmio.h>
#include <lib/psci/psci.h>

#include <platform.h>
#include <platform_def.h>
#include <qti_cpu.h>
#include <qti_plat.h>
#include <qtiseclib_cb_interface.h>
#include <qtiseclib_defs_plat.h>
#include <qtiseclib_interface.h>

#define QTI_LOCAL_PSTATE_WIDTH		4
#define QTI_LOCAL_PSTATE_MASK		((1 << QTI_LOCAL_PSTATE_WIDTH) - 1)

#if PSCI_OS_INIT_MODE
#define QTI_LAST_AT_PLVL_MASK		(QTI_LOCAL_PSTATE_MASK <<	\
					 (QTI_LOCAL_PSTATE_WIDTH *	\
					  (PLAT_MAX_PWR_LVL + 1)))
#endif

/* Make composite power state parameter till level 0 */
#define qti_make_pwrstate_lvl0(lvl0_state, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | ((type) << PSTATE_TYPE_SHIFT))

/* Make composite power state parameter till level 1 */
#define qti_make_pwrstate_lvl1(lvl1_state, lvl0_state, type) \
		(((lvl1_state) << QTI_LOCAL_PSTATE_WIDTH) | \
		qti_make_pwrstate_lvl0(lvl0_state, type))

/* Make composite power state parameter till level 2 */
#define qti_make_pwrstate_lvl2(lvl2_state, lvl1_state, lvl0_state, type) \
		(((lvl2_state) << (QTI_LOCAL_PSTATE_WIDTH * 2)) | \
		qti_make_pwrstate_lvl1(lvl1_state, lvl0_state, type))

/* Make composite power state parameter till level 3 */
#define qti_make_pwrstate_lvl3(lvl3_state, lvl2_state, lvl1_state, lvl0_state, type) \
		(((lvl3_state) << (QTI_LOCAL_PSTATE_WIDTH * 3)) | \
		qti_make_pwrstate_lvl2(lvl2_state, lvl1_state, lvl0_state, type))

/* QTI_CORE_PWRDN_EN_MASK happens to be same across all CPUs */
#define QTI_CORE_PWRDN_EN_MASK		1

#pragma weak qti_system_off
#pragma weak qti_system_reset
#pragma weak qti_system_reset2

/* Forward declarations for weak symbols */
__dead2 void qti_system_off(void);
__dead2 void qti_system_reset(void);
int qti_system_reset2(int is_vendor, int reset_type, u_register_t cookie);

/* cpu power control happens to be same across all CPUs */
DEFINE_RENAME_SYSREG_RW_FUNCS(cpu_pwrctrl_val, S3_0_C15_C2_7)

const unsigned int qti_pm_idle_states[] = {
	qti_make_pwrstate_lvl0(QTI_LOCAL_STATE_OFF,
			       PSTATE_TYPE_POWERDOWN),
	qti_make_pwrstate_lvl0(QTI_LOCAL_STATE_DEEPOFF,
			       PSTATE_TYPE_POWERDOWN),
	qti_make_pwrstate_lvl1(QTI_LOCAL_STATE_DEEPOFF,
			       QTI_LOCAL_STATE_DEEPOFF,
			       PSTATE_TYPE_POWERDOWN),
	qti_make_pwrstate_lvl2(QTI_LOCAL_STATE_OFF,
			       QTI_LOCAL_STATE_DEEPOFF,
			       QTI_LOCAL_STATE_DEEPOFF,
			       PSTATE_TYPE_POWERDOWN),
	qti_make_pwrstate_lvl3(QTI_LOCAL_STATE_OFF,
			       QTI_LOCAL_STATE_DEEPOFF,
			       QTI_LOCAL_STATE_DEEPOFF,
			       QTI_LOCAL_STATE_DEEPOFF,
			       PSTATE_TYPE_POWERDOWN),
	0,
};

/*******************************************************************************
 * QTI standard platform handler called to check the validity of the power
 * state parameter. The power state parameter has to be a composite power
 * state.
 ******************************************************************************/
int qti_validate_power_state(unsigned int power_state,
			     psci_power_state_t *req_state)
{
	unsigned int state_id;
	int i;

	assert(req_state);

	/*
	 *  Currently we are using a linear search for finding the matching
	 *  entry in the idle power state array. This can be made a binary
	 *  search if the number of entries justify the additional complexity.
	 */
	for (i = 0; !!qti_pm_idle_states[i]; i++) {
#if PSCI_OS_INIT_MODE
		if ((power_state & ~QTI_LAST_AT_PLVL_MASK) ==
		    qti_pm_idle_states[i])
#else
		if (power_state == qti_pm_idle_states[i])
#endif
			break;
	}

	/* Return error if entry not found in the idle state array */
	if (!qti_pm_idle_states[i])
		return PSCI_E_INVALID_PARAMS;

	i = 0;
	state_id = psci_get_pstate_id(power_state);

	/* Parse the State ID and populate the state info parameter */
	for (i = QTI_PWR_LVL0; i <= PLAT_MAX_PWR_LVL; i++) {
		req_state->pwr_domain_state[i] = state_id &
		    QTI_LOCAL_PSTATE_MASK;
		state_id >>= QTI_LOCAL_PSTATE_WIDTH;
	}
#if PSCI_OS_INIT_MODE
	req_state->last_at_pwrlvl = state_id & QTI_LOCAL_PSTATE_MASK;
#endif

	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * PLATFORM FUNCTIONS
 ******************************************************************************/

#ifndef PLAT_ipq52xx
static void qti_set_cpupwrctlr_val(void)
{
	unsigned long val;

	val = read_cpu_pwrctrl_val();
	val |= QTI_CORE_PWRDN_EN_MASK;
	write_cpu_pwrctrl_val(val);

	isb();
}
#endif

/**
 * CPU power on function - ideally we want a wrapper since this function is
 * target specific. But to unblock teams.
 */
static int qti_cpu_power_on(u_register_t mpidr)
{
	int core_pos = plat_core_pos_by_mpidr(mpidr);

	/* If not valid mpidr, return error */
	if (core_pos < 0 || core_pos >= QTISECLIB_PLAT_CORE_COUNT) {
		return PSCI_E_INVALID_PARAMS;
	}

	return qtiseclib_psci_node_power_on(mpidr);
}

static bool is_cpu_off(const psci_power_state_t *target_state)
{
	if ((target_state->pwr_domain_state[QTI_PWR_LVL0] ==
	     QTI_LOCAL_STATE_OFF) ||
	    (target_state->pwr_domain_state[QTI_PWR_LVL0] ==
	     QTI_LOCAL_STATE_DEEPOFF)) {
		return true;
	} else {
		return false;
	}
}

#ifdef PLAT_ipq52xx
static void qti_gic_restore_spis_to_core(unsigned int core);
#endif

static void qti_cpu_power_on_finish(const psci_power_state_t *target_state)
{
	const uint8_t *pwr_states =
	    (const uint8_t *)target_state->pwr_domain_state;
	qtiseclib_psci_node_on_finish(pwr_states);

	if (is_cpu_off(target_state)) {
		plat_qti_gic_cpuif_enable();
#ifdef PLAT_ipq52xx
		/*
		 * Restore the SPI target affinity that qti_node_power_off
		 * cleared for this core, so hot-unplug/re-plug is affinity
		 * neutral across cycles.
		 */
		qti_gic_restore_spis_to_core(plat_my_core_pos());
#endif
	}
}

static void qti_cpu_standby(plat_local_state_t cpu_state)
{
}

#ifdef PLAT_ipq52xx
/*
 * Migrate GIC SPIs away from a CPU that is powering down.
 *
 * ipq52xx uses the legacy SPM sequencer and (unlike ipq96xx's CPUCP P-Channel
 * with PCHANNEL_MASK_WAKEUP) has no hardware way to mask wake-up interrupts to a
 * collapsing core. On CPU_OFF, Linux is expected to migrate a core's affine
 * IRQs elsewhere, but for some SPIs that migration fails (kernel logs
 * "IRQx: set affinity failed"). Any SPI still targeting the dying core (e.g.
 * an all-CPU-affine SPI with target mask 0xF) will re-assert sys_spm_wakeup and
 * pull the core straight back out of C3, so it warm-boots while its PSCI
 * affinity state is OFF and BL31 panics.
 *
 * Defensively clear the dying core's bit from every SPI's GICD_ITARGETSR byte.
 * GICv2 ITARGETSR is one byte per interrupt id; only SPIs (id >= 32) are
 * writable (SGIs/PPIs ignore writes).
 *
 * Reroute target when the dying core was an SPI's SOLE target:
 * we send it to the PRIMARY cpu (core 0). Core 0 is the last-man-standing core
 * in Linux CPU hotplug (it is never offlined), so it is the only core
 * guaranteed to still be online regardless of how many other cores have already
 * been hot-unplugged. This keeps the migration correct and scalable for
 * offlining core 1, 2 and 3 in any order - we never reroute onto a core that
 * may itself already be powered down (which would re-introduce the very wake
 * bug we are fixing).
 *
 * ipq96xx does not need any of this: its CPUCP P-Channel hardware masks
 * wake-up interrupts to a collapsing core directly, so this workaround is
 * scoped to PLAT_ipq52xx only.
 */
#define QTI_PRIMARY_CORE_POS	0U

/*
 * Per-core saved bitmap of which SPIs had this core in their target mask when
 * the core went down, so power-on can restore exactly what offline cleared.
 * One bit per SPI id; sized for the max SPI count (GICv2: up to 1020 ids).
 *
 * qti_saved_spi_map:      bit set  => this SPI targeted the dying core.
 * qti_saved_spi_rerouted: bit set  => that SPI was the dying core's SOLE
 *                          target, so migrate force-rewrote its byte to the
 *                          reroute (primary) core rather than just clearing a
 *                          bit. These two cases are NOT inverses of each
 *                          other and must be restored differently:
 *   - normal (multi-target) clear: restore ORs the core's bit back in.
 *   - sole-owner reroute: restore must overwrite the target back to exactly
 *     {core}, undoing the reroute precisely - a plain OR would instead leave
 *     the SPI permanently targeting {core, primary}, drifting a bit further
 *     every offline/online cycle (confirmed bug: 2nd+ offline/online of the
 *     same core left previously-sole-owner SPIs stuck on {core0, core}).
 */
#define QTI_GIC_MAX_INTS	1020U
#define QTI_GIC_SPI_WORDS	((QTI_GIC_MAX_INTS + 31U) / 32U)
static uint32_t qti_saved_spi_map[PLATFORM_CORE_COUNT][QTI_GIC_SPI_WORDS];
static uint32_t qti_saved_spi_rerouted[PLATFORM_CORE_COUNT][QTI_GIC_SPI_WORDS];

static void qti_gic_migrate_spis_off_core(unsigned int dying_core)
{
	uintptr_t gicd = BASE_GICD_BASE;
	uint8_t dying_mask = (uint8_t)(1U << dying_core);
	uint8_t reroute_mask;
	uint32_t *saved = qti_saved_spi_map[dying_core];
	uint32_t *rerouted = qti_saved_spi_rerouted[dying_core];
	unsigned int i;
	unsigned int it_lines, num_ids;
	unsigned int cleared_count = 0U;
	unsigned int rerouted_count = 0U;

	/*
	 * Reroute orphaned SPIs to the primary core, unless the primary is the
	 * one going down (should not happen via Linux hotplug); in that
	 * degenerate case fall back to the lowest core that is not the dying
	 * one so we never write an all-zero (disabled) target.
	 */
	if (dying_core != QTI_PRIMARY_CORE_POS) {
		reroute_mask = (uint8_t)(1U << QTI_PRIMARY_CORE_POS);
	} else {
		reroute_mask = (uint8_t)(1U << ((dying_core == 0U) ? 1U : 0U));
	}

	/* GICD_TYPER.ITLinesNumber: total ids = 32 * (ITLinesNumber + 1). */
	it_lines = mmio_read_32(gicd + GICD_TYPER) & TYPER_IT_LINES_NO_MASK;
	num_ids = 32U * (it_lines + 1U);
	if (num_ids > QTI_GIC_MAX_INTS) {
		num_ids = QTI_GIC_MAX_INTS;
	}

	/* Fresh snapshot for this power-down. */
	for (i = 0U; i < QTI_GIC_SPI_WORDS; i++) {
		saved[i] = 0U;
		rerouted[i] = 0U;
	}

	for (i = 32U; i < num_ids; i++) {
		uintptr_t addr = gicd + GICD_ITARGETSR + i;
		uint8_t tgt = mmio_read_8(addr);
		uint8_t new_tgt;

		if ((tgt & dying_mask) == 0U) {
			continue;	/* not routed to the dying core */
		}

		/* Remember this SPI targeted the dying core, to restore later. */
		saved[i / 32U] |= (1U << (i % 32U));
		cleared_count++;

		new_tgt = (uint8_t)(tgt & (uint8_t)~dying_mask);
		if (new_tgt == 0U) {
			/*
			 * Was sole target: reroute to the primary core, and
			 * remember it so restore can UNDO this precisely
			 * (overwrite back to {core}) instead of OR-ing the
			 * core's bit into {primary}, which would leave the
			 * SPI permanently on {primary, core} and drift
			 * further on every subsequent offline/online cycle.
			 */
			new_tgt = reroute_mask;
			rerouted[i / 32U] |= (1U << (i % 32U));
			rerouted_count++;
			VERBOSE("qti_gic: core%u off: SPI%u sole-owner, tgt 0x%x -> 0x%x (rerouted)\n",
				dying_core, i, tgt, new_tgt);
		} else {
			VERBOSE("qti_gic: core%u off: SPI%u tgt 0x%x -> 0x%x\n",
				dying_core, i, tgt, new_tgt);
		}
		mmio_write_8(addr, new_tgt);
	}
	dsbsy();

	INFO("qti_gic: core%u off: migrated %u SPIs off (%u sole-owner rerouted to core%u)\n",
	     dying_core, cleared_count, rerouted_count, QTI_PRIMARY_CORE_POS);
}

/*
 * Symmetric restore: when the core comes back online, undo exactly what
 * qti_gic_migrate_spis_off_core() did on the way down, so interrupt affinity
 * is bit-for-bit what it was before the hot-unplug - stable across any number
 * of offline/online cycles, in any order, and safe even when another core is
 * restored in between (see the un-reroute derivation below).
 *
 * Two cases, handled differently (see the comment on qti_saved_spi_rerouted):
 *  - normal clear (SPI still had other targets): OR this core's bit back in.
 *  - sole-owner reroute (SPI was forced onto {primary}): clear the reroute
 *    core's bit and OR this core's bit back in - NOT a blind overwrite to
 *    {core}, because another core's restore may have already OR'd its own
 *    bit into this same byte (e.g. a multi-target SPI whose OTHER target was
 *    restored first while this core was still down); overwriting would
 *    silently drop that core's legitimate restore.
 */
static void qti_gic_restore_spis_to_core(unsigned int core)
{
	uintptr_t gicd = BASE_GICD_BASE;
	uint8_t core_mask = (uint8_t)(1U << core);
	uint8_t reroute_mask;
	uint32_t *saved = qti_saved_spi_map[core];
	uint32_t *rerouted = qti_saved_spi_rerouted[core];
	unsigned int i;
	unsigned int restored_count = 0U;
	unsigned int unrerouted_count = 0U;

	/* Must match the reroute target qti_gic_migrate_spis_off_core() used. */
	if (core != QTI_PRIMARY_CORE_POS) {
		reroute_mask = (uint8_t)(1U << QTI_PRIMARY_CORE_POS);
	} else {
		reroute_mask = (uint8_t)(1U << ((core == 0U) ? 1U : 0U));
	}

	for (i = 32U; i < QTI_GIC_MAX_INTS; i++) {
		uintptr_t addr;
		uint8_t tgt;
		uint8_t new_tgt;

		if ((saved[i / 32U] & (1U << (i % 32U))) == 0U) {
			continue;
		}

		addr = gicd + GICD_ITARGETSR + i;
		tgt = mmio_read_8(addr);

		if ((rerouted[i / 32U] & (1U << (i % 32U))) != 0U) {
			/*
			 * Undo the sole-owner reroute: drop the reroute core's
			 * bit, add this core's bit back. Any OTHER bits set by
			 * a different core's restore in the meantime are left
			 * untouched.
			 */
			new_tgt = (uint8_t)((tgt & (uint8_t)~reroute_mask) | core_mask);
			unrerouted_count++;
		} else {
			new_tgt = (uint8_t)(tgt | core_mask);
		}

		VERBOSE("qti_gic: core%u on: SPI%u tgt 0x%x -> 0x%x%s\n",
			core, i, tgt, new_tgt,
			((rerouted[i / 32U] & (1U << (i % 32U))) != 0U) ?
			" (un-rerouted)" : "");
		mmio_write_8(addr, new_tgt);
		restored_count++;
	}
	dsbsy();

	INFO("qti_gic: core%u on: restored %u SPIs (%u un-rerouted from core%u)\n",
	     core, restored_count, unrerouted_count, QTI_PRIMARY_CORE_POS);

	/* Snapshot consumed. */
	for (i = 0U; i < QTI_GIC_SPI_WORDS; i++) {
		saved[i] = 0U;
		rerouted[i] = 0U;
	}
}
#endif /* PLAT_ipq52xx */

static void qti_node_power_off(const psci_power_state_t *target_state)
{
	qtiseclib_psci_node_power_off((const uint8_t *)
				      target_state->pwr_domain_state);
	if (is_cpu_off(target_state)) {
#ifdef PLAT_ipq52xx
		/*
		 * Retarget any SPI still pointing at this (dying) core BEFORE
		 * the SPM collapses it, so a re-asserted interrupt cannot wake
		 * it back up into a warm-boot-while-OFF panic.
		 */
		qti_gic_migrate_spis_off_core(plat_my_core_pos());
#endif
		plat_qti_gic_cpuif_disable();
#ifndef PLAT_ipq52xx
		qti_set_cpupwrctlr_val();
#endif
	}
}

static void qti_node_suspend(const psci_power_state_t *target_state)
{
	qtiseclib_psci_node_suspend((const uint8_t *)target_state->
				    pwr_domain_state);
	if (is_cpu_off(target_state)) {
		plat_qti_gic_cpuif_disable();
#ifndef PLAT_ipq52xx
		qti_set_cpupwrctlr_val();
#endif
	}
}

static void qti_node_suspend_finish(const psci_power_state_t *target_state)
{
	const uint8_t *pwr_states =
	    (const uint8_t *)target_state->pwr_domain_state;
	qtiseclib_psci_node_suspend_finish(pwr_states);
	if (is_cpu_off(target_state)) {
		plat_qti_gic_cpuif_enable();
	}
}

__dead2 void qti_domain_power_down_wfi(const psci_power_state_t *target_state)
{

	/* For now just do WFI - add any target specific handling if needed */
	psci_power_down_wfi();
	/* We should never reach here */
}

static __dead2 void assert_ps_hold(void)
{
	mmio_write_32(QTI_PS_HOLD_REG, 0);
	mdelay(1000);

	/* Should be dead before reaching this. */
	panic();
}

__dead2 void qti_system_off(void)
{
	qti_pmic_prepare_shutdown();
	assert_ps_hold();
}

__dead2 void qti_system_reset(void)
{
	qti_pmic_prepare_reset();
	assert_ps_hold();
}

void qti_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	int i = 0;
	unsigned int state_id, power_state;
	int size = ARRAY_SIZE(qti_pm_idle_states);

	/*
	 * Find deepest state.
	 * The arm_pm_idle_states[] array has last element by default 0,
	 * so the real deepest state is second last element of that array.
	 */
	power_state = qti_pm_idle_states[size - 2];
	state_id = psci_get_pstate_id(power_state);

	/* Parse the State ID and populate the state info parameter */
	while (state_id) {
		req_state->pwr_domain_state[i++] =
		    state_id & QTI_LOCAL_PSTATE_MASK;
		state_id >>= QTI_LOCAL_PSTATE_WIDTH;
	}

#if PSCI_OS_INIT_MODE
	req_state->last_at_pwrlvl = PLAT_MAX_PWR_LVL;
#endif
}

/*
 * Structure containing platform specific PSCI operations. Common
 * PSCI layer will use this.
 */
const plat_psci_ops_t plat_qti_psci_pm_ops = {
	.pwr_domain_on = qti_cpu_power_on,
	.pwr_domain_on_finish = qti_cpu_power_on_finish,
	.cpu_standby = qti_cpu_standby,
	.pwr_domain_off = qti_node_power_off,
	.pwr_domain_suspend = qti_node_suspend,
	.pwr_domain_suspend_finish = qti_node_suspend_finish,
	.pwr_domain_pwr_down_wfi = qti_domain_power_down_wfi,
	.system_off = qti_system_off,
	.system_reset = qti_system_reset,
	.system_reset2 = qti_system_reset2,
	.get_node_hw_state = NULL,
	.translate_power_state_by_mpidr = NULL,
	.get_sys_suspend_power_state = qti_get_sys_suspend_power_state,
	.validate_power_state = qti_validate_power_state,
};

/**
 * The QTI Standard platform definition of platform porting API
 * `plat_setup_psci_ops`.
 */
int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	int err;

	err = qtiseclib_psci_init((uintptr_t)bl31_warm_entrypoint);
	if (err == PSCI_E_SUCCESS) {
		*psci_ops = &plat_qti_psci_pm_ops;
	}

	return err;
}
