#
# Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := App_Ping_$(RTOS)_$(NETWORK)

$(NAME)_SOURCES := $(RTOS)_$(NETWORK)_Ping.c

# Disable watchdog for all WWD apps
GLOBAL_DEFINES += WICED_DISABLE_WATCHDOG

NO_WICED_API := 1

LWIP_NUM_PACKET_BUFFERS_IN_POOL := 4

FreeRTOS_START_STACK := 600
ThreadX_START_STACK  := 600
VALID_PLATFORMS := BCM943362WCD4 BCM4774_HB
