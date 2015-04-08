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
#include <rom.h>
#include <rom_map.h>
#include <timer_a.h>
#include <adc14.h>
#include <ref_a.h>
#include <pmap.h>
#include <gpio.h>

#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432.h>

#include <ti/sysbios/family/arm/m3/Hwi.h>

/*
 * analogWrite() support
 */

extern PWM_Config PWM_config[];

/* Mappable PWM Timer capture pins */
const uint8_t mappable_pwms[] = {
    PM_TA0CCR1A,
    PM_TA0CCR2A,
    PM_TA0CCR3A,
    PM_TA0CCR4A,
    PM_TA1CCR1A,
    PM_TA1CCR2A,
    PM_TA1CCR3A,
    PM_TA1CCR4A,
};

/* port number to PXMAP translation */
const uint8_t pxmap[] = {
    0,
    P1MAP,
    P2MAP,
    P3MAP,
    P4MAP,
    P5MAP,
    P6MAP,
    P7MAP,
};

#define NOT_IN_USE 0

/* Current PWM timer GPIO mappings */
uint16_t used_pwm_port_pins[] = {
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
};

#define NOT_MAPPABLE 0xffff

/*
 * Prior to a pin being used for analogWrite(), this table
 * contains the PIN IDs for achievable dynamic PWM mappings.
 *
 * While in use, those PIN IDs are replaced with the
 * corresponding index of the PWM resource that was mapped
 * to the pin.
 */
uint16_t digital_pin_to_pwm_index[] = {
    NOT_MAPPABLE,       /*  dummy */
    NOT_MAPPABLE,       /*  1  - 3.3V */
    NOT_MAPPABLE,       /*  2  - P6.0_A15 */
    GPIOMSP432_P3_2,    /*  3  - P3.2_URXD */
    GPIOMSP432_P3_3,    /*  4  - P3.3_UTXD */
    NOT_MAPPABLE,       /*  5  - P4.1_IO_A12 */
    NOT_MAPPABLE,       /*  6  - P4.3_A10 */
    NOT_MAPPABLE,       /*  7  - P1.5_SPICLK */
    NOT_MAPPABLE,       /*  8  - P4.6_IO_A7 */
    NOT_MAPPABLE,       /*  9  - P6.5_I2CSCL */
    NOT_MAPPABLE,       /*  10 - P6.4_I2CSDA */

    GPIOMSP432_P3_6,    /*  11 - P3.6_IO */
    NOT_MAPPABLE,       /*  12 - P5.2_IO */
    NOT_MAPPABLE,       /*  13 - P5.0_IO */
    NOT_MAPPABLE,       /*  14 - P1.7_SPIMISO */
    NOT_MAPPABLE,       /*  15 - P1.6_SPIMOSI */
    NOT_MAPPABLE,       /*  16 - RESET */
    NOT_MAPPABLE,       /*  17 - P5.7_IO */
    GPIOMSP432_P3_0,    /*  18 - P3.0_IO */
    GPIOMSP432_P2_5,    /*  19 - P2.5_IO_PWM */
    NOT_MAPPABLE,       /*  20 - GND */

    NOT_MAPPABLE,       /*  21 - 5V */
    NOT_MAPPABLE,       /*  22 - GND */
    NOT_MAPPABLE,       /*  23 - P6.1_A14 */
    NOT_MAPPABLE,       /*  24 - P4.0_A13 */
    NOT_MAPPABLE,       /*  25 - P4.2_A11 */
    NOT_MAPPABLE,       /*  26 - P4.4_A9 */
    NOT_MAPPABLE,       /*  27 - P4.5_A8 */
    NOT_MAPPABLE,       /*  28 - P4.7_A6 */
    NOT_MAPPABLE,       /*  29 - P5.4_IO */
    NOT_MAPPABLE,       /*  30 - P5.5_IO */

    GPIOMSP432_P3_7,    /*  31 - P3.7_IO */
    GPIOMSP432_P3_5,    /*  32 - P3.5_IO */
    NOT_MAPPABLE,       /*  33 - P5.1_IO */
    GPIOMSP432_P2_3,    /*  34 - P2.3_IO */
    NOT_MAPPABLE,       /*  35 - P6.7_IO_CAPT */
    NOT_MAPPABLE,       /*  36 - P6.6_IO_CAPT */
    NOT_MAPPABLE,       /*  37 - P5.6_PWM */
    GPIOMSP432_P2_4,    /*  38 - P2.4_PWM */
    GPIOMSP432_P2_6,    /*  39 - P2.6_PWM */
    GPIOMSP432_P2_7,    /*  40 - P2.7_PWM */

    /* pins 41-56 */
    NOT_MAPPABLE,       /*  41 - P8.5 */
    NOT_MAPPABLE,       /*  42 - P9.0 */
    NOT_MAPPABLE,       /*  43 - P8.4 */
    NOT_MAPPABLE,       /*  44 - P8.2 */
    NOT_MAPPABLE,       /*  45 - P9.2 */
    NOT_MAPPABLE,       /*  46 - P6.2 */
    GPIOMSP432_P7_3,    /*  47 - P7.3 */
    GPIOMSP432_P7_1,    /*  48 - P7.1 */
    NOT_MAPPABLE,       /*  49 - P9.4 */
    NOT_MAPPABLE,       /*  40 - P9.6 */
    NOT_MAPPABLE,       /*  51 - P8.0 */
    GPIOMSP432_P7_4,    /*  52 - P7.4 */
    GPIOMSP432_P7_6,    /*  53 - P7.6 */
    NOT_MAPPABLE,       /*  54 - P10.0 */
    NOT_MAPPABLE,       /*  55 - P10_2 */
    NOT_MAPPABLE,       /*  56 - P10.4 */

    /* pins 57-72 */
    NOT_MAPPABLE,       /*  57 - P8.6 */
    NOT_MAPPABLE,       /*  58 - P8.7 */
    NOT_MAPPABLE,       /*  59 - P9.1 */
    NOT_MAPPABLE,       /*  60 - P8.3 */
    NOT_MAPPABLE,       /*  61 - P5.3 */
    NOT_MAPPABLE,       /*  62 - P9.3 */
    NOT_MAPPABLE,       /*  63 - P6.3 */
    GPIOMSP432_P7_2,    /*  64 - P7.2 */
    GPIOMSP432_P7_0,    /*  65 - P7.0 */
    NOT_MAPPABLE,       /*  66 - P9.5 */
    NOT_MAPPABLE,       /*  67 - P9.7 */
    GPIOMSP432_P7_5,    /*  68 - P7.5 */
    GPIOMSP432_P7_7,    /*  69 - P7.7 */
    NOT_MAPPABLE,       /*  70 - P10.1 */
    NOT_MAPPABLE,       /*  71 - P10.3 */
    NOT_MAPPABLE,       /*  72 - P10.5 */

    /* virtual pins 73-78 */
    NOT_MAPPABLE,       /*  73 - P1.1 SW1 */
    NOT_MAPPABLE,       /*  74 - P1.4 SW2 */
    GPIOMSP432_P2_0,    /*  75 - P2.0 RED_LED */
    GPIOMSP432_P2_1,    /*  76 - P2.1 GREEN_LED */
    GPIOMSP432_P2_2,    /*  77 - P2.2 BLUE_LED */
    NOT_MAPPABLE,       /*  78 - P1.0 LED1 */
};

/*
 * For the MSP432, the timers used for PWM are clocked at 12MHz.
 * The period is set to 2.04ms in the PWM_open() call.
 * The PWM objects are configured for PWM_DUTY_COUNTS mode to minimize
 * the PWM_setDuty() processing overhead.
 * The 2.04ms period yields a period count of 24480.
 * The Arduino analogWrite() API takes a value of 0-255 for the duty cycle.
 * The PWM scale factor is then 24480 / 255 = 96
 */

#define PWM_SCALE_FACTOR 24480/255

void analogWrite(uint8_t pin, int val)
{
    uint16_t pwmIndex, pinId, pinNum;
    uint_fast8_t port;
    uint_fast16_t pinMask;
    uint32_t hwiKey;

    hwiKey = Hwi_disable();

    if (digital_pin_to_pin_function[pin] == PIN_FUNC_ANALOG_OUTPUT) {
        pwmIndex = digital_pin_to_pwm_index[pin];
    }
    else {
        /* re-configure pin if possible */
        PWM_Params params;

        /*
         * The pwmIndex fetched from the pin_to_pwm_index[] table
         * is either an actual index into the PWM instance table
         * if the pin has already been mapped to a PWM resource,
         * or a mappable port/pin ID, or NOT_MAPPABLE.
         */
        pinId = digital_pin_to_pwm_index[pin];

        if (pinId == NOT_MAPPABLE) {
            Hwi_restore(hwiKey);
            return; /* can't get there from here */
        }

        /* find an unused PWM resource and port map it */
        for (pwmIndex = 0; pwmIndex < 8; pwmIndex++) {
            if (used_pwm_port_pins[pwmIndex] == NOT_IN_USE) {
                /* remember which pinId is being used by this PWM resource */
                used_pwm_port_pins[pwmIndex] = pinId; /* save port/pin info */
                /* remember which PWM resource is being used by this pin */
                digital_pin_to_pwm_index[pin] = pwmIndex; /* save pwm index */
                break;
            }
        }

        if (pwmIndex > 7) {
            Hwi_restore(hwiKey);
            return; /* no unused PWM ports */
        }

        PWM_Params_init(&params);

        /* Open the PWM port */
        params.period = 2040; /* arduino period is 2.04ms (490Hz) */
        params.dutyMode = PWM_DUTY_COUNTS;
        PWM_open(pwmIndex, &params);

        port = pinId >> 8;
        pinMask = pinId & 0xff;

        /* derive pinNum from pinMask */
        pinNum = 0;
        while (((1 << pinNum) & pinMask) == 0) pinNum++;

        /* Get write-access to port mapping registers: */
        PMAP->rKEYID = PMAP_KEYID_VAL;

        /* Enable reconfiguration during runtime */
        PMAP->rCTL.r = (PMAP->rCTL.r & ~PMAPRECFG) | PMAP_ENABLE_RECONFIGURATION;

        /* Configure Port Mapping for this pin: */
        HWREG8(PMAP_BASE + pinNum + pxmap[port]) = mappable_pwms[pwmIndex];

        /* Disable write-access to port mapping registers: */
        PMAP->rKEYID = 0;

        /* Enable PWM output on GPIO pins */
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(port, pinMask,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);

        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_OUTPUT;
    }

    Hwi_restore(hwiKey);

    PWM_setDuty((PWM_Handle)&(PWM_config[pwmIndex]), (val * PWM_SCALE_FACTOR));
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
    uint16_t pwmIndex = digital_pin_to_pwm_index[pin];
    uint_fast8_t port;
    uint_fast16_t pinMask;
    uint16_t pinNum;
    uint32_t hwiKey;

    /* stop the timer */
    analogWrite(pin, 0);

    /* undo dynamic port mapping plumbing */
    port = used_pwm_port_pins[pwmIndex] >> 8;
    pinMask = used_pwm_port_pins[pwmIndex] & 0xff;

    /* derive pinNum from pinMask */
    pinNum = 0;
    while (((1 << pinNum) & pinMask) == 0) pinNum++;
    /* the following code was extracted from PMAP_configurePort() */

    //Get write-access to port mapping registers:
    PMAP->rKEYID = PMAP_KEYID_VAL;

    //Enable reconfiguration during runtime
    PMAP->rCTL.r = (PMAP->rCTL.r & ~PMAPRECFG) | PMAP_ENABLE_RECONFIGURATION;

    //Undo Port Mapping for this pin:
    HWREG8(PMAP_BASE + pinNum + pxmap[port]) = PM_NONE;

    //Disable write-access to port mapping registers:
    PMAP->rKEYID = 0;

    /* Close PWM port */
    PWM_close((PWM_Handle)&(PWM_config[pwmIndex]));

    hwiKey = Hwi_disable();

    /* restore pin table entry with port/pin info */
    digital_pin_to_pwm_index[pin] = used_pwm_port_pins[pwmIndex];

    /* free up pwm resource */
    used_pwm_port_pins[pwmIndex] = NOT_IN_USE;

    Hwi_restore(hwiKey);
}

/*
 * analogRead() support
 */

#define NOT_ON_ADC      0xFF

static int8_t analogResolution = 10;
static bool adc_module_enabled = false;

static const uint16_t adc_to_port_pin[] = {
    GPIOMSP432_P5_5,  /* A0 */
    GPIOMSP432_P5_4,  /* A1 */
    GPIOMSP432_P5_3,  /* A2 */
    GPIOMSP432_P5_2,  /* A3 */
    GPIOMSP432_P5_1,  /* A4 */
    GPIOMSP432_P5_0,  /* A5 */
    GPIOMSP432_P4_7,  /* A6 */
    GPIOMSP432_P4_6,  /* A7 */

    GPIOMSP432_P4_5,  /* A8 */
    GPIOMSP432_P4_4,  /* A9 */
    GPIOMSP432_P4_3,  /* A10 */
    GPIOMSP432_P4_2,  /* A11 */
    GPIOMSP432_P4_1,  /* A12 */
    GPIOMSP432_P4_0,  /* A13 */
    GPIOMSP432_P6_1,  /* A14 */
    GPIOMSP432_P6_0,  /* A15 */

    GPIOMSP432_P9_1,  /* A16 */
    GPIOMSP432_P9_0,  /* A17 */
    GPIOMSP432_P8_7,  /* A18 */
    GPIOMSP432_P8_6,  /* A19 */
    GPIOMSP432_P8_5,  /* A20 */
    GPIOMSP432_P8_4,  /* A21 */
    GPIOMSP432_P8_3,  /* A22 */
    GPIOMSP432_P8_2,  /* A23 */
};

static const uint8_t digital_pin_to_adc_index[] = {
    /* port_pin */
    NOT_ON_ADC,     /*  dummy */

    /* pins 1-10 */
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

    /* pins 11-20 */
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

    /* pins 21-30 */
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

    /* pins 31-40 */
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

    /* pins 41-56 */
    ADC14INCH_20,   /*  41 - P8.5 */
    ADC14INCH_17,   /*  42 - P9.0 */
    ADC14INCH_21,   /*  43 - P8.4 */
    ADC14INCH_23,   /*  44 - P8.2 */
    NOT_ON_ADC,     /*  45 - P9.2 */
    NOT_ON_ADC,     /*  46 - P6.2 */
    NOT_ON_ADC,     /*  47 - P7.3 */
    NOT_ON_ADC,     /*  48 - P7.1 */
    NOT_ON_ADC,     /*  49 - P9.4 */
    NOT_ON_ADC,     /*  40 - P9.6 */
    NOT_ON_ADC,     /*  51 - P8.0 */
    NOT_ON_ADC,     /*  52 - P7.4 */
    NOT_ON_ADC,     /*  53 - P7.6 */
    NOT_ON_ADC,     /*  54 - P10.0 */
    NOT_ON_ADC,     /*  55 - P10_2 */
    NOT_ON_ADC,     /*  56 - P10.4 */

    /* pins 57-72 */
    ADC14INCH_19,   /*  57 - P8.6 */
    ADC14INCH_18,   /*  58 - P8.7 */
    ADC14INCH_16,   /*  59 - P9.1 */
    ADC14INCH_22,   /*  60 - P8.3 */
    NOT_ON_ADC,     /*  61 - P5.3 */
    NOT_ON_ADC,     /*  62 - P9.3 */
    NOT_ON_ADC,     /*  63 - P6.3 */
    NOT_ON_ADC,     /*  64 - P7.2 */
    NOT_ON_ADC,     /*  65 - P7.0 */
    NOT_ON_ADC,     /*  66 - P9.5 */
    NOT_ON_ADC,     /*  67 - P9.7 */
    NOT_ON_ADC,     /*  68 - P7.5 */
    NOT_ON_ADC,     /*  69 - P7.7 */
    NOT_ON_ADC,     /*  70 - P10.1 */
    NOT_ON_ADC,     /*  71 - P10.3 */
    NOT_ON_ADC,     /*  72 - P10.5 */

    /* virtual pins 73-78 */
    NOT_ON_ADC,     /*  73 - P1.1 SW1 */
    NOT_ON_ADC,     /*  73 - P1.4 SW2 */
    NOT_ON_ADC,     /*  75 - P2.0 RED_LED */
    NOT_ON_ADC,     /*  76 - P2.1 GREEN_LED */
    NOT_ON_ADC,     /*  77 - P2.2 BLUE_LED */
    NOT_ON_ADC,     /*  78 - P1.0 LED1 */
};

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
    uint32_t hwiKey;

    if (adcIndex == NOT_ON_ADC) return (0);

    adcMem = 1 << adcIndex;
    adcInt = 1 << adcIndex;

    hwiKey = Hwi_disable();

    /* re-configure pin if necessary */
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_ANALOG_INPUT) {
        uint_fast8_t port;
        uint_fast16_t pinMask;

        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_INPUT;

        /* initialize top level ADC module if it hasn't been already */
        if (adc_module_enabled == false) {
            adc_module_enabled = true;

            /* Initializing ADC (MCLK/1/1) */
            MAP_ADC14_enableModule();
            MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK,
                             ADC_PREDIVIDER_1,
                             ADC_DIVIDER_1,
                             0);
            /* Setting reference voltage to 2.5 */
            MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
            MAP_REF_A_enableReferenceVoltage();

            analogReadResolution(analogResolution);
        }

        port = adc_to_port_pin[adcIndex] >> 8;
        pinMask = adc_to_port_pin[adcIndex] & 0xff;

        /* Setting up GPIO pins as analog inputs (and references) */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port, pinMask,
                                        GPIO_TERTIARY_MODULE_FUNCTION);
    }

    /* minimize latency by re-enabling ints temporarily */
    Hwi_restore(hwiKey);

    /* Sadly, I think the following code must all be done thread safely */
    hwiKey = Hwi_disable();

    /* stop all current conversions */
    MAP_ADC14_disableConversion();

    /* Configuring ADC Memory in single conversion mode
     * with use of internal VSS as references */
    MAP_ADC14_configureSingleSampleMode(adcMem, false);
    MAP_ADC14_configureConversionMemory(adcMem,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    adcIndex, false);

    /* Enabling sample timer in auto iteration mode and interrupts */
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* clear out any stale conversions */
    status = MAP_ADC14_getInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    /* start new conversion */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    status = MAP_ADC14_getInterruptStatus();

    while ((adcInt & status) == 0) {
        status = MAP_ADC14_getInterruptStatus();
    }

    /* clear out last sample */
    MAP_ADC14_clearInterruptFlag(adcInt);
    sample = MAP_ADC14_getResult(adcMem);

    Hwi_restore(hwiKey);

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
    uint8_t adcIndex = digital_pin_to_adc_index[pin];
    uint_fast8_t port;
    uint_fast16_t pinMask;

    port = adc_to_port_pin[adcIndex] >> 8;
    pinMask = adc_to_port_pin[adcIndex] & 0xff;

    /* Place Pin in NORMAL GPIO mode */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(port, pinMask,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);
}

/*
 * \brief sets the number of bits to shift the value read by ADCFIFORead()
 */
void analogReadResolution(uint16_t bits)
{
    uint32_t adcBits;

    switch (bits) {
        case 8:
            adcBits = ADC_8BIT;
            break;
        case 10:
            adcBits = ADC_10BIT;
            break;
        case 12:
            adcBits = ADC_12BIT;
            break;
        case 14:
            adcBits = ADC_14BIT;
            break;
    }
    analogResolution = bits;

    MAP_ADC14_setResolution(adcBits);
}
