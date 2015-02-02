/*
 ************************************************************************
 *  WInterrupts.c
 *
 *  Arduino core files for TI-RTOS
 *      Copyright (c) 2013 Louis Peryea. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:

  WInterrupts.c Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

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

  Modified 24 November 2006 by David A. Mellis
  Modified 1 August 2010 by Mark Sproul
  Modified for MSP430 by Robert Wessels
 */

#include "Energia.h"
#include "ti/drivers/GPIO.h"

extern GPIO_Config GPIO_config[];
extern GPIO_HWAttrs gpioHWAttrs[];

extern GPIO_Callbacks Board_gpioPort1Callbacks;
extern GPIO_Callbacks Board_gpioPort2Callbacks;
extern GPIO_Callbacks Board_gpioPort3Callbacks;
extern GPIO_Callbacks Board_gpioPort4Callbacks;
extern GPIO_Callbacks Board_gpioPort5Callbacks;
extern GPIO_Callbacks Board_gpioPort6Callbacks;

static GPIO_Callbacks *gpioCallbacks[6] =
{
    &Board_gpioPort1Callbacks,
    &Board_gpioPort2Callbacks,
    &Board_gpioPort3Callbacks,
    &Board_gpioPort4Callbacks,
    &Board_gpioPort5Callbacks,
    &Board_gpioPort6Callbacks
};

void Wiring_GPIO_hwiIntFxn(UArg callbacks)
{
    uint32_t        pins;
    unsigned int    i;
    GPIO_Callbacks *portCallback;

    portCallback = (GPIO_Callbacks*)callbacks;

    /* Find out which pins have their interrupt flags set */
    pins = MAP_GPIO_getEnabledInterruptStatus(portCallback->port);

    /* Match the interrupt to its corresponding callback function */
    for (i = 0; pins; i++) {
        if (pins & 0x1) {
        	MAP_GPIO_clearInterruptFlag(portCallback->port, (1 << i));
            if (portCallback->callbackFxn[i] != NULL) {
                portCallback->callbackFxn[i]();
            }
        }
        pins = pins >> 1;
    }
}

void interrupts(void)
{
    Hwi_enable();
}

void noInterrupts(void)
{
    Hwi_disable();
}

void attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode)
{
    volatile uint32_t portNum;
    GPIO_IntType intType;
    uint8_t bitNum, i;

    uint8_t hwAttrIndex = digital_pin_to_hwAttrs_index[pin];

    if (hwAttrIndex == NOT_A_PORT) return;

    portNum = (uint32_t) gpioHWAttrs[hwAttrIndex].port;

    i = digitalPinToBitMask(pin);

    bitNum = 0;

    /* derive bitNum from bit mask */
    while (i >>= 1) {
        bitNum++;
    }

    /* 
     * find the callback structure with matching port and 
     * place userFunc in the bitNum respective callbackFxn
     */
    for (i = 0; i < 6; i++) {
        if (portNum == gpioCallbacks[i]->port) {
            gpioCallbacks[i]->callbackFxn[bitNum] = userFunc;
        }
    }

    switch(mode) {
    case LOW:
        intType = GPIO_INT_LOW;
        break;
    case CHANGE:
        intType = GPIO_INT_BOTH_EDGES;
        break;
    case RISING:
        intType = GPIO_INT_RISING;
        break;
    case FALLING:
        intType = GPIO_INT_FALLING;
        break;
    case HIGH:
        intType = GPIO_INT_HIGH;
        break;
    }

    GPIO_enableInt(hwAttrIndex, intType);
}

void detachInterrupt(uint8_t pin) {
    uint8_t bitNum, i;
    volatile uint32_t portNum;
    uint8_t hwAttrIndex = digital_pin_to_hwAttrs_index[pin];

    if (hwAttrIndex == NOT_A_PORT) return;

    GPIO_disableInt(hwAttrIndex);
    GPIO_clearInt(hwAttrIndex);

    i = digitalPinToBitMask(pin);

    portNum = (uint32_t) gpioHWAttrs[hwAttrIndex].port;

    while (i >>= 1) {
        bitNum++;
    }

    for (i = 0; i < 6; i++) {
        if (portNum == gpioCallbacks[i]->port) {
            gpioCallbacks[i]->callbackFxn[bitNum] = 0;
        }
    }
}
