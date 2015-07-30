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

#include <stdbool.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Types.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/lm4/Timer.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#include <ti/drivers/pwm/PWMTimerCC26xx.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include <inc/hw_types.h>
#include <inc/hw_gpt.h>
#include <driverlib/ioc.h>
#include <driverlib/timer.h>

#define NUM_TIMERS  8

void         PWMTimerCC26xx_close(PWM_Handle handle);
int          PWMTimerCC26xx_control(PWM_Handle handle, unsigned int cmd,
                                  void *arg);
unsigned int PWMTimerCC26xx_getPeriodCounts(PWM_Handle handle);
unsigned int PWMTimerCC26xx_getPeriodMicroSecs(PWM_Handle handle);
void         PWMTimerCC26xx_init(PWM_Handle handle);
PWM_Handle   PWMTimerCC26xx_open(PWM_Handle handle, PWM_Params *params);
void         PWMTimerCC26xx_setDuty(PWM_Handle handle, uint32_t duty);

/* PWM function table for PWMTimerCC26xx implementation */
const PWM_FxnTable PWMTimerCC26xx_fxnTable = {
    PWMTimerCC26xx_close,
    PWMTimerCC26xx_control,
    PWMTimerCC26xx_getPeriodCounts,
    PWMTimerCC26xx_getPeriodMicroSecs,
    PWMTimerCC26xx_init,
    PWMTimerCC26xx_open,
    PWMTimerCC26xx_setDuty
};

/*
 * The following table is used to dynamically map a device pin
 * to a GPT's PWM output pin.
 *
 * Although quite difficult to sort out, it appears that
 * the port IDs of the GPT pins are 23-30, corresponding
 * to GPT0-A, GPT0-B, GPT1-A, GPT1-B, etc.
 *
 * Table 11-3 of the CC2650 TRM is the source of this mapping:
 *  23 MCU_GPTM_GPTM0 GMTM timer pin GPTM0
 *  24 MCU_GPTM_GPTM1 GMTM timer pin GPTM2
 *  25 MCU_GPTM_GPTM2 GMTM timer pin GPTM3
 *  26 MCU_GPTM_GPTM3 GMTM timer pin GPTM4
 *  27 MCU_GPTM_GPTM4 GMTM timer pin GPTM4
 *  28 MCU_GPTM_GPTM5 GMTM timer pin GPTM5
 *  29 MCU_GPTM_GPTM6 GMTM timer pin GPTM6
 *  30 MCU_GPTM_GPTM7 GMTM timer pin GPTM7
 *
 * These portIDs seem to map to the definitions of 
 * IOC_IOCFG17_PORT_ID_PORT_EVENT0-7 respectively as found in
 * the inc/hw_ioc.h header file.
 *
 */

static const uint8_t gptPinPortIds[] = {
   IOC_IOCFG17_PORT_ID_PORT_EVENT0,
   IOC_IOCFG17_PORT_ID_PORT_EVENT1,
   IOC_IOCFG17_PORT_ID_PORT_EVENT2,
   IOC_IOCFG17_PORT_ID_PORT_EVENT3,
   IOC_IOCFG17_PORT_ID_PORT_EVENT4,
   IOC_IOCFG17_PORT_ID_PORT_EVENT5,
   IOC_IOCFG17_PORT_ID_PORT_EVENT6,
   IOC_IOCFG17_PORT_ID_PORT_EVENT7,
};

/*
 *  ======================== PIN driver objects ================================
 */
/* PIN driver state objects, 1 for each possible PWM pin */
static PIN_State pinState[NUM_TIMERS];
/* PIN driver handle */
static PIN_Handle hPin[NUM_TIMERS];

extern const PWM_Params PWM_defaultParams;

/*
 * GPT peripheral match register prescalars are 8 bits wide.  Max value which
 * can be represented is 255.
 */
const uint8_t PWMTimerCC26xx_MAX_PRESCALAR_VALUE = (~0);

/*
 * GPT peripheral load & match registers are 16 bits wide.  Max value which
 * can be represented is 65535.
 */
const uint16_t PWMTimerCC26xx_MAX_MATCH_VALUE = (~0);

/*
 *  ======== PWMTimerCC26xx_close ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTimerCC26xx_close(PWM_Handle handle)
{
    unsigned int                key;
    uint16_t                    kernelTimerID;
    uint16_t                    timerPairEnabledBit;
    PWMTimerCC26xx_Object        *object = handle->object;
    PWMTimerCC26xx_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Set duty to 0 before closing the PWM instance */
    PWMTimerCC26xx_setDuty(handle, 0);
    TimerDisable(hwAttrs->baseAddr, hwAttrs->timer);

    key = Hwi_disable();
    /*
     * Before closing the PWM instance, we must verify whether the other PWM
     * timer half is operating.  The Timer will be marked unused only if both
     * PWM half timers are disabled.
     */
    kernelTimerID = 1 << ((hwAttrs->baseAddr >> 12) & 0xF);
    timerPairEnabledBit = (TIMER_BOTH & (~hwAttrs->timer)) &
                          (GPT_CTL_TAEN | GPT_CTL_TBEN);

    if (!(HWREG(hwAttrs->baseAddr + GPT_O_CTL) & timerPairEnabledBit)) {
        /* Both timer is completely disabled */
        Timer_setAvailMask(Timer_getAvailMask() | kernelTimerID);
    }

    object->period = 0;
    
    /* release pin */
    PIN_close(hPin[object->timerNum]);
    hPin[object->timerNum] = NULL;
    
    Hwi_restore(key);

    Log_print1(Diags_USER1, "PWM: (%p) is closed", (UArg) handle);
}

/*
 *  ======== PWMTimerCC26xx_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int PWMTimerCC26xx_control(PWM_Handle handle, unsigned int cmd, void *arg)
{
    unsigned int                key;
    uint32_t                    newPeriod;
    uint32_t                    maxPeriod;
    PWMTimerCC26xx_Object        *object = handle->object;
    PWMTimerCC26xx_HWAttrs const *hwAttrs = handle->hwAttrs;

    switch (cmd) {
        case PWMTimerCC26xx_CHANGE_PERIOD:
            newPeriod = *((uint32_t *) arg);

            /* Ensure the period is less than the max period supported */
            maxPeriod = (((PWMTimerCC26xx_MAX_PRESCALAR_VALUE + 1) *
                       PWMTimerCC26xx_MAX_MATCH_VALUE) / object->cyclesPerMicroSec) - 1;
            if (!newPeriod || newPeriod > maxPeriod) {
                return (-1);
            }

            object->period  = newPeriod;
            key = Hwi_disable();
            TimerPrescaleSet(hwAttrs->baseAddr, hwAttrs->timer,
                             (object->period * object->cyclesPerMicroSec) /
                             PWMTimerCC26xx_MAX_MATCH_VALUE);
            TimerLoadSet(hwAttrs->baseAddr, hwAttrs->timer,
                         (object->period * object->cyclesPerMicroSec) %
                         PWMTimerCC26xx_MAX_MATCH_VALUE);

            Hwi_restore(key);
            return PWMTimerCC26xx_CHANGE_PERIOD;
    }

    /* No implementation yet */
    return (PWM_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== PWMTimerCC26xx_getPeriodCounts ========
 *  @pre    Function assumes that the handle is not NULL
 */
unsigned int PWMTimerCC26xx_getPeriodCounts(PWM_Handle handle)
{
    PWMTimerCC26xx_Object *object = handle->object;

    return (object->period * object->cyclesPerMicroSec);
}

/*
 *  ======== PWMTimerCC26xx_getPeriodMicroSecs ========
 *  @pre    Function assumes that the handle is not NULL
 */
unsigned int PWMTimerCC26xx_getPeriodMicroSecs(PWM_Handle handle)
{
    return (((PWMTimerCC26xx_Object *) handle->object)->period);
}

/*
 *  ======== PWMTimerCC26xx_init ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTimerCC26xx_init(PWM_Handle handle)
{
    /* Mark the object as available */
    ((PWMTimerCC26xx_Object *) handle->object)->period = 0;
}

/*
 *  ======== PWMTimerCC26xx_open ========
 *  @pre    Function assumes that the handle is not NULL
 */
PWM_Handle PWMTimerCC26xx_open(PWM_Handle handle, PWM_Params *params)
{
    unsigned int                key;
    uint16_t                    kernelTimerID;
    uint16_t                    timerPairEnabledBit;
    uint32_t                    timerPeriod;
    uint32_t                    timerConfigVal;
    Types_FreqHz                freq;
    PWMTimerCC26xx_Object        *object = handle->object;
    PWMTimerCC26xx_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint8_t                     timerNum;
    uint16_t                    pwmPinId;
    PIN_Config                  pwmPinTable[2];

    key = Hwi_disable();

    /* Ensure peripheral is not already open */
    if (object->period) {
        Hwi_restore(key);
        return (NULL);
    }

    /*
     * Before opening the PWM instance, we must verify that the Timer is not
     * being used by the Kernel.  Additionally, the Timer peripheral could have
     * already been initialized by the other PWM timer half.  So we must also
     * verify if the other half is initialized.
     */
    kernelTimerID = 1 << ((hwAttrs->baseAddr >> 12) & 0xF);
    timerPairEnabledBit = (TIMER_BOTH & (~hwAttrs->timer)) &
                          (GPT_CTL_TAEN | GPT_CTL_TBEN);

    if (kernelTimerID & Timer_getAvailMask()) {
        /* Timer unused, claim timer from Kernel */
        kernelTimerID = (~kernelTimerID) & 0x0F;
        Timer_setAvailMask(Timer_getAvailMask() & kernelTimerID);
    }
    else if (!(HWREG(hwAttrs->baseAddr + GPT_O_CTL) & timerPairEnabledBit)) {
        /* Timer used and was not initialized by the other half timer */
        Hwi_restore(key);
        return (NULL);
    }

    object->period = (~0);
    Hwi_restore(key);

    if(params == NULL) {
        params = (PWM_Params *) &PWM_defaultParams;
    }

    BIOS_getCpuFreq(&freq);
    object->cyclesPerMicroSec = freq.lo / 1000000;

    /* Calculate the maximum period supported by the peripheral */
    timerPeriod = (((PWMTimerCC26xx_MAX_PRESCALAR_VALUE + 1) *
                    PWMTimerCC26xx_MAX_MATCH_VALUE) / object->cyclesPerMicroSec) - 1;

    /* Ensure the period is less than the max period supported */
    Assert_isTrue(params->period && params->period <= timerPeriod, NULL);

    /*
     * Set PWM duty to initial value (not 0) - required when inverting
     * output polarity to generate a duty equal to 0 or period.  See comments in
     * PWMTimerCC3200_setDuty for more information.
     */
    object->duty = 1;
    object->dutyMode = params->dutyMode;
    object->period = params->period;
    timerNum = (hwAttrs->baseAddr >> 11) & 0x1E; /* 0, 2, 4, 6 */
    timerNum += hwAttrs->timer == TIMER_A ? 0 : 1; /* add in timer LSB */
    object->timerNum = timerNum;

    /* configure pwm pin */
    if (params->custom) { /* use the custom params' pwmPinId if supplied */
        pwmPinId = ((PWMTimerCC26xx_PWMPinCfg *)(params->custom))->pwmPinId;
    }
    else {
        pwmPinId = hwAttrs->pwmPinId;
    }
    pwmPinTable[0] = pwmPinId | PIN_INPUT_DIS | PIN_PUSHPULL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH;
    pwmPinTable[1] = PIN_TERMINATE;
    
    hPin[timerNum] = PIN_open(&pinState[timerNum], pwmPinTable);
    
    /* Is pin already allocated? */
    if (!hPin[timerNum]) {
        return false;
    }

    /* Register power dependency - i.e. power up and enable clock for GPTx. */
    Power_setDependency(hwAttrs->powerMngrId);

    /* Set IO muxing for the GPT pwm pin */
    PINCC26XX_setMux(hPin[timerNum], pwmPinId, gptPinPortIds[timerNum]);

    /*
     * The TimerConfigure API halts both timers when it is used to configure
     * a single half timer.  The code below performs the register
     * operations necessary to configure each half timer individually.
     */
    TimerDisable(hwAttrs->baseAddr, hwAttrs->timer);

    /* Split the timer and configure it as a PWM */
    timerConfigVal = ((hwAttrs->timer & (TIMER_CFG_A_PWM | TIMER_CFG_B_PWM)) |
                      TIMER_CFG_SPLIT_PAIR);
    HWREG(hwAttrs->baseAddr + GPT_O_CFG) |= (timerConfigVal >> 24);
    if(hwAttrs->timer & TIMER_A){
        HWREG(hwAttrs->baseAddr + GPT_O_TAMR) = timerConfigVal & 255;
    }
    else  {
        HWREG(hwAttrs->baseAddr + GPT_O_TBMR) = (timerConfigVal >> 8) & 255;
    }
    TimerLevelControl(hwAttrs->baseAddr, hwAttrs->timer, params->polarity);

    timerPeriod = object->period * object->cyclesPerMicroSec;
    TimerPrescaleSet(hwAttrs->baseAddr, hwAttrs->timer,
                     timerPeriod / PWMTimerCC26xx_MAX_MATCH_VALUE);
    TimerLoadSet(hwAttrs->baseAddr, hwAttrs->timer,
                 timerPeriod % PWMTimerCC26xx_MAX_MATCH_VALUE);

    PWMTimerCC26xx_setDuty(handle, 0);
    TimerEnable(hwAttrs->baseAddr, hwAttrs->timer);

    Log_print2(Diags_USER1, "PWM:(%p) opened; period set to: %d", (UArg) handle,
               object->period);

    return (handle);
}

/*
 *  ======== PWMTimerCC26xx_setDuty ========
 *  @pre    Function assumes that handle is not NULL
 */
void PWMTimerCC26xx_setDuty(PWM_Handle handle, uint32_t duty)
{
    unsigned int                key;
    uint32_t                    timerPeriod;
    uint32_t                    dutyInCycles;
    PWMTimerCC26xx_Object        *object = handle->object;
    PWMTimerCC26xx_HWAttrs const *hwAttrs = handle->hwAttrs;

    key = Hwi_disable();

    timerPeriod = object->period * object->cyclesPerMicroSec;
    switch (object->dutyMode) {
        case PWM_DUTY_COUNTS:
            /* Duty specified as timer counts */
            Assert_isTrue(duty <= timerPeriod, NULL);

            dutyInCycles = timerPeriod;
            if (duty < dutyInCycles) {
                dutyInCycles -= duty;
            }
            break;

        case PWM_DUTY_TIME:
            /* Duty is specified in microseconds */
            Assert_isTrue(duty <= object->period, NULL);

            dutyInCycles = object->period;
            if (duty < object->period) {
                dutyInCycles -= duty;
            }
            dutyInCycles *= object->cyclesPerMicroSec;
            break;

        case PWM_DUTY_SCALAR:
            /* Duty specified as a number [0 - 65535] scaled to the period */
            Assert_isTrue(duty <= PWMTimerCC26xx_MAX_MATCH_VALUE, NULL);

            /* If duty is 0; Max value is used but the polarity inverted */
            dutyInCycles = timerPeriod;
            if (duty && duty < PWMTimerCC26xx_MAX_MATCH_VALUE) {
                dutyInCycles = (dutyInCycles * 100) / PWMTimerCC26xx_MAX_MATCH_VALUE;
                dutyInCycles = (dutyInCycles *
                                (PWMTimerCC26xx_MAX_MATCH_VALUE - duty)) / 100;
                if (!dutyInCycles) {
                    ++dutyInCycles;
                }
            }
            break;

        default:
            Hwi_restore(key);

            Log_print1(Diags_USER1,
                       "PWM: (%p) unsupported PWM duty mode; duty unchanged",
                       (UArg) handle);
            return;
    }

    /*
     * The timer peripheral cannot generate a duty of 0 when the timer is
     * counting down.  In cases where a 0 duty is desired, the PWM duty is set
     * to the period value (output remains active) and output polarity is
     * inverted.  Additionally, if the output is changed from 0 (to a non-zero
     * value) the PWM output polarity must be inverted again.
     *
     * The code below uses the previous duty (object->duty) and the new duty to
     * determine if the polarity should be inverted.
     * For more details refer to the device specific datasheet and the following
     * E2E post:
     *  http://e2e.ti.com/support/microcontrollers/tiva_arm/f/908/t/354826.aspx
     */
    if (((!duty) && object->duty) || (duty && (!object->duty))) {
        HWREG(hwAttrs->baseAddr + GPT_O_CTL) ^=
                       (hwAttrs->timer & (GPT_CTL_TAPWML | GPT_CTL_TBPWML));
    }

    object->duty = duty;
    TimerPrescaleMatchSet(hwAttrs->baseAddr, hwAttrs->timer,
                          dutyInCycles / PWMTimerCC26xx_MAX_MATCH_VALUE);
    TimerMatchSet(hwAttrs->baseAddr, hwAttrs->timer,
                  dutyInCycles % PWMTimerCC26xx_MAX_MATCH_VALUE);

    Hwi_restore(key);
    Log_print2(Diags_USER2, "PWM: (%p) duty set to: %d", (UArg) handle, duty);
}
