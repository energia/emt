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
 
#define ARDUINO_MAIN
#include "wiring_private.h"
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/prcm.h>
#include <driverlib/rom_map.h>
#include <driverlib/pin.h>
#include <driverlib/timer.h>
#include "driverlib/adc.h"

#include <ti/drivers/PWM.h>

/*
 * analogWrite() support
 */

extern PWM_Config PWM_config[];

#define NOT_ON_TIMER    128

#define TIMERA0A 0
#define TIMERA0B 1
#define TIMERA1A 2
#define TIMERA1B 3
#define TIMERA2A 4
#define TIMERA2B 5
#define TIMERA3A 6
#define TIMERA3B 7

const uint8_t digital_pin_to_timer[] = {
	NOT_ON_TIMER,	/*  dummy */
	NOT_ON_TIMER,	/*  1  - 3.3V */
	NOT_ON_TIMER,	/*  2  - GPIO_03 */
	NOT_ON_TIMER,	/*  3  - GPIO_13 */
	NOT_ON_TIMER,	/*  4  - GPIO_12 */
	NOT_ON_TIMER,	/*  5  - GPIO_06 */
	NOT_ON_TIMER,	/*  6  - GPIO_04 */
	NOT_ON_TIMER,	/*  7  - GPIO_14 */
	NOT_ON_TIMER,	/*  8  - GPIO_07 */
	TIMERA3A, 		/*  9  -  port 0 */
	TIMERA3B, 		/*  10 -  port 1 */
	NOT_ON_TIMER,	/*  11 - GPIO_22 */
	NOT_ON_TIMER,	/*  12 - GPIO_01 */
	TIMERA1A, 		/*  13 -  port 0 */
	NOT_ON_TIMER,	/*  14 - GPIO_15 */
	NOT_ON_TIMER,	/*  15 - GPIO_16 */
	NOT_ON_TIMER,	/*  16 - RESET */
	NOT_ON_TIMER,	/*  17 - GPIO_31 */
	NOT_ON_TIMER,	/*  18 - GPIO_17 */
	NOT_ON_TIMER,	/*  19 - GPIO_28 */
	NOT_ON_TIMER,	/*  20 - GND */
	NOT_ON_TIMER,	/*  21 - 5V */
	NOT_ON_TIMER,	/*  22 - GND */
	NOT_ON_TIMER,	/*  23 - GPIO_02 */
	NOT_ON_TIMER,	/*  24 - GPIO_05 */
	NOT_ON_TIMER,	/*  25 - GPIO_03 */
	NOT_ON_TIMER,	/*  26 - GPIO_04 */
	NOT_ON_TIMER,	/*  27 - GPIO_08 */
	NOT_ON_TIMER,	/*  28 - GPIO_30 */
	TIMERA2B,		/*  29 -  port 1 */
	NOT_ON_TIMER,	/*  30 - GPIO_00 */
	TIMERA0A, 		/*  31 -  port 0 */
	NOT_ON_TIMER,	/*  32 - GPIO_23 */
	NOT_ON_TIMER,	/*  33 - GPIO_05 */
	NOT_ON_TIMER,	/*  34 - GPIO_07 */
	NOT_ON_TIMER,	/*  35 - GPIO_28 */
	TIMERA1A, 		/*  36 -  port 0 */
	TIMERA2B, 		/*  37 -  port 1 */
	TIMERA0A, 		/*  38 -  port 0 */
	TIMERA3A, 		/*  39 -  port 0 */
	TIMERA3B, 		/*  40 -  port 1 */
};

#define NOT_A_PIN       0

const uint16_t digital_pin_to_pin_num[] = {
	NOT_A_PIN,	/*  dummy */
	NOT_A_PIN,	/*  1  - 3.3V */
	PIN_58,		/*  2  - GPIO_03 */
	PIN_04,		/*  3  - GPIO_13 */
	PIN_03,		/*  4  - GPIO_12 */
	PIN_61,		/*  5  - GPIO_06 */
	PIN_59,		/*  6  - GPIO_04 */
	PIN_05,		/*  7  - GPIO_14 */
	PIN_62,		/*  8  - GPIO_07 */
	PIN_01,		/*  9  - GPIO_10 */
	PIN_02,		/*  10 - GPIO_11 */
	PIN_15,		/*  11 - GPIO_22 */
	PIN_55,		/*  12 - GPIO_01 */
	PIN_21,		/*  13 - GPIO_25 */
	PIN_06,		/*  14 - GPIO_15 */
	PIN_07,		/*  15 - GPIO_16 */
	NOT_A_PIN,	/*  16 - RESET */
	PIN_45,		/*  17 - GPIO_31 */
	PIN_08,		/*  18 - GPIO_17 */
	PIN_18,		/*  19 - GPIO_28 */
	NOT_A_PIN,	/*  20 - GND */
	NOT_A_PIN,	/*  21 - 5V */
	NOT_A_PIN,	/*  22 - GND */
	PIN_57,		/*  23 - GPIO_02 */
	PIN_60,		/*  24 - GPIO_05 */
	PIN_58,		/*  25 - GPIO_03 */
	PIN_59,		/*  26 - GPIO_04 */
	PIN_63,		/*  27 - GPIO_08 */
	PIN_53,		/*  28 - GPIO_30 */
	PIN_64,		/*  29 - GPIO_09 */
	PIN_50,		/*  30 - GPIO_00 */
	PIN_17,		/*  31 - GPIO_24 */
	PIN_16,		/*  32 - GPIO_23 */
	PIN_60,		/*  33 - GPIO_05 */
	PIN_62,		/*  34 - GPIO_07 */
	PIN_18,		/*  35 - GPIO_28 */
	PIN_21,		/*  36 - GPIO_25 */
	PIN_64,		/*  37 - GPIO_09 */
	PIN_17,		/*  38 - GPIO_24 */
	PIN_01,		/*  39 - GPIO_10 */
	PIN_02		/*  40 - GPIO_11 */
};

/*
 * For the CC3200, the timers used for PWM are clocked at 80MHz.
 * The period is set to 2.04ms in the PWM_open() calls in Board_init().
 * The PWM objects are configured for PWM_DUTY_COUNTS mode to minimize
 * the PWM_setDuty() processing overhead.
 * The 2.04ms period yields a period count of 163200.
 * The Arduino analogWrite() API takes a value of 0-255 for the duty cycle.
 * The PWM scale factor is then 163200 / 255 = 640
 */

#define PWM_SCALE_FACTOR 163200/255

void analogWrite(uint8_t pin, int val) 
{
    uint8_t timer = digital_pin_to_timer[pin];

    if (timer == NOT_ON_TIMER) {
        return;
    }

    /* re-configure pin if necessary */
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_UNUSED &&
             digital_pin_to_pin_function[pin] != PIN_FUNC_ANALOG_OUTPUT) {
        uint16_t pnum = digital_pin_to_pin_num[pin]; 

        switch(timer) {
            /* PWM0/1 */
            case TIMERA0A:
            case TIMERA0B:
                MAP_PinTypeTimer(pnum, PIN_MODE_5);
                break;
            /* PWM2/3 */
            case TIMERA1A:
            case TIMERA1B:
                MAP_PinTypeTimer(pnum, PIN_MODE_9);
                break;
            /* PWM4/5 */
            case TIMERA2A:
            case TIMERA2B:
                MAP_PinTypeTimer(pnum, PIN_MODE_3);
                break;
            /* PWM6/7 */
            case TIMERA3A:
            case TIMERA3B:
                MAP_PinTypeTimer(pnum, PIN_MODE_3);
                break;
        }

        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_OUTPUT;
    }

    PWM_setDuty((PWM_Handle)&(PWM_config[timer]), (val * PWM_SCALE_FACTOR));
}

/*
 * This internal API is used to de-configure a pin that has been
 * put in analogWrite() mode. 
 *
 * It will free up the pin's PWM resource after
 * it is no longer being used to support analogWrite() on a different
 * pin. It is called by pinMap() when a pin's function is being modified.
 */
void stopAnalogWrite(uint8_t pin)
{
    /* stop the timer */
    analogWrite(pin, 0);
}

/*
 * analogRead() support
 */

#define NOT_ON_ADC      0xFF

static int8_t analogReadShift = 4;

static const uint32_t digital_pin_to_analog_in[] = {
    NOT_ON_ADC,     /*  dummy   */
    NOT_ON_ADC,		/*  1	*/
    ADC_CH_1,		/*  2 - P58	*/
    NOT_ON_ADC,     /*  3	*/
    NOT_ON_ADC, 	/*  4	*/
    NOT_ON_ADC, 	/*  5	*/
    ADC_CH_2,		/*  6 - P59	*/
    NOT_ON_ADC,		/*  7 	*/
    NOT_ON_ADC, 	/*  8 	*/
    NOT_ON_ADC, 	/*  9 	*/
    NOT_ON_ADC, 	/*  10 	*/
    NOT_ON_ADC, 	/*  11 	*/
    NOT_ON_ADC, 	/*  12 	*/
    NOT_ON_ADC, 	/*  13 	*/
    NOT_ON_ADC, 	/*  14 	*/
    NOT_ON_ADC,		/*  15	*/
    NOT_ON_ADC,     /*  16 	*/
    NOT_ON_ADC,     /*  17 	*/
    NOT_ON_ADC, 	/*  18 	*/
    NOT_ON_ADC,     /*  19 	*/
    NOT_ON_ADC, 	/*  20 	*/
    NOT_ON_ADC, 	/*  21 	*/
    NOT_ON_ADC, 	/*  22 	*/
    ADC_CH_0,		/*  23 - P57	*/
    ADC_CH_3,		/*  24 - P60	*/
    NOT_ON_ADC,		/*  25 	*/
    NOT_ON_ADC,		/*  26 	*/
    NOT_ON_ADC, 	/*  27 	*/
    NOT_ON_ADC, 	/*  28 	*/
    NOT_ON_ADC, 	/*  29 	*/
    NOT_ON_ADC,     /*  30 	*/
    NOT_ON_ADC, 	/*  31 	*/
    NOT_ON_ADC,     /*  32 	*/
    NOT_ON_ADC,     /*  33 	*/
    NOT_ON_ADC,     /*  34 	*/
    NOT_ON_ADC,     /*  35 	*/
    NOT_ON_ADC,     /*  36 	*/
    NOT_ON_ADC,     /*  37 	*/
    NOT_ON_ADC,  	/*  38 	*/
    NOT_ON_ADC,		/*  39 	*/
    NOT_ON_ADC,     /*  40 	*/
};

/*
 * \brief           Reads an analog value from the pin specified.
 * \param[in] pin   The pin number to read from.
 * \return          A 16-bit integer containing a 12-bit sample from the ADC.
 */
uint16_t analogRead(uint8_t pin)
{
    uint32_t channel = digital_pin_to_analog_in[pin];

    if (channel == NOT_ON_ADC) { // invalid ADC pin
        return (0);
    }

    /* re-configure pin if necessary */
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_ANALOG_INPUT) {
        // Pinmux the pin to be analog
        MAP_PinTypeADC(digital_pin_to_pin_num[pin], 0xff);

        // Enable the channel
        MAP_ADCChannelEnable(ADC_BASE, channel);

        // Enable ADC module
        MAP_ADCEnable(ADC_BASE);

        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_INPUT;
    }

    // Get the sample
    uint16_t sample = 0;
    while (true){
        if (MAP_ADCFIFOLvlGet(ADC_BASE, channel)) {
            sample = (MAP_ADCFIFORead(ADC_BASE, channel) & 0x3ffc) >> analogReadShift;
            break;
        }
    }

    // Disable ADC module to save power
    MAP_ADCDisable(ADC_BASE);

    // Disable the channel
    MAP_ADCChannelDisable(ADC_BASE, channel);

    return (sample);
}

/*
 * This internal API is used to de-configure a pin that has been
 * put in analogRead() mode.
 *
 * It is called by pinMap() when a pin's function is
 * being modified.
 */
void stopAnalogRead(uint8_t pin)
{
}

/*
 * \brief sets the number of bits to shift the value read by ADCFIFORead()
 */
void analogReadResolution(uint16_t bits)
{
    analogReadShift = 14 - bits;
}
