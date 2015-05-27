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

#include <ti/runtime/wiring/cc3200/wiring_private.h>

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>

#include <driverlib/pin.h>
#include <driverlib/timer.h>
#include "driverlib/adc.h"

uint8_t digital_pin_to_pin_function[] = {
    PIN_FUNC_UNUSED,    /*  dummy */

    PIN_FUNC_UNUSED,    /*  1  - VDD */
    PIN_FUNC_UNUSED,    /*  2  - GND */
    PIN_FUNC_UNUSED,    /*  3  - GPIO_12 SCL */
    PIN_FUNC_UNUSED,    /*  4  - GPIO_13 SDA */
    PIN_FUNC_UNUSED,    /*  5  - GPIO_08 DP12 AUDIO FS */
    PIN_FUNC_UNUSED,    /*  6  - GPIO_30 DP7 AUDIO CLK */
    PIN_FUNC_UNUSED,    /*  7  - GPIO_17 DP11 CSN */
    PIN_FUNC_UNUSED,    /*  8  - GPIO_31 DP6 */
    PIN_FUNC_UNUSED,    /*  9  - VDD */
    PIN_FUNC_UNUSED,    /*  10 - GPIO_01 DP5 UART TX */

    PIN_FUNC_UNUSED,    /*  11 - GPIO_16 DP10 MOSI */
    PIN_FUNC_UNUSED,    /*  12 - GPIO_01 DP4 UART_RX */
    PIN_FUNC_UNUSED,    /*  13 - GPIO_15 DP9 MISO */
    PIN_FUNC_UNUSED,    /*  14 - GPIO_25 DP3 SOP2 */
    PIN_FUNC_UNUSED,    /*  15 - GPIO_14 DP8 SCLK */
    PIN_FUNC_UNUSED,    /*  16 - GPIO_09 DP2 */
    PIN_FUNC_UNUSED,    /*  17 - GPIO_05 DP ID */
    PIN_FUNC_UNUSED,    /*  18 - GPIO_04 DP1 BUTTON2 */
    PIN_FUNC_UNUSED,    /*  19 - POWER GOOD */
    PIN_FUNC_UNUSED,    /*  20 - GPIO_10 DP0/LED2 */

    PIN_FUNC_UNUSED,    /*  21 - GPIO_00 AUDIO DI */
    PIN_FUNC_UNUSED,    /*  22 - GPIO_03 BATTMON */
    PIN_FUNC_UNUSED,    /*  23 - GPIO_06 BUZZER */
    PIN_FUNC_UNUSED,    /*  24 - GPIO_07 REED */
    PIN_FUNC_UNUSED,    /*  25 - GPIO_11 BUTTON1 */
    PIN_FUNC_UNUSED,    /*  26 - GPIO_22 TMP RDY */
    PIN_FUNC_UNUSED,    /*  27 - GPIO_23 TDI/LED1 */
    PIN_FUNC_UNUSED,    /*  28 - GPIO_24 TDO/MPU INT */
    PIN_FUNC_UNUSED,    /*  29 - GPIO_28 MIC PWR */
    PIN_FUNC_UNUSED,    /*  30 - GPIO_29 JTAG TMS */
};

const uint8_t digital_pin_to_timer[] = {
    NOT_ON_TIMER,   /*  dummy */

    NOT_ON_TIMER,   /*  1  - VDD */
    NOT_ON_TIMER,   /*  2  - GND */
    NOT_ON_TIMER,   /*  3  - GPIO_12 SCL */
    NOT_ON_TIMER,   /*  4  - GPIO_13 SDA */
    NOT_ON_TIMER,   /*  5  - GPIO_08 DP12 AUDIO FS */
    NOT_ON_TIMER,   /*  6  - GPIO_30 DP7 AUDIO CLK */
    NOT_ON_TIMER,   /*  7  - GPIO_17 DP11 CSN */
    NOT_ON_TIMER,   /*  8  - GPIO_31 DP6 */
    NOT_ON_TIMER,   /*  9  - VDD */
    NOT_ON_TIMER,   /*  10 - GPIO_01 DP5 UART TX */

    NOT_ON_TIMER,   /*  11 - GPIO_16 DP10 MOSI */
    NOT_ON_TIMER,   /*  12 - GPIO_01 DP4 UART_RX */
    NOT_ON_TIMER,   /*  13 - GPIO_15 DP9 MISO */
    TIMERA1A,       /*  14 - GPIO_25 DP3 SOP2 */
    NOT_ON_TIMER,   /*  15 - GPIO_14 DP8 SCLK */
    TIMERA2B,       /*  16 - GPIO_09 DP2 */
    NOT_ON_TIMER,   /*  17 - GPIO_05 DP ID */
    NOT_ON_TIMER,   /*  18 - GPIO_04 DP1 BUTTON2 */
    NOT_ON_TIMER,   /*  19 - POWER GOOD */
    TIMERA3A,       /*  20 - GPIO_10 DP0/LED2 */

    NOT_ON_TIMER,   /*  21 - GPIO_00 AUDIO DI */
    NOT_ON_TIMER,   /*  22 - GPIO_03 BATTMON */
    NOT_ON_TIMER,   /*  23 - GPIO_06 BUZZER */
    NOT_ON_TIMER,   /*  24 - GPIO_07 REED */
    TIMERA3B,       /*  25 - GPIO_11 BUTTON1 */
    NOT_ON_TIMER,   /*  26 - GPIO_22 TMP RDY */
    NOT_ON_TIMER,   /*  27 - GPIO_23 TDI/LED1 */
    TIMERA0A,       /*  28 - GPIO_24 TDO/MPU INT */
    NOT_ON_TIMER,   /*  29 - GPIO_28 MIC PWR */
    NOT_ON_TIMER,   /*  30 - GPIO_29 JTAG TMS */
};

const uint16_t digital_pin_to_pin_num[] = {
    NOT_A_PIN,  /*  dummy */
    
    NOT_A_PIN,  /*  1  - VDD */
    NOT_A_PIN,  /*  2  - GND */
    PIN_03,     /*  3  - GPIO_12 SCL */
    PIN_04,     /*  4  - GPIO_13 SDA */
    PIN_63,     /*  5  - GPIO_08 DP12 AUDIO FS */
    PIN_53,     /*  6  - GPIO_30 DP7 AUDIO CLK */
    PIN_08,     /*  7  - GPIO_17 DP11 CSN */
    PIN_45,     /*  8  - GPIO_31 DP6 */
    NOT_A_PIN,  /*  9  - VDD */
    PIN_55,     /*  10 - GPIO_01 DP5 UART TX */

    PIN_07,     /*  11 - GPIO_16 DP10 MOSI */
    PIN_55,     /*  12 - GPIO_01 DP4 UART_RX */
    PIN_06,     /*  13 - GPIO_15 DP9 MISO */
    PIN_21,     /*  14 - GPIO_25 DP3 SOP2 */
    PIN_05,     /*  15 - GPIO_14 DP8 SCLK */
    PIN_64,     /*  16 - GPIO_09 DP2 */
    PIN_60,     /*  17 - GPIO_05 DP ID */
    PIN_59,     /*  18 - GPIO_04 DP1 BUTTON2 */
    NOT_A_PIN,  /*  19 - POWER GOOD */
    PIN_01,     /*  20 - GPIO_10 DP0/LED2 */

    PIN_50,     /*  21 - GPIO_00 AUDIO DI */
    PIN_58,     /*  22 - GPIO_03 BATTMON */
    PIN_61,     /*  23 - GPIO_06 BUZZER */
    PIN_62,     /*  24 - GPIO_07 REED */
    PIN_02,     /*  25 - GPIO_11 BUTTON1 */
    PIN_15,     /*  26 - GPIO_22 TMP RDY */
    PIN_16,     /*  27 - GPIO_23 TDI/LED1 */
    PIN_17,     /*  28 - GPIO_24 TDO/MPU INT */
    PIN_18,     /*  29 - GPIO_28 MIC PWR */
    PIN_20,     /*  30 - GPIO_29 JTAG TMS */
};

