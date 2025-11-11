/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef PLAT_LD_S
#define PLAT_LD_S

MEMORY {
    SHARED_IMEM_RO (rx): ORIGIN = QTI_SHARED_IMEM_RO_BASE, LENGTH = QTI_SHARED_IMEM_RO_SIZE
    SHARED_IMEM_RW (rw): ORIGIN = QTI_SHARED_IMEM_RW_BASE, LENGTH = QTI_SHARED_IMEM_RW_SIZE
    DIAG (rw): ORIGIN = DIAG_BASE, LENGTH = DIAG_LEN
}

SECTIONS {
    ASSERT((DIAG_BASE + DIAG_LEN <= QTI_SHARED_IMEM_RO_BASE) ||
           (QTI_SHARED_IMEM_RO_BASE + QTI_SHARED_IMEM_RO_SIZE <= DIAG_BASE),
           "DIAG overlaps with SHARED_IMEM_RO")

    ASSERT((DIAG_BASE + DIAG_LEN <= QTI_SHARED_IMEM_RW_BASE) ||
           (QTI_SHARED_IMEM_RW_BASE + QTI_SHARED_IMEM_RW_SIZE <= DIAG_BASE),
           "DIAG overlaps with SHARED_IMEM_RW")

    shared_imem_ro (QTI_SHARED_IMEM_RO_BASE) : {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (QTI_IMEM_RESET_ENTRY))
        KEEP(* (QTI_SYSDBG_IMEM_CODE))
        KEEP(*(.code_imem.QTISECLIB_IMEM_CODE))
    } >SHARED_IMEM_RO
    ASSERT(. <= (QTI_SHARED_IMEM_RO_BASE + QTI_SHARED_IMEM_RO_SIZE),
           "Shared IMEM RO region has exceeded its limit.")

    shared_imem_rw (QTI_SHARED_IMEM_RW_BASE):  {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (.uncached_imem.QTI_SYSDBG_IMEM_DATA))
    } >SHARED_IMEM_RW
    ASSERT(. <= (QTI_SHARED_IMEM_RW_BASE +  QTI_SHARED_IMEM_RW_SIZE),
           "Shared IMEM RW region has exceeded its limit.")

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
