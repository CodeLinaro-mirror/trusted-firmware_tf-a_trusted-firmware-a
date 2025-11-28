#ifndef __ICB_ERROR_H__
#define __ICB_ERROR_H__
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "stdint.h"
#include <stdbool.h>

/*============================================================================
 *				FUNCTIONS
 *============================================================================
 */

/*============================================================================*/
/**
 * @brief
 *      Initializes all ICB bus error drivers.
 *
 * @param[in]  None.
 *
 * @return
 *      None.
 *
 * @dependencies
 *      None.
 *
 * @sideeffects
 *      None.
 */
/*============================================================================*/
void qti_icb_error_init(void);

#endif /* __ICB_ERROR_H__ */
