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

#include "wiring_private.h"

uint8_t digital_pin_to_pin_function[] = {
    PIN_FUNC_UNUSED,    /*  dummy */
    PIN_FUNC_UNUSED,    /*  1  - 3.3V */
    PIN_FUNC_UNUSED,    /*  2  - P6.0_A15 */
    PIN_FUNC_UNUSED,    /*  3  - P3.2_URXD */
    PIN_FUNC_UNUSED,    /*  4  - P3.3_UTXD */
    PIN_FUNC_UNUSED,    /*  5  - P4.1_IO_A12 */
    PIN_FUNC_UNUSED,    /*  6  - P4.3_A10 */
    PIN_FUNC_UNUSED,    /*  7  - P1.5_SPICLK */
    PIN_FUNC_UNUSED,    /*  8  - P4.6_IO_A7 */
    PIN_FUNC_UNUSED,    /*  9  - P6.5_I2CSCL */
    PIN_FUNC_UNUSED,    /*  10 - P6.4_I2CSDA */

    PIN_FUNC_UNUSED,    /*  11 - P3.6_IO */
    PIN_FUNC_UNUSED,    /*  12 - P5.2_IO */
    PIN_FUNC_UNUSED,    /*  13 - P5.0_IO */
    PIN_FUNC_UNUSED,    /*  14 - P1.7_SPIMISO */
    PIN_FUNC_UNUSED,    /*  15 - P1.6_SPIMOSI */
    PIN_FUNC_UNUSED,    /*  16 - RESET */
    PIN_FUNC_UNUSED,    /*  17 - P5.7_IO */
    PIN_FUNC_UNUSED,    /*  18 - P3.0_IO */
    PIN_FUNC_UNUSED,    /*  19 - P2.5_IO_PWM */
    PIN_FUNC_UNUSED,    /*  20 - GND */

    PIN_FUNC_UNUSED,    /*  21 - 5V */
    PIN_FUNC_UNUSED,    /*  22 - GND */
    PIN_FUNC_UNUSED,    /*  23 - P6.1_A14 */
    PIN_FUNC_UNUSED,    /*  24 - P4.0_A13 */
    PIN_FUNC_UNUSED,    /*  25 - P4.2_A11 */
    PIN_FUNC_UNUSED,    /*  26 - P4.4_A9 */
    PIN_FUNC_UNUSED,    /*  27 - P4.5_A8 */
    PIN_FUNC_UNUSED,    /*  28 - P4.7_A6 */
    PIN_FUNC_UNUSED,    /*  29 - P5.4_IO */
    PIN_FUNC_UNUSED,    /*  30 - P5.5_IO */

    PIN_FUNC_UNUSED,    /*  31 - P3.7_IO */
    PIN_FUNC_UNUSED,    /*  32 - P3.5_IO */
    PIN_FUNC_UNUSED,    /*  33 - P5.1_IO */
    PIN_FUNC_UNUSED,    /*  34 - P2.3_IO */
    PIN_FUNC_UNUSED,    /*  35 - P6.7_IO_CAPT */
    PIN_FUNC_UNUSED,    /*  36 - P6.6_IO_CAPT */
    PIN_FUNC_UNUSED,    /*  37 - P5.6_PWM */
    PIN_FUNC_UNUSED,    /*  38 - P2.4_PWM */
    PIN_FUNC_UNUSED,    /*  39 - P2.6_PWM */
    PIN_FUNC_UNUSED,    /*  40 - P2.7_PWM */

    /* pins 41-56 */
    PIN_FUNC_UNUSED,    /*  41 - P8.5 */
    PIN_FUNC_UNUSED,    /*  42 - P9.0 */
    PIN_FUNC_UNUSED,    /*  43 - P8.4 */
    PIN_FUNC_UNUSED,    /*  44 - P8.2 */
    PIN_FUNC_UNUSED,    /*  45 - P9.2 */
    PIN_FUNC_UNUSED,    /*  46 - P6.2 */
    PIN_FUNC_UNUSED,    /*  47 - P7.3 */
    PIN_FUNC_UNUSED,    /*  48 - P7.1 */
    PIN_FUNC_UNUSED,    /*  49 - P9.4 */
    PIN_FUNC_UNUSED,    /*  40 - P9.6 */
    PIN_FUNC_UNUSED,    /*  51 - P8.0 */
    PIN_FUNC_UNUSED,    /*  52 - P7.4 */
    PIN_FUNC_UNUSED,    /*  53 - P7.6 */
    PIN_FUNC_UNUSED,    /*  54 - P10.0 */
    PIN_FUNC_UNUSED,    /*  55 - P10_2 */
    PIN_FUNC_UNUSED,    /*  56 - P10.4 */

    /* pins 57-72 */
    PIN_FUNC_UNUSED,    /*  57 - P8.6 */
    PIN_FUNC_UNUSED,    /*  58 - P8.7 */
    PIN_FUNC_UNUSED,    /*  59 - P9.1 */
    PIN_FUNC_UNUSED,    /*  60 - P8.3 */
    PIN_FUNC_UNUSED,    /*  61 - P5.3 */
    PIN_FUNC_UNUSED,    /*  62 - P9.3 */
    PIN_FUNC_UNUSED,    /*  63 - P6.3 */
    PIN_FUNC_UNUSED,    /*  64 - P7.2 */
    PIN_FUNC_UNUSED,    /*  65 - P7.0 */
    PIN_FUNC_UNUSED,    /*  66 - P9.5 */
    PIN_FUNC_UNUSED,    /*  67 - P9.7 */
    PIN_FUNC_UNUSED,    /*  68 - P7.5 */
    PIN_FUNC_UNUSED,    /*  69 - P7.7 */
    PIN_FUNC_UNUSED,    /*  70 - P10.1 */
    PIN_FUNC_UNUSED,    /*  71 - P10.3 */
    PIN_FUNC_UNUSED,    /*  72 - P10.5 */

    /* virtual pins 73-78 */
    PIN_FUNC_UNUSED,    /*  73 - P1.1 SW1 */
    PIN_FUNC_UNUSED,    /*  74 - P1.4 SW2 */
    PIN_FUNC_UNUSED,    /*  75 - P2.0 RED_LED */
    PIN_FUNC_UNUSED,    /*  76 - P2.1 GREEN_LED */
    PIN_FUNC_UNUSED,    /*  77 - P2.2 BLUE_LED */
    PIN_FUNC_UNUSED,    /*  78 - P1.0 LED1 */
};
