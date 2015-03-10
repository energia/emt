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
#include <ti/drivers/GPIO.h>

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
    GPIO_PinConfig intType;

    switch(mode) {
        case LOW:
            intType = GPIO_CFG_IN_INT_LOW;
            break;
        case CHANGE:
            intType = GPIO_CFG_IN_INT_BOTH_EDGES;
            break;
        case RISING:
            intType = GPIO_CFG_IN_INT_RISING;
            break;
        case FALLING:
            intType = GPIO_CFG_IN_INT_FALLING;
            break;
        case HIGH:
            intType = GPIO_CFG_IN_INT_HIGH;
            break;
    }

    GPIO_setConfig(pin, GPIO_CFG_IN_INT_ONLY | intType);

    GPIO_setCallback(pin, (GPIO_CallbackFxn)userFunc);

    GPIO_enableInt(pin);
}

void detachInterrupt(uint8_t pin) {
    GPIO_setCallback(pin, NULL);
}
