/*
 ************************************************************************
 *  wiring_digital.cpp
 *
 *  Energia core files for cc3200
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
#include <rom.h>
#include <rom_map.h>

extern GPIO_HWAttrs gpioHWAttrs[];

void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t hwAttrIndex = digital_pin_to_hwAttrs_index[pin];

    if (hwAttrIndex == NOT_A_PORT) return;

    uint32_t pinMask = gpioHWAttrs[hwAttrIndex].pin;
    uint32_t portNum = (uint32_t) gpioHWAttrs[hwAttrIndex].port;

    gpioHWAttrs[hwAttrIndex].direction = GPIO_INPUT;
    digital_pin_to_pin_function[pin] = PIN_FUNC_DIGITAL_INPUT;

    if (mode == INPUT) {
        MAP_GPIO_setAsInputPinWithPullUpResistor(portNum, pinMask);
    } else if (mode == INPUT_PULLUP) {
        MAP_GPIO_setAsInputPinWithPullUpResistor(portNum, pinMask);
    } else if (mode == INPUT_PULLDOWN) {
        MAP_GPIO_setAsInputPinWithPullUpResistor(portNum, pinMask);
    } else {//mode == OUTPUT
        MAP_GPIO_setAsOutputPin(portNum, pinMask);
        MAP_GPIO_setDriveStrengthHigh(portNum, pinMask);
        MAP_GPIO_setOutputLowOnPin(portNum, pinMask);
        gpioHWAttrs[hwAttrIndex].direction = GPIO_OUTPUT;
        digital_pin_to_pin_function[pin] = PIN_FUNC_DIGITAL_OUTPUT;
    }
}

int digitalRead(uint8_t pin)
{
    uint8_t hwAttrIndex = digital_pin_to_hwAttrs_index[pin];

    if (hwAttrIndex == NOT_A_PORT) {
        return (LOW);
    }

    if (digital_pin_to_pin_function[pin] != PIN_FUNC_DIGITAL_INPUT) {
        pinMode(pin, INPUT);
    }

    if (GPIO_read(hwAttrIndex)) {
        return (HIGH);
    }

    return (LOW);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    uint8_t hwAttrIndex = digital_pin_to_hwAttrs_index[pin];

    if (hwAttrIndex == NOT_A_PORT) {
        return;
    }

    if (digital_pin_to_pin_function[pin] != PIN_FUNC_DIGITAL_OUTPUT) {
        pinMode(pin, OUTPUT);
    }

    GPIO_write(hwAttrIndex, val ? 0xff : 0);
}
