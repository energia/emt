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

#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <driverlib/ioc.h>
#include <driverlib/aux_adc.h>
#include <driverlib/aux_wuc.h> 

#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>
#include <ti/drivers/pwm/PWMTimerCC26xx.h>
#include <ti/drivers/pin/PINCC26XX.h>


/*
 * analogWrite() support
 */

extern PWM_Config PWM_config[];
extern const GPIOCC26XX_Config GPIOCC26XX_config;

#define NOT_IN_USE 0

/* Mappable PWM Timer capture pins */
const uint8_t mappable_pwms[] = {
   IOC_IOCFG17_PORT_ID_PORT_EVENT0,
   IOC_IOCFG17_PORT_ID_PORT_EVENT1,
   IOC_IOCFG17_PORT_ID_PORT_EVENT2,
   IOC_IOCFG17_PORT_ID_PORT_EVENT3,
   IOC_IOCFG17_PORT_ID_PORT_EVENT4,
   IOC_IOCFG17_PORT_ID_PORT_EVENT5,
   IOC_IOCFG17_PORT_ID_PORT_EVENT6,
   IOC_IOCFG17_PORT_ID_PORT_EVENT7,
};

/* Current PWM timer GPIO mappings */
uint8_t used_pwm_port_pins[] = {
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
    NOT_IN_USE,
};

/*
 * While in use, this table contains
 * the index of the PWM Timer resource that is mapped
 * to the corresponding pin.
 */
uint16_t digital_pin_to_pwm_index[32];

/*
 * Common digital pin decommissioning function
 */
void stopDigitalIO(uint8_t pin)
{
    GPIOCC26xx_release(pin);
}

/*
 * Do whatever is necessary to prepare the digital input pin to be
 * configured in any other mode
 */
void stopDigitalRead(uint8_t pin)
{
   stopDigitalIO(pin);
}

/*
 * Do whatever is necessary to prepare the digital output pin to be
 * configured in any other mode
 */
 void stopDigitalWrite(uint8_t pin)
{
   stopDigitalIO(pin);
}

/*
 * For the CC26xx, the timers used for PWM are clocked at 48MHz.
 * A PWM period of 2.04ms is chosen for Arduino compatibility.
 * The period is set to 2.040ms in the PWM_open() calls below.
 * The PWM objects are configured for PWM_DUTY_COUNTS mode to minimize
 * the PWM_setDuty() processing overhead.
 * The 2.040ms period yields a period count of 97,920.
 * The Arduino analogWrite() API takes a value of 0-255 for the duty cycle.
 * The PWM scale factor is then 97,920 / 255 = 384
 */

#define PWM_SCALE_FACTOR 384

void analogWrite(uint8_t pin, int val) 
{
    uint8_t pwmIndex;
    uint8_t pwmPinId;
    uint32_t hwiKey;

    hwiKey = Hwi_disable();
    
    if (digital_pin_to_pin_function[pin] == PIN_FUNC_ANALOG_OUTPUT) {
        pwmIndex = digital_pin_to_pwm_index[pin];
    }
    else {
        /* re-configure pin if possible */
        PWM_Params params;
        PWMTimerCC26xx_PWMPinCfg pwmPinCfg;
        
        if (digital_pin_to_pin_function[pin] == PIN_FUNC_INVALID) {
            Hwi_restore(hwiKey);
            return; /* can't get there from here */
        }
        
        /* extract 16bit pinID from pin */
        pwmPinId = GPIOCC26XX_config.pinConfigs[pin] & 0xff;
        
        /* find an unused PWM resource and port map it */
        for (pwmIndex = 0; pwmIndex < 8; pwmIndex++) {
            if (used_pwm_port_pins[pwmIndex] == NOT_IN_USE) {
                /* remember which pinId is being used by this PWM resource */
                used_pwm_port_pins[pwmIndex] = pwmPinId; /* save pwm pin info */
                /* remember which PWM resource is being used by this pin */
                digital_pin_to_pwm_index[pin] = pwmIndex; /* save pwm index */
                break;
            }
        }

        if (pwmIndex > 7) {
            Hwi_restore(hwiKey);
            return; /* no unused PWM ports */
        }

        /* undo pin's current plumbing */
        switch (digital_pin_to_pin_function[pin]) {
            case PIN_FUNC_ANALOG_INPUT:
                stopAnalogRead(pin);
                break;
            case PIN_FUNC_DIGITAL_INPUT:
                stopDigitalRead(pin);
                break;
            case PIN_FUNC_DIGITAL_OUTPUT:
                stopDigitalWrite(pin);
                break;
        }
        
        /* Open the PWM port */
        PWM_Params_init(&params);

        params.period = 2040; /* arduino period is 2.04ms (490Hz) */
        params.dutyMode = PWM_DUTY_COUNTS;
        pwmPinCfg.pwmPinId = pwmPinId; /* override HWAttrs pwmPinId */
        params.custom = &pwmPinCfg;
        
        PWM_open(pwmIndex, &params);
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
    /* Close PWM port */
    PWM_close((PWM_Handle)&(PWM_config[pwmIndex]));
    /* restore pin table entry with port/pin info */
    digital_pin_to_pwm_index[pin] = used_pwm_port_pins[pwmIndex];
    /* free up pwm resource */
    used_pwm_port_pins[pwmIndex] = NOT_IN_USE;
}

/*
 * analogRead() support
 */

static int8_t analogReadShift = 2;
static PIN_Handle adcPinHandle;
static PIN_State adcPinState;
static PIN_Config adcPinTable[2];
static uint8_t adcPin;
static uint8_t adcAuxIo;
static uint32_t adcTrimGain, adcTrimOffset;

/*
 * \brief           configure the A/D reference voltage
 * \param mode      DEFAULT, INTERNAL, EXTERNAL, ...
 * \return          void
 */
void analogReference(uint16_t mode)
{
}

/*
 * \brief           Reads an analog value from the pin specified.
 * \param[in] pin   The pin number to read from.
 * \return          A 16-bit integer containing a 12-bit sample from the ADC.
 */
uint16_t analogRead(uint8_t pin)
{
    uint32_t hwiKey;
    uint32_t adcSample;
    
    hwiKey = Hwi_disable();
    
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_ANALOG_INPUT) {
        /* adcPinId must be 16 bits to compare with EMPTY_PIN */
        uint8_t adcPinId;
        adcPinId = GPIOCC26XX_config.pinConfigs[pin] & 0xff;

        /* for 7x7 packages, only IOIDs 23-30 are tied to AUXIO channels */
        if ((adcPinId < 23) || 
            (adcPinId > 30) || 
            (digital_pin_to_pin_function[pin] == PIN_FUNC_INVALID)) {
            Hwi_restore(hwiKey);
            return (0); /* can't get there from here */
        }

        if (adcPinHandle) {
            /* tear down last analogWrite() pin */
            PIN_close(adcPinHandle);
            digital_pin_to_pin_function[adcPin] = PIN_FUNC_UNUSED;
        }
        
        /* undo pin's current plumbing */
        switch (digital_pin_to_pin_function[pin]) {
            case PIN_FUNC_ANALOG_OUTPUT:
                stopAnalogWrite(pin);
                break;
            case PIN_FUNC_DIGITAL_INPUT:
                stopDigitalRead(pin);
                break;
            case PIN_FUNC_DIGITAL_OUTPUT:
                stopDigitalWrite(pin);
                break;
        }

        /* open pin */
        adcPinTable[0] = adcPinId | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS;
        adcPinTable[1] = PIN_TERMINATE;
        adcPinHandle = PIN_open(&adcPinState, adcPinTable);
        
        /* get trim gain and offset */
        adcTrimGain = AUXADCGetAdjustmentGain(AUXADC_REF_VDDS_REL);
        adcTrimOffset = AUXADCGetAdjustmentOffset(AUXADC_REF_VDDS_REL);
        
        if (!adcPinHandle) {
            Hwi_restore(hwiKey);
            return (0); /* PIN is owned by someone else */
        }
        
        adcAuxIo = ADC_COMPB_IN_AUXIO7 + (adcPinId - 23);
        adcPin = pin;      
        digital_pin_to_pin_function[pin] = PIN_FUNC_ANALOG_INPUT;
    }

    // Enable clock for ADC digital and analog interface
    AUXWUCClockEnable(AUX_WUC_ADC_CLOCK | AUX_WUC_ANAIF_CLOCK | AUX_WUC_ADI_CLOCK);
    // Connect corresponding AUXIOx channel as analog input.
    AUXADCSelectInput(adcAuxIo);
    // Set up ADC (use 3.3V VDD as reference)
    AUXADCEnableAsync(AUXADC_REF_VDDS_REL, AUXADC_TRIGGER_MANUAL);
    // Disallow STANDBY mode while using the ADC.
    Power_setConstraint(Power_SB_DISALLOW);
    // Trigger ADC sampling
    AUXADCGenManualTrigger();
    /* fetch the next sample */
    adcSample = AUXADCAdjustValueForGainAndOffset(AUXADCReadFifo(), 
                        adcTrimGain, adcTrimOffset);
    // Disable ADC
    AUXADCDisable();
    // Allow STANDBY mode again
    Power_releaseConstraint(Power_SB_DISALLOW);
    /* finally allow another thread to do an analogRead */
    Hwi_restore(hwiKey);

    return (adcSample >> analogReadShift);
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
    if (pin == adcPin) {
        if (adcPinHandle) {
            /* tear down last analogWrite() pin */
            PIN_close(adcPinHandle);
            digital_pin_to_pin_function[adcPin] = PIN_FUNC_UNUSED;
            adcPinHandle = NULL;
        }
    }
}

/*
 * \brief sets the number of bits to shift the value read by ADCFIFORead()
 */
void analogReadResolution(uint16_t bits)
{
    analogReadShift = 12 - bits;
}
