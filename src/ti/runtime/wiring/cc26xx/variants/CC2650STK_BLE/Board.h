/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       Board.h
 *
 *  @brief      CC2650SENSORTAG Board Specific header file.
 *
 *  NB! This is the board file for PCB version 1.2
 *
 *  The CC2650 header file should be included in an application as follows:
 *  @code
 *  #include <Board.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __BOARD_H__
#define __BOARD_H__

#include <ti/drivers/UART.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TI_COMPILER_VERSION__
#undef gcc
#define gcc 1
#endif

/** ============================================================================
 *  Includes
 *  ==========================================================================*/
#include <ti/drivers/PIN.h>
#include <driverlib/ioc.h>

/** ============================================================================
 *  Externs
 *  ==========================================================================*/
extern PIN_Config BoardGpioInitTable[];

/** ============================================================================
 *  Defines
 *  ==========================================================================*/

/* Symbol by generic Board.c to include the correct PCB  specific Board.c */
#define CC2650ST_0120

/* Identify as SensorTag */
#define CC2650ST_7ID

/* Same RF Configuration as 7x7 EM */
#define CC2650EM_7ID

/* This PCB version supports magnetometer */
#define FEATURE_MAGNETOMETER

/* External flash manufacturer and device ID */
#define EXT_FLASH_MAN_ID            0xEF
#define EXT_FLASH_DEV_ID            0x12

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                <pin mapping>
 */

/* Discrete outputs */
#define Board_LED1                  IOID_10
#define Board_LED2                  IOID_15
#define Board_BUZZER                IOID_21
#define Board_LED_ON                1
#define Board_LED_OFF               0
#define Board_BUZZER_ON             1
#define Board_BUZZER_OFF            0

/* Discrete inputs */
#define Board_KEY_LEFT              IOID_0
#define Board_KEY_RIGHT             IOID_4
#define Board_RELAY                 IOID_3

/* Sensor outputs */
#define Board_MPU_INT               IOID_7
#define Board_TMP_RDY               IOID_1

/* I2C */
#define Board_I2C0_SDA0             IOID_5
#define Board_I2C0_SCL0             IOID_6
#define Board_I2C0_SDA1             IOID_8
#define Board_I2C0_SCL1             IOID_9

/* SPI */
#define Board_SPI_FLASH_CS          IOID_14
#define Board_SPI_DEVPK_CS          IOID_20
#define Board_FLASH_CS_ON           0
#define Board_FLASH_CS_OFF          1

#define Board_SPI0_MISO             IOID_18
#define Board_SPI0_MOSI             IOID_19
#define Board_SPI0_CLK              IOID_17
#define Board_SPI0_CSN              PIN_UNASSIGNED
#define Board_SPI1_MISO             PIN_UNASSIGNED
#define Board_SPI1_MOSI             PIN_UNASSIGNED
#define Board_SPI1_CLK              PIN_UNASSIGNED
#define Board_SPI1_CSN              PIN_UNASSIGNED

/* UART (when connected to SRF06EB) */
#define Board_EB_UART_TX            IOID_16
#define Board_EB_UART_RX            IOID_17

/* PWM pins */
#define Board_PWM0_PIN              IOID_0
#define Board_PWM1_PIN              IOID_1
#define Board_PWM2_PIN              IOID_2
#define Board_PWM3_PIN              IOID_3
#define Board_PWM4_PIN              IOID_4
#define Board_PWM5_PIN              IOID_5
#define Board_PWM6_PIN              IOID_6
#define Board_PWM7_PIN              IOID_7

/* DevPack */
#define Board_AUDIOFS_TDO           IOID_16
#define Board_AUDIODO               IOID_22
#define Board_DP2                   IOID_23
#define Board_DP1                   IOID_24
#define Board_DP0                   IOID_25
#define Board_DP3                   IOID_27
#define Board_DP4_UARTRX            IOID_28
#define Board_DP5_UARTTX            IOID_29
#define Board_DEVPK_ID              IOID_30

/* Power control */
#define Board_MPU_POWER             IOID_12
#define Board_MPU_POWER_ON          1
#define Board_MPU_POWER_OFF         0

/* Audio */
#define Board_MIC_POWER             IOID_13
#define Board_MIC_POWER_ON          1
#define Board_MIC_POWER_OFF         0
#define Board_AUDIO_DI              IOID_2
#define Board_AUDIO_CLK             IOID_11

/* UART pins used by driver */
#define Board_UART_TX               Board_DP5_UARTTX
#define Board_UART_RX               Board_DP4_UARTRX

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/
/* Generic I2C instance identifiers */
#define Board_I2C                   CC2650_I2C0
/* Generic SPI instance identifiers */
#define Board_SPI0                  CC2650_SPI0
#define Board_SPI1                  CC2650_SPI1
/* Generic UART instance identifiers */
#define Board_UART                  CC2650_UART0
/* Generic PWM instance identifiers */
#define Board_PWM                   CC2650_PWM0


/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    CC2650_I2CName
 *  @brief  Enum of I2C names on the CC2650 dev board
 */
typedef enum CC2650_I2CName {
    CC2650_I2C0 = 0,
    CC2650_I2C1,
    CC2650_I2CCOUNT
} CC2650_I2CName;

/*!
 *  @def    CC2650_CryptoName
 *  @brief  Enum of Crypto names on the CC2650 dev board
 */
typedef enum CC2650_CryptoName {
    CC2650_CRYPTO0 = 0,
    CC2650_CRYPTOCOUNT
} CC2650_CryptoName;


/*!
 *  @def    CC2650_SPIName
 *  @brief  Enum of SPI names on the CC2650 dev board
 */
typedef enum CC2650_SPIName {
    CC2650_SPI0 = 0,
    CC2650_SPI1,
    CC2650_SPICOUNT
} CC2650_SPIName;

/*!
 *  @def    CC2650_UARTName
 *  @brief  Enum of UARTs on the CC2650 dev board
 */
typedef enum CC2650_UARTName {
    CC2650_UART0 = 0,
    CC2650_UARTCOUNT
} CC2650_UARTName;

/*!
 *  @def    CC2650_UdmaName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC2650_UdmaName {
    CC2650_UDMA0 = 0,
    CC2650_UDMACOUNT
} CC2650_UdmaName;

/*!
 *  @def    CC2650_PWMName
 *  @brief  Enum of PWM pin names on the CC3200_LP dev board
 */
typedef enum CC2650_PWMName {
    CC2650_PWM0 = 0, /* PWM output from TIMERA0 side A */
    CC2650_PWM1 = 1, /* PWM output from TIMERA0 side B */
    CC2650_PWM2 = 2, /* PWM output from TIMERA1 side A */
    CC2650_PWM3 = 3, /* PWM output from TIMERA1 side B */
    CC2650_PWM4 = 4, /* PWM output from TIMERA2 side A */
    CC2650_PWM5 = 5, /* PWM output from TIMERA2 side B */
    CC2650_PWM6 = 6, /* PWM output from TIMERA3 side A */
    CC2650_PWM7 = 7, /* PWM output from TIMERA3 side B */
    CC2650_PWMCOUNT
} CC2650_PWMName;

#ifdef __cplusplus
}
#endif

/* These #defines allow us to reuse TI-RTOS across other device families */
#define     Board_LED0              Board_LED1

#define     Board_BUTTON0           Board_KEY_LEFT
#define     Board_BUTTON1           Board_KEY_RIGHT

#define     Board_I2C0              Board_I2C
#define     Board_I2C_TMP           Board_I2C0
#define     Board_UART0             Board_UART
#define     Board_AES0              Board_AES
#define     Board_WATCHDOG0         Board_WATCHDOG

#define     Board_initGeneral()     PIN_init(BoardGpioInitTable)
#define     Board_initWatchdog()    Watchdog_init()

/* Board specific I2C addresses */
#define     Board_TMP006_ADDR       (0x44)
#define     Board_MPU9250_ADDR      (0x68)
#define     Board_SHT21_ADDR        (0x40)
#define     Board_OPT3001_ADDR      (0x45)
#define     Board_BMP280_ADDR       (0x77)


#endif /* __BOARD_H__ */
