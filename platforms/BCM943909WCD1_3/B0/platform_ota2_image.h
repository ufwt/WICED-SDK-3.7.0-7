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
 *  OTA2 platform-specific defines
 *
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 *                     Macros
 ******************************************************/

/* To use a different than the default CRC, define here in this platform-specific header */
//#define OTA2_CRC_INIT_VALUE                              CRC32_INIT_VALUE
//#define OTA2_CRC_FUNCTION(address, size, previous_value) (uint32_t)crc32(address, size, previous_value)
//typedef uint32_t    OTA2_CRC_VAR;
//#define OTA2_CRC_HTON(value)                             htonl(value)
//#define OTA2_CRC_NTOH(value)                             ntohl(value)


#ifdef CHECK_BATTERY_LEVEL_BEFORE_OTA2_UPGRADE
// TODO:
#endif



#ifdef __cplusplus
} /*extern "C" */
#endif

