/*
 * Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "http_stream.h"
#include "dns.h"

#include "ota2_test_dct.h"
#include "wiced_ota2_service.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define OTA2_APP_PRINT(level, arg)   {if (g_player != NULL){if (g_player->log_level >= level) WPRINT_MACRO(arg);}else{WPRINT_MACRO(arg);} }

/******************************************************
 *                    Constants
 ******************************************************/

#define OTA2_THREAD_STACK_SIZE  8000

#define PLAYER_TAG_VALID                0x62EDBA26
#define PLAYER_TAG_INVALID              0xDEADBEEF

#define OTA2_HTTP_THREAD_PRIORITY       (WICED_DEFAULT_WORKER_PRIORITY)
#define OTA2_HTTP_STACK_SIZE            (6*1024)


#define SECONDS_PER_MINUTE              (60)
#define SECONDS_PER_HOUR                (SECONDS_PER_MINUTE * 60)
#define SECONDS_IN_24_HOURS             (SECONDS_PER_HOUR * 24) //86400


/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum {
    PLAYER_EVENT_SHUTDOWN             = (1 <<  0),

    PLAYER_EVENT_DISCONNECT_AP        = (1 <<  1),
    PLAYER_EVENT_CONNECT_DEFAULT_AP   = (1 <<  2),
    PLAYER_EVENT_CONNECT_OTA2_AP      = (1 <<  3),
    PLAYER_EVENT_STATUS               = (1 <<  4),

    PLAYER_EVENT_GET_UPDATE           = (1 <<  5),
    PLAYER_EVENT_GET_TIMED_UPDATE     = (1 <<  6),
    PLAYER_EVENT_STOP_TIMED_UPDATE    = (1 <<  7),

    PLAYER_EVENT_FACTORY_RESET_STATUS = (1 <<  8),
    PLAYER_EVENT_FACTORY_RESET_REBOOT = (1 <<  9),
    PLAYER_EVENT_FACTORY_RESET_NOW    = (1 << 10),

    PLAYER_EVENT_UPDATE_NOW           = (1 << 11),
    PLAYER_EVENT_UPDATE_REBOOT        = (1 << 12),
    PLAYER_EVENT_UPDATE_STATUS        = (1 << 13),

    PLAYER_EVENT_LOG_LEVEL            = (1 << 20),

} PLAYER_EVENTS_T;

#define PLAYER_ALL_EVENTS       (-1)

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct ota2_data_s {
    uint32_t tag;

    wiced_ota2_log_level_t           log_level;

    wiced_event_flags_t             events;

    platform_dct_network_config_t*  dct_network;
    platform_dct_wifi_config_t*     dct_wifi;
    ota2_dct_t*                     dct_app;

    /* source info for http streaming */
    char                            uri_to_stream[WICED_OTA2_HTTP_QUERY_SIZE];

    /* background service for OTA2 info */
    char                                    ota2_host_name[WICED_OTA2_HOST_NAME];
    char                                    ota2_file_path[WICED_OTA2_FILE_PATH];
    wiced_ota2_backround_service_params_t   ota2_bg_params;
    void*                                   ota2_bg_service;

    /* debugging */
    wiced_time_t                    start_time;         /* when the app started */

    /* internal */
    void*                           internal;

} ota2_data_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

extern ota2_data_t *g_player;

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
