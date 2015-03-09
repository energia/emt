/*
 ************************************************************************
 *	pins_energia.h
 *
 *	Energia core files for MSP432
 *		Copyright (c) 2014 Robert Wessels. All right reserved.
 *
 ***********************************************************************
  Derived from:
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

#ifndef Pins_Energia_h
#define Pins_Energia_h

#include <stdbool.h>
#include <stdint.h>

static const uint8_t RED_LED = 75;
static const uint8_t GREEN_LED = 76;
static const uint8_t BLUE_LED = 77;
static const uint8_t YELLOW_LED = 78; /* Mapped to the other RED LED */
static const uint8_t PUSH1 = 73;
static const uint8_t PUSH2 = 74;

#endif
