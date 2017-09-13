#
# Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#
NAME := NetX

NETX_VERSION := 5.5_sp1

VALID_RTOS_LIST:= ThreadX

$(NAME)_COMPONENTS += WICED/network/NetX/WWD

ifeq (,$(APP_WWD_ONLY)$(NS_WWD_ONLY)$(RTOS_WWD_ONLY))
$(NAME)_COMPONENTS += WICED/network/NetX/WICED
endif

# Define some macros to allow for some network-specific checks
GLOBAL_DEFINES += NETWORK_$(NAME)=1
GLOBAL_DEFINES += $(NAME)_VERSION=$$(SLASH_QUOTE_START)v$(NETX_VERSION)$$(SLASH_QUOTE_END)
# prevent sending reset for non-blocking disconnect
GLOBAL_DEFINES += NX_INCLUDE_USER_DEFINE_FILE
GLOBAL_DEFINES += __fd_set_defined

ifneq ($(TOOLCHAIN_NAME),IAR)
GLOBAL_DEFINES += SYS_TIME_H_AVAILABLE
endif

GLOBAL_INCLUDES := ver$(NETX_VERSION) \
                   WICED


ifdef WICED_ENABLE_TRACEX
# Precompiled library with TraceX
NETX_LIBRARY_NAME :=NetX.TraceX.$(RTOS).$(HOST_ARCH).$(BUILD_TYPE).a
else
# Precompiled library
NETX_LIBRARY_NAME :=NetX.$(RTOS).$(HOST_ARCH).$(BUILD_TYPE).a
endif

ifneq ($(wildcard $(CURDIR)$(NETX_LIBRARY_NAME)),)
# Using a precompiled Library
$(NAME)_PREBUILT_LIBRARY := $(NETX_LIBRARY_NAME)
else
# Build from source (Broadcom internal)
-include $(CURDIR)NetX_src.mk
endif #ifneq ($(wildcard $(CURDIR)$(NETX_LIBRARY_NAME)),)

$(NAME)_SOURCES += ver$(NETX_VERSION)/nx_external_functions.c