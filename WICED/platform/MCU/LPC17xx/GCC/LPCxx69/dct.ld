/*
 * Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */


MEMORY
{
/*
    BTLDR_OR_VECTOR_FLASH(rx) : ORIGIN = 0x0, LENGTH = 512
*/
    DCT1_FLASH     (rx) : ORIGIN = 0x4000,      LENGTH = 16K
    DCT2_FLASH     (rx) : ORIGIN = 0x8000,      LENGTH = 16K
    APP_FLASH      (rx) : ORIGIN = 0xC00,       LENGTH = 464K
    SRAM_32K_LOCAL (rwx): ORIGIN = 0x10000000,  LENGTH = 32K
    SRAM_32K_AHB   (rwx): ORIGIN = 0x2007C000,  LENGTH = 32K
}


PHDRS
{
    rodata PT_LOAD ;
}

SECTIONS
{
    .rodata :
    {
        dct_location = .;
        KEEP(*(.rodata.initial_dct))
        KEEP(*(.rodata._app_dct))
        KEEP(*(.rodata.wiced_apps_lut))
        KEEP(*(.rodata.wiced_apps_lut_fill))
        dct_end = .;

    } >DCT1_FLASH  AT> DCT1_FLASH :rodata
}

PROVIDE( dct_full_size_loc = LENGTH( DCT1_FLASH ));
PROVIDE( dct_used_size_loc = (dct_end - ORIGIN( DCT1_FLASH )));