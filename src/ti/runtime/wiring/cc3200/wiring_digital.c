/*
 ************************************************************************
 *  wiring_digital.cpp
 *
 *  Energia core files
 *      Copyright (c) 2014 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring_digital.c - digital input and output functions
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

#include "wiring_private.h"
#include <ti/drivers/GPIO.h>

/* device specific routine */
GPIO_PinConfig mode2gpioConfig(uint8_t pin, uint8_t mode)
{
   switch (mode) {
        case INPUT:
        case INPUT_PULLUP:
            digital_pin_to_pin_function[pin] = PIN_FUNC_DIGITAL_INPUT;
            return (GPIO_CFG_IN_PU);

        case INPUT_PULLDOWN:
            digital_pin_to_pin_function[pin] = PIN_FUNC_DIGITAL_INPUT;
            return (GPIO_CFG_IN_PD);

        case OUTPUT:
            digital_pin_to_pin_function[pin] = PIN_FUNC_DIGITAL_OUTPUT;
            return (GPIO_CFG_OUT_STR_HIGH);
    }

    /* unknown mode */
    digital_pin_to_pin_function[pin] = PIN_FUNC_UNUSED;
    return (GPIO_DO_NOT_CONFIG);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    /* undo any previous plumbing */
    switch (digital_pin_to_pin_function[pin])
    {
        case PIN_FUNC_ANALOG_OUTPUT:
            stopAnalogWrite(pin);
            break;

        case PIN_FUNC_ANALOG_INPUT:
            stopAnalogRead(pin);
            break;
    }

    GPIO_PinConfig gpioConfig = mode2gpioConfig(pin, mode);

    if (gpioConfig != GPIO_DO_NOT_CONFIG) {
        GPIO_setConfig(pin, gpioConfig);
    }
}

int digitalRead(uint8_t pin)
{
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_DIGITAL_INPUT) {
        pinMode(pin, INPUT);
    }

    if (GPIO_read(pin)) {
        return (HIGH);
    }

    return (LOW);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    if (digital_pin_to_pin_function[pin] != PIN_FUNC_DIGITAL_OUTPUT) {
        pinMode(pin, OUTPUT);
    }

    GPIO_write(pin, val ? 1 : 0);
}
