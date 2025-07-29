/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef PLAT_LD_S
#define PLAT_LD_S


SECTIONS {
    shared_imem_ro (QTI_SHARED_IMEM_RO_BASE) : {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (QTI_IMEM_RESET_ENTRY))
        KEEP(* (QTI_SYSDBG_IMEM_CODE))
        KEEP(*(.code_imem.QTISECLIB_IMEM_CODE))
    }
    ASSERT(. <= (QTI_SHARED_IMEM_RO_BASE + QTI_SHARED_IMEM_RO_SIZE), "Shared IMEM RO region has exceeded its limit.")

    shared_imem_rw (QTI_SHARED_IMEM_RW_BASE):  {
        . = ALIGN(PAGE_SIZE);
        KEEP(* (.uncached_imem.QTI_SYSDBG_IMEM_DATA))
    }
    ASSERT(. <= (QTI_SHARED_IMEM_RW_BASE +  QTI_SHARED_IMEM_RW_SIZE), "Shared IMEM RW region has exceeded its limit.")

    diag (QTI_DIAG_RG_START) (NOLOAD) : {
        __DIAG_REGION_START__ = .;
        . = . + QTI_DIAG_RG_SIZE;
        __DIAG_REGION_END__ = .;
    }
}

#endif /* PLAT_LD_S */
