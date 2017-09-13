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

#include "platform_sleep.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/* UART port used for standard I/O */
#define STDIO_UART  ( WICED_UART_1 )

/******************************************************
 *                   Enumerations
 ******************************************************/

/*
BCM943903WCD1_1 platform pin definitions ...
+-------------------------------------------------------------------------------------------------------------------+
| Enum ID       |Pin  |   Pin Name on    |  Module                           |  Conn  |  Peripheral  |  Peripheral  |
|               | #   |   43903WLBU_1    |  Alias                            |  Pin   |  Available   |    Alias     |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_1  | E1  | GPIO_0           | GPIO_0                            |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_2  | D1  | GPIO_1           | GPIO_1_GSPI_MODE                  |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_3  | E5  | GPIO_7           | GPIO_7_WCPU_BOOT_MODE             |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_4  | D9  | GPIO_8           | GPIO_8_TAP_SEL                    |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_5  | E2  | GPIO_9           | GPIO_9                            |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_6  | C1  | GPIO_10          | GPIO_10                           |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_7  | B1  | GPIO_11          | GPIO_11_ACPU_BOOT_MODE            |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_8  | C6  | GPIO_12          | GPIO_12                           |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_9  | E8  | GPIO_13          | GPIO_13_SDIO_MODE                 |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_10 | A3  | GPIO_14          | GPIO_14                           |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_11 | A2  | GPIO_15          | GPIO_15                           |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_12 | C2  | GPIO_16          | GPIO_16                           |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_19 | C5  | SPI_0_MISO       | SPI0_MISO                         |    -   |  SPI, GPIO   | GPIO_24      |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_20 | B5  | SPI_0_CLK        | SPI0_CLK                          |    -   |  SPI, GPIO   | GPIO_25      |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_21 | B6  | SPI_0_MOSI       | SPI0_SISO                         |    -   |  SPI, GPIO   | GPIO_26      |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_22 | A5  | SPI_0_CS         | SPI0_CS                           |    -   |  SPI, GPIO   | GPIO_27      |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_23 | G10 | GPIO_2           | GPIO_2_JTAG_TCK                   |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_24 | G12 | GPIO_3           | GPIO_3_JTAG_TMS                   |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_25 | F10 | GPIO_4           | GPIO_4_JTAG_TDI                   |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_26 | F11 | GPIO_5           | GPIO_5_JTAG_TDO                   |    -   |  GPIO        |              |
|---------------+-----+------------------+-----------------------------------+--------+--------------+--------------|
| WICED_GPIO_27 | F12 | GPIO_6           | GPIO_6_JTAG_TRST_L                |    -   |  GPIO        |              |
+-------------------------------------------------------------------------------------------------------------------+

+--------------------------------------------------------------------------------------------------------------------------------+
| Enum ID                 |Pin  |   Pin Name on |    Board                                 |  Conn  |  Peripheral |  Peripheral  |
|                         | #   |      43909    |  Connection                              |  Pin   |  Available  |    Alias     |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_1  | K10 | RF_SW_CTRL_6  | RF_SW_CTRL_6_UART1_RX_IN                 |  -     |  UART1      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_2  | L12 | RF_SW_CTRL_7  | RF_SW_CTRL_7_RSRC_INIT_MODE_UART1_TX_OUT |  -     |  UART1      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_3  | C7  | UART0_RXD     | UART0_RXD_IN                             |  -     |  UART2      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_4  | B8  | UART0_TXD     | UART0_TXD_OUT                            |  -     |  UART2      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_5  | A7  | UART0_CTS     | UART0_CTS_IN                             |  -     |  UART2      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_6  | B7  | UART0_RTS     | UATR0_CTS_OUT                            |  -     |  UART2      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_7  | M12 | RF_SW_CTRL_8  | RF_SW_CTRL_8_BT_SECI_IN                  |  -     |  UART3      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------|
| WICED_PERIPHERAL_PIN_8  | N11 | RF_SW_CTRL_9  | RF_SW_CTRL_9_HIB_LPO_SEL_BT_SECI_OUT     |  -     |  UART3      |              |
|-------------------------+-----+---------------+------------------------------------------+--------+-------------+--------------+
*/

typedef enum
{
    WICED_GPIO_1,
    WICED_GPIO_2,
    WICED_GPIO_3,
    WICED_GPIO_4,
    WICED_GPIO_5,
    WICED_GPIO_6,
    WICED_GPIO_7,
    WICED_GPIO_8,
    WICED_GPIO_9,
    WICED_GPIO_10,
    WICED_GPIO_11,
    WICED_GPIO_12,
    WICED_GPIO_13,
    WICED_GPIO_14,
    WICED_GPIO_15,
    WICED_GPIO_16,
    WICED_GPIO_17,
    WICED_GPIO_18,
    WICED_GPIO_19,
    WICED_GPIO_20,
    WICED_GPIO_21,
    WICED_GPIO_22,
    WICED_GPIO_23,
    WICED_GPIO_24,
    WICED_GPIO_25,
    WICED_GPIO_26,
    WICED_GPIO_27,
    WICED_GPIO_MAX, /* Denotes the total number of GPIO port aliases. Not a valid GPIO alias */
} wiced_gpio_t;

typedef enum
{
    WICED_PERIPHERAL_PIN_1 = WICED_GPIO_MAX,
    WICED_PERIPHERAL_PIN_2,
    WICED_PERIPHERAL_PIN_3,
    WICED_PERIPHERAL_PIN_4,
    WICED_PERIPHERAL_PIN_5,
    WICED_PERIPHERAL_PIN_6,
    WICED_PERIPHERAL_PIN_7,
    WICED_PERIPHERAL_PIN_8,
    WICED_PERIPHERAL_PIN_MAX, /* Denotes the total number of GPIO and peripheral pins. Not a valid pin alias */
} wiced_peripheral_pin_t;

typedef enum
{
    WICED_SPI_1,
    WICED_SPI_MAX, /* Denotes the total number of SPI port aliases. Not a valid SPI alias */
} wiced_spi_t;

typedef enum
{
    WICED_I2C_1,
    WICED_I2C_MAX, /* Denotes the total number of I2C port aliases. Not a valid I2C alias */
} wiced_i2c_t;

typedef enum
{
    WICED_PWM_MAX, /* Denotes the total number of PWM port aliases. Not a valid PWM alias */
} wiced_pwm_t;

typedef enum
{
   WICED_ADC_NONE = 0xff, /* No ADCs */
} wiced_adc_t;


typedef enum
{
    WICED_UART_1,   /* ChipCommon Slow UART */
    WICED_UART_2,   /* ChipCommon Fast UART */
    WICED_UART_3,   /* GCI UART */
    WICED_UART_MAX, /* Denotes the total number of UART port aliases. Not a valid UART alias */
} wiced_uart_t;

typedef enum
{
    WICED_I2S_MAX, /* Denotes the total number of I2S port aliases. Not a valid I2S alias */
} wiced_i2s_t;

/* Logical Button-ids which map to physical buttons on the board */
typedef enum
{
    PLATFORM_BUTTON_1,
    PLATFORM_BUTTON_2,
    PLATFORM_BUTTON_MAX, /* Denotes the total number of Buttons on the board. Not a valid Button Alias */
} platform_button_t;

/* Components connected to external I/Os */
#define WICED_LED1         ( WICED_GPIO_8 )
#define WICED_LED2         ( WICED_GPIO_12 )
#define WICED_BUTTON1      ( WICED_GPIO_5 )
#define WICED_BUTTON2      ( WICED_GPIO_6 )

#define WICED_PLATFORM_BUTTON_COUNT  ( 2 )

#define AUTH_IC_I2C_PORT (WICED_I2C_1)
#define WICED_GPIO_AUTH_RST  ( WICED_GPIO_9 )

/* no audio on this platform */
#define PLATFORM_DEFAULT_AUDIO_INPUT    AUDIO_DEVICE_ID_NONE
#define PLATFORM_AUDIO_NUM_INPUTS       (0)
#define PLATFORM_DEFAULT_AUDIO_OUTPUT   AUDIO_DEVICE_ID_OUTPUT_NONE
#define PLATFORM_AUDIO_NUM_OUTPUTS      (0)

#ifdef __cplusplus
} /*extern "C" */
#endif
