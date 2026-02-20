/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef PLAT_LD_S
#define PLAT_LD_S

MEMORY {
    IMEM_RO (rx): ORIGIN = QTI_IMEM_RO_BASE, LENGTH = QTI_IMEM_RO_SIZE
    IMEM_RW (rw): ORIGIN = QTI_IMEM_RW_BASE, LENGTH = QTI_IMEM_RW_SIZE
    IMEM_DEBUG_DATA (rw!a): ORIGIN = QTI_IMEM_DEBUG_DATA_BASE, LENGTH = QTI_IMEM_DEBUG_DATA_SIZE
    DIAG (rw): ORIGIN = DIAG_BASE, LENGTH = DIAG_LEN
}

SECTIONS {
    imem_ro (QTI_IMEM_RO_BASE) : {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (QTI_IMEM_RESET_ENTRY))
        KEEP(* (QTI_SYSDBG_IMEM_CODE))
        KEEP(*(.code_imem.QTISECLIB_IMEM_CODE))
    } >IMEM_RO

    ASSERT(. <= (QTI_IMEM_RO_BASE + QTI_IMEM_RO_SIZE),
        "IMEM RO region has exceeded its limit.")

    ASSERT((QTI_IMEM_RO_BASE + QTI_IMEM_RO_SIZE) <= QTI_IMEM_RW_BASE,
        "IMEM_RO overlaps with IMEM_RW")

    imem_rw (QTI_IMEM_RW_BASE):  {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (.uncached_imem.QTI_SYSDBG_IMEM_DATA))
    } >IMEM_RW

    ASSERT(. <= (QTI_IMEM_RW_BASE + QTI_IMEM_RW_SIZE),
        "IMEM RW region has exceeded its limit.")

    ASSERT((QTI_IMEM_RW_BASE + QTI_IMEM_RW_SIZE) <= QTI_IMEM_DEBUG_DATA_BASE,
        "IMEM_RW overlaps with IMEM_DEBUG_DATA")

    imem_debug_data (QTI_IMEM_DEBUG_DATA_BASE) (NOLOAD) : {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (.imem_rw_debug_data))
    } >IMEM_DEBUG_DATA

    ASSERT(. <= (QTI_IMEM_DEBUG_DATA_BASE + QTI_IMEM_DEBUG_DATA_SIZE),
        "IMEM DEBUG DATA region has exceeded its limit.")

    ASSERT((QTI_IMEM_DEBUG_DATA_BASE + QTI_IMEM_DEBUG_DATA_SIZE) <= DIAG_BASE,
        "IMEM_DEBUG_DATA overlaps with DIAG")

    /****************************************************************
    *  TZOS Diag
    *  Attrib.: RW, PXN=1, Uncacheable, PL1
    *  Note: TZ Diag size : 12KB , placed in uncacheable OCIMEM region.
    ****************************************************************/
    diag (DIAG_BASE) : {
        __DIAG_REGION_START__ = .;
        . = . + DIAG_LEN;
        __DIAG_REGION_END__ = .;
    } >DIAG
}

#endif /* PLAT_LD_S */
