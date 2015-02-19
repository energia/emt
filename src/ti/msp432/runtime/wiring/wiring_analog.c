/*
 ************************************************************************
 *  wiring_analog.c
 *
 *  Energia core files for MSP430
 *  Copyright (c) 2014 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring_analog.c - analog input and output
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 */
#define ARDUINO_MAIN
#include "wiring_private.h"
#include <rom.h>
#include <rom_map.h>
#include <timer_a.h>
#include <adc14.h>

#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO2.h>
#include <ti/drivers/gpio/GPIO2MSP432.h>

const uint16_t pwm_to_port_pin[] = {
    GPIO2MSP432_P2_4,
    GPIO2MSP432_P2_5,
    GPIO2MSP432_P2_6,
    GPIO2MSP432_P2_7,
    GPIO2MSP432_P5_6,
    GPIO2MSP432_P5_7,
    GPIO2MSP432_P6_6,
    GPIO2MSP432_P6_7,
};

const uint8_t digital_pin_to_pwm_index[] = {
    NOT_ON_TIMER,   /*  dummy */
    NOT_ON_TIMER,   /*  1  - 3.3V */
    NOT_ON_TIMER,   /*  2  - P6.0_A15 */
    NOT_ON_TIMER,   /*  3  - P3.2_URXD */
    NOT_ON_TIMER,   /*  4  - P3.3_UTXD */
    NOT_ON_TIMER,   /*  5  - P4.1_IO_A12 */
    NOT_ON_TIMER,   /*  6  - P4.3_A10 */
    NOT_ON_TIMER,   /*  7  - P1.5_SPICLK */
    NOT_ON_TIMER,   /*  8  - P4.6_IO_A7 */
    NOT_ON_TIMER,   /*  9  - P6.5_I2CSCL */
    NOT_ON_TIMER,   /*  10 - P6.4_I2CSDA */
    NOT_ON_TIMER,   /*  11 - P3.6_IO */
    NOT_ON_TIMER,   /*  12 - P5.2_IO */
    NOT_ON_TIMER,   /*  13 - P5.0_IO */
    NOT_ON_TIMER,   /*  14 - P1.7_SPIMISO */
    NOT_ON_TIMER,   /*  15 - P1.6_SPIMOSI */
    NOT_ON_TIMER,   /*  16 - RESET */
    5,              /*  17 - P5.7_IO */
    NOT_ON_TIMER,   /*  18 - P3.0_IO */
    1,              /*  19 - P2.5_IO_PWM */
    NOT_ON_TIMER,   /*  20 - GND */
    NOT_ON_TIMER,   /*  21 - 5V */
    NOT_ON_TIMER,   /*  22 - GND */
    NOT_ON_TIMER,   /*  23 - P6.1_A14 */
    NOT_ON_TIMER,   /*  24 - P4.0_A13 */
    NOT_ON_TIMER,   /*  25 - P4.2_A11 */
    NOT_ON_TIMER,   /*  26 - P4.4_A9 */
    NOT_ON_TIMER,   /*  27 - P4.5_A8 */
    NOT_ON_TIMER,   /*  28 - P4.7_A6 */
    NOT_ON_TIMER,   /*  29 - P5.4_IO */
    NOT_ON_TIMER,   /*  30 - P5.5_IO */
    NOT_ON_TIMER,   /*  31 - P3.7_IO */
    NOT_ON_TIMER,   /*  32 - P3.5_IO */
    NOT_ON_TIMER,   /*  33 - P5.1_IO */
    NOT_ON_TIMER,   /*  34 - P2.3_IO */
    7,              /*  35 - P6.7_IO_CAPT */
    6,              /*  36 - P6.6_IO_CAPT */
    4,              /*  37 - P5.6_PWM */
    0,              /*  38 - P2.4_PWM */
    2,              /*  39 - P2.6_PWM */
    3,              /*  40 - P2.7_PWM */
    NOT_ON_TIMER,   /*  41 - P1.1 SW1 */
    NOT_ON_TIMER,   /*  42 - P1.4 SW2 */
    NOT_ON_TIMER,   /*  43 - P2.0 RED_LED */
    NOT_ON_TIMER,   /*  44 - P2.1 GREEN_LED */
    NOT_ON_TIMER,   /*  45 - P2.2 BLUE_LED */
    NOT_ON_TIMER,   /*  46 - P1.0 LED1 */
};

extern PWM_Config PWM_config[];

/*
 * For the MSP432, the timers used for PWM are clocked at 12MHz.
 * The period is set to 2.04ms in the PWM_open() calls in Board_init().
 * The PWM objects are configured for PWM_DUTY_COUNTS mode to minimize
 * the PWM_setDuty() processing overhead.
 * The 2.04ms period yields a period count of 24480.
 * The Arduino analogWrite() API takes a value of 0-255 for the duty cycle.
 * The PWM scale factor is then 24480 / 255 = 96
 */

#define PWM_SCALE_FACTOR 24480/255

void analogWrite(uint8_t pin, int val)
{
    uint8_t pwmIndex = digital_pin_to_pwm_index[pin];

    if (pwmIndex == NOT_ON_TIMER) {
        return;
    }

    /* re-configure pin if necessary */
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_ANALOG_OUTPUT) {
        uint_fast8_t port;
        uint_fast16_t pinMask;

        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_OUTPUT;

        port = pwm_to_port_pin[pwmIndex] >> 8;
        pinMask = pwm_to_port_pin[pwmIndex] & 0xff;
        /* Enable PWM output on GPIO pins */
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(port, pinMask,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);
    }

    PWM_setDuty((PWM_Handle)&(PWM_config[pwmIndex]), (val * PWM_SCALE_FACTOR));
}

const uint16_t adc_to_port_pin[] = {
    GPIO2MSP432_P5_5,  /* A0 */
    GPIO2MSP432_P5_4,  /* A1 */
    GPIO2MSP432_P5_3,  /* A2 */
    GPIO2MSP432_P5_2,  /* A3 */
    GPIO2MSP432_P5_1,  /* A4 */
    GPIO2MSP432_P5_0,  /* A5 */
    GPIO2MSP432_P4_7,  /* A6 */
    GPIO2MSP432_P4_6,  /* A7 */
    GPIO2MSP432_P4_5,  /* A8 */
    GPIO2MSP432_P4_4,  /* A9 */
    GPIO2MSP432_P4_3,  /* A10 */
    GPIO2MSP432_P4_2,  /* A11 */
    GPIO2MSP432_P4_1,  /* A12 */
    GPIO2MSP432_P4_0,  /* A13 */
    GPIO2MSP432_P6_1,  /* A14 */
    GPIO2MSP432_P6_0,  /* A15 */
};

const uint8_t digital_pin_to_adc_index[] = {
    NOT_ON_ADC,     /*  dummy */
    NOT_ON_ADC,     /*  1  - 3.3V */
    ADC14INCH_15,   /*  2  - P6.0_A15 */
    NOT_ON_ADC,     /*  3  - P3.2_URXD */
    NOT_ON_ADC,     /*  4  - P3.3_UTXD */
    ADC14INCH_12,   /*  5  - P4.1_IO_A12 */
    ADC14INCH_10,   /*  6  - P4.3_A10 */
    NOT_ON_ADC,     /*  7  - P1.5_SPICLK */
    ADC14INCH_7,    /*  8  - P4.6_IO_A7 */
    NOT_ON_ADC,     /*  9  - P6.5_I2CSCL */
    NOT_ON_ADC,     /*  10 - P6.4_I2CSDA */
    NOT_ON_ADC,     /*  11 - P3.6_IO */
    ADC14INCH_3,    /*  12 - P5.2_IO */
    ADC14INCH_5,    /*  13 - P5.0_IO */
    NOT_ON_ADC,     /*  14 - P1.7_SPIMISO */
    NOT_ON_ADC,     /*  15 - P1.6_SPIMOSI */
    NOT_ON_ADC,     /*  16 - RESET */
    NOT_ON_ADC,     /*  17 - P5.7_IO */
    NOT_ON_ADC,     /*  18 - P3.0_IO */
    NOT_ON_ADC,     /*  19 - P2.5_IO_PWM */
    NOT_ON_ADC,     /*  20 - GND */
    NOT_ON_ADC,     /*  21 - 5V */
    NOT_ON_ADC,     /*  22 - GND */
    ADC14INCH_14,   /*  23 - P6.1_A14 */
    ADC14INCH_13,   /*  24 - P4.0_A13 */
    ADC14INCH_11,   /*  25 - P4.2_A11 */
    ADC14INCH_9,    /*  26 - P4.4_A9 */
    ADC14INCH_8,    /*  27 - P4.5_A8 */
    ADC14INCH_6,    /*  28 - P4.7_A6 */
    ADC14INCH_1,    /*  29 - P5.4_IO */
    ADC14INCH_0,    /*  30 - P5.5_IO */
    NOT_ON_ADC,     /*  31 - P3.7_IO */
    NOT_ON_ADC,     /*  32 - P3.5_IO */
    ADC14INCH_4,    /*  33 - P5.1_IO */
    NOT_ON_ADC,     /*  34 - P2.3_IO */
    NOT_ON_ADC,     /*  35 - P6.7_IO_CAPT */
    NOT_ON_ADC,     /*  36 - P6.6_IO_CAPT */
    NOT_ON_ADC,     /*  37 - P5.6_PWM */
    NOT_ON_ADC,     /*  38 - P2.4_PWM */
    NOT_ON_ADC,     /*  39 - P2.6_PWM */
    NOT_ON_ADC,     /*  40 - P2.7_PWM */
    NOT_ON_ADC,     /*  41 - P1.1 SW1 */
    NOT_ON_ADC,     /*  42 - P1.4 SW2 */
    NOT_ON_ADC,     /*  43 - P2.0 RED_LED */
    NOT_ON_ADC,     /*  44 - P2.1 GREEN_LED */
    NOT_ON_ADC,     /*  45 - P2.2 BLUE_LED */
    NOT_ON_ADC,     /*  46 - P1.0 LED1 */
};

static int8_t analogReadShift = 4;
static bool adc_module_enabled = false;

/*
 * \brief           Reads an analog value from the pin specified.
 * \param[in] pin   The pin number to read from.
 * \return          A 16-bit integer containing a 12-bit sample from the ADC.
 */
uint16_t analogRead(uint8_t pin)
{
    uint8_t adcIndex = digital_pin_to_adc_index[pin];
    uint32_t adcMem, adcInt;
    uint16_t sample = 0;
    uint64_t status;

    if (adcIndex == NOT_ON_ADC) return;

    adcMem = 1 << adcIndex;
    adcInt = 1 << adcIndex;

    /* re-configure pin if necessary */
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_ANALOG_INPUT) {
        uint_fast8_t port;
        uint_fast16_t pinMask;

        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_INPUT;
        /* initialize top level ADC module if it hasn't been already */
        if (adc_module_enabled == false) {
            /* Initializing ADC (MCLK/1/1) */
            ADC14_enableModule();
            ADC14_initModule(ADC_CLOCKSOURCE_MCLK,
                             ADC_PREDIVIDER_1,
                             ADC_DIVIDER_1,
                             0);
            adc_module_enabled = true;
        }

        port = adc_to_port_pin[adcIndex] >> 8;
        pinMask = adc_to_port_pin[adcIndex] & 0xff;

        /* Setting up GPIO pins as analog inputs (and references) */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port, pinMask,
                                        GPIO_TERTIARY_MODULE_FUNCTION);
        /* Enabling sample timer in auto iteration mode and interrupts */
        MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    }

    /* clear out last sample */
    MAP_ADC14_clearInterruptFlag(adcInt);

    /* Configuring ADC Memory in single conversion mode
     * with use of internal VSS as references */
    MAP_ADC14_configureSingleSampleMode(adcMem, false);
    MAP_ADC14_configureConversionMemory(adcMem,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    adcIndex, false);
    /* start new conversion */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    status = MAP_ADC14_getInterruptStatus();

    while ((adcInt & status) == 0) {
        status = MAP_ADC14_getInterruptStatus();
    }

    sample = MAP_ADC14_getResult(adcMem);
    MAP_ADC14_clearInterruptFlag(adcInt);

    return (sample);
}

/*
 * \brief sets the number of bits to shift the value read by ADCFIFORead()
 */
void analogReadResolution(uint8_t bits)
{
    analogReadShift = 14 - bits;
}
