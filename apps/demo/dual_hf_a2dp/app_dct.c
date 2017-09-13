/*
 * Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 */

#include "wiced_framework.h"
#include "app_dct.h"
#include "bluetooth_nv.h"


/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct
{
    bt_dct_t       bt_dct;
} app_dct_t;

/******************************************************
 *               Function Declarations
 ******************************************************/

/******************************************************
 *               Variables Definitions
 ******************************************************/

DEFINE_APP_DCT(app_dct_t)
{
    .bt_dct =
    {
        .bt_hash_table              = {0},
        .bt_paired_device_info[0]   = {0},
        .bt_local_id_keys           = {{0}},
    }
};

const uint32_t app_dct_offset[] =
{
    [APP_DCT_BT]      = OFFSETOF(app_dct_t, bt_dct),
};
/******************************************************
 *               Function Definitions
 ******************************************************/

uint32_t app_dct_get_offset(app_dct_offset_list_t section)
{
    return app_dct_offset[section];
}
