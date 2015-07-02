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

uint8_t digital_pin_to_pin_function[] = {
    PIN_FUNC_UNUSED,    /*  dummy */
    PIN_FUNC_UNUSED,    /*  1  - VDD */
    PIN_FUNC_UNUSED,    /*  2  - GND */
    PIN_FUNC_UNUSED,    /*  3  - DIO_06 SCL */
    PIN_FUNC_UNUSED,    /*  4  - DIO_05 SDA */
    PIN_FUNC_UNUSED,    /*  5  - DIO_16 DP12/AUDIO FS/TD0 */
    PIN_FUNC_UNUSED,    /*  6  - DIO_11 DP7/AUDIO CLK */
    PIN_FUNC_UNUSED,    /*  7  - DIO_20 DP11/CSN */
    PIN_FUNC_UNUSED,    /*  8  - DIO_22 DP6/AUDIO DO */
    PIN_FUNC_UNUSED,    /*  9  - VDD */
    PIN_FUNC_UNUSED,    /*  10 - DIO_29 DP5/UART_TX */
    PIN_FUNC_UNUSED,    /*  11 - DIO_19 DP10/MOSI */
    PIN_FUNC_UNUSED,    /*  12 - DIO_28 DP4/UART_RX */
    PIN_FUNC_UNUSED,    /*  13 - DIO_18 DP9/MISO */
    PIN_FUNC_UNUSED,    /*  14 - DIO_27 DP3 */
    PIN_FUNC_UNUSED,    /*  15 - DIO_17 DP8/SCLK/TDI */
    PIN_FUNC_UNUSED,    /*  16 - DIO_23 DP2  */
    PIN_FUNC_UNUSED,    /*  17 - DIO_30 DP_ID */
    PIN_FUNC_UNUSED,    /*  18 - DIO_24 DP1 */
    PIN_FUNC_UNUSED,    /*  19 - POWER_GOOD */
    PIN_FUNC_UNUSED,    /*  20 - DIO_25 DP0 */
    PIN_FUNC_UNUSED,    /*  21 - DIO_02 AUDIO DI */
    PIN_FUNC_UNUSED,    /*  22 - DIO_12 MPU PWR */
    PIN_FUNC_UNUSED,    /*  23 - DIO_21 BUZZER */
    PIN_FUNC_UNUSED,    /*  24 - DIO_03 REED */
    PIN_FUNC_UNUSED,    /*  25 - DIO_04 BUTTON1 */
    PIN_FUNC_UNUSED,    /*  26 - DIO_01 TMP RDY */
    PIN_FUNC_UNUSED,    /*  27 - DIO_10 LED1 */
    PIN_FUNC_UNUSED,    /*  28 - DIO_07 MPU INT */
    PIN_FUNC_UNUSED,    /*  29 - DIO_13 MIC PWR */
    PIN_FUNC_UNUSED,    /*  30 - DIO_00 BUTTON2 */
    PIN_FUNC_UNUSED,    /*  31 - DIO_15 LED2 */
};

const uint8_t digital_pin_to_timer[] = {
    NOT_ON_TIMER,   /*  dummy */
    NOT_ON_TIMER,   /*  1  - VDD */
    NOT_ON_TIMER,   /*  2  - GND */
    NOT_ON_TIMER,   /*  3  - DIO_06 SCL */
    NOT_ON_TIMER,   /*  4  - DIO_05 SDA */
    NOT_ON_TIMER,   /*  5  - DIO_16 DP12/AUDIO FS/TD0 */
    NOT_ON_TIMER,   /*  6  - DIO_11 DP7/AUDIO CLK */
    NOT_ON_TIMER,   /*  7  - DIO_20 DP11/CSN */
    NOT_ON_TIMER,   /*  8  - DIO_22 DP6/AUDIO DO */
    NOT_ON_TIMER,   /*  9  - VDD */
    NOT_ON_TIMER,   /*  10 - DIO_29 DP5/UART_TX */
    NOT_ON_TIMER,   /*  11 - DIO_19 DP10/MOSI */
    NOT_ON_TIMER,   /*  12 - DIO_28 DP4/UART_RX */
    NOT_ON_TIMER,   /*  13 - DIO_18 DP9/MISO */
    NOT_ON_TIMER,   /*  14 - DIO_27 DP3 */
    NOT_ON_TIMER,   /*  15 - DIO_17 DP8/SCLK/TDI */
    NOT_ON_TIMER,   /*  16 - DIO_23 DP2  */
    NOT_ON_TIMER,   /*  17 - DIO_30 DP_ID */
    NOT_ON_TIMER,   /*  18 - DIO_24 DP1 */
    NOT_ON_TIMER,   /*  19 - POWER_GOOD */
    NOT_ON_TIMER,   /*  20 - DIO_25 DP0 */
    NOT_ON_TIMER,   /*  21 - DIO_02 AUDIO DI */
    NOT_ON_TIMER,   /*  22 - DIO_12 MPU PWR */
    NOT_ON_TIMER,   /*  23 - DIO_21 BUZZER */
    NOT_ON_TIMER,   /*  24 - DIO_03 REED */
    NOT_ON_TIMER,   /*  25 - DIO_04 BUTTON1 */
    NOT_ON_TIMER,   /*  26 - DIO_01 TMP RDY */
    NOT_ON_TIMER,   /*  27 - DIO_10 LED1 */
    NOT_ON_TIMER,   /*  28 - DIO_07 MPU INT */
    NOT_ON_TIMER,   /*  29 - DIO_13 MIC PWR */
    NOT_ON_TIMER,   /*  30 - DIO_00 BUTTON2 */
    NOT_ON_TIMER,   /*  31 - DIO_15 LED2 */
};

const uint16_t digital_pin_to_pin_num[] = {
    NOT_A_PIN,  /*  dummy */
    NOT_A_PIN,  /*  1  - VDD */
    NOT_A_PIN,  /*  2  - GND */
    NOT_A_PIN,  /*  3  - DIO_06 SCL */
    NOT_A_PIN,  /*  4  - DIO_05 SDA */
    NOT_A_PIN,  /*  5  - DIO_16 DP12/AUDIO FS/TD0 */
    NOT_A_PIN,  /*  6  - DIO_11 DP7/AUDIO CLK */
    NOT_A_PIN,  /*  7  - DIO_20 DP11/CSN */
    NOT_A_PIN,  /*  8  - DIO_22 DP6/AUDIO DO */
    NOT_A_PIN,  /*  9  - VDD */
    NOT_A_PIN,  /*  10 - DIO_29 DP5/UART_TX */
    NOT_A_PIN,  /*  11 - DIO_19 DP10/MOSI */
    NOT_A_PIN,  /*  12 - DIO_28 DP4/UART_RX */
    NOT_A_PIN,  /*  13 - DIO_18 DP9/MISO */
    NOT_A_PIN,  /*  14 - DIO_27 DP3 */
    NOT_A_PIN,  /*  15 - DIO_17 DP8/SCLK/TDI */
    NOT_A_PIN,  /*  16 - DIO_23 DP2  */
    NOT_A_PIN,  /*  17 - DIO_30 DP_ID */
    NOT_A_PIN,  /*  18 - DIO_24 DP1 */
    NOT_A_PIN,  /*  19 - POWER_GOOD */
    NOT_A_PIN,  /*  20 - DIO_25 DP0 */
    NOT_A_PIN,  /*  21 - DIO_02 AUDIO DI */
    NOT_A_PIN,  /*  22 - DIO_12 MPU PWR */
    NOT_A_PIN,  /*  23 - DIO_21 BUZZER */
    NOT_A_PIN,  /*  24 - DIO_03 REED */
    NOT_A_PIN,  /*  25 - DIO_04 BUTTON1 */
    NOT_A_PIN,  /*  26 - DIO_01 TMP RDY */
    NOT_A_PIN,  /*  27 - DIO_10 LED1 */
    NOT_A_PIN,  /*  28 - DIO_07 MPU INT */
    NOT_A_PIN,  /*  29 - DIO_13 MIC PWR */
    NOT_A_PIN,  /*  30 - DIO_00 BUTTON2 */
    NOT_A_PIN,  /*  31 - DIO_15 LED2 */
};

const uint32_t digital_pin_to_analog_in[] = {
    NOT_ON_ADC,     /*  dummy */
    NOT_ON_ADC,     /*  1  - VDD */
    NOT_ON_ADC,     /*  2  - GND */
    NOT_ON_ADC,     /*  3  - DIO_06 SCL */
    NOT_ON_ADC,     /*  4  - DIO_05 SDA */
    NOT_ON_ADC,     /*  5  - DIO_16 DP12/AUDIO FS/TD0 */
    NOT_ON_ADC,     /*  6  - DIO_11 DP7/AUDIO CLK */
    NOT_ON_ADC,     /*  7  - DIO_20 DP11/CSN */
    NOT_ON_ADC,     /*  8  - DIO_22 DP6/AUDIO DO */
    NOT_ON_ADC,     /*  9  - VDD */
    NOT_ON_ADC,     /*  10 - DIO_29 DP5/UART_TX */
    NOT_ON_ADC,     /*  11 - DIO_19 DP10/MOSI */
    NOT_ON_ADC,     /*  12 - DIO_28 DP4/UART_RX */
    NOT_ON_ADC,     /*  13 - DIO_18 DP9/MISO */
    NOT_ON_ADC,     /*  14 - DIO_27 DP3 */
    NOT_ON_ADC,     /*  15 - DIO_17 DP8/SCLK/TDI */
    NOT_ON_ADC,     /*  16 - DIO_23 DP2  */
    NOT_ON_ADC,     /*  17 - DIO_30 DP_ID */
    NOT_ON_ADC,     /*  18 - DIO_24 DP1 */
    NOT_ON_ADC,     /*  19 - POWER_GOOD */
    NOT_ON_ADC,     /*  20 - DIO_25 DP0 */
    NOT_ON_ADC,     /*  21 - DIO_02 AUDIO DI */
    NOT_ON_ADC,     /*  22 - DIO_12 MPU PWR */
    NOT_ON_ADC,     /*  23 - DIO_21 BUZZER */
    NOT_ON_ADC,     /*  24 - DIO_03 REED */
    NOT_ON_ADC,     /*  25 - DIO_04 BUTTON1 */
    NOT_ON_ADC,     /*  26 - DIO_01 TMP RDY */
    NOT_ON_ADC,     /*  27 - DIO_10 LED1 */
    NOT_ON_ADC,     /*  28 - DIO_07 MPU INT */
    NOT_ON_ADC,     /*  29 - DIO_13 MIC PWR */
    NOT_ON_ADC,     /*  30 - DIO_00 BUTTON2 */
    NOT_ON_ADC,     /*  31 - DIO_15 LED2 */
};
