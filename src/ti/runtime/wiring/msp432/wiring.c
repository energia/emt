/*
 ************************************************************************
 *  wiring.c
 *
 *  Arduino core files for TI-RTOS
 *      Copyright (c) 2013 Louis Peryea. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
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
#include "Energia.h"
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

unsigned long micros(void)
{
    Types_FreqHz freq;
    Types_Timestamp64 time;
    uint64_t t64;
    
    Timestamp_getFreq(&freq);
    Timestamp_get64(&time);
    t64 = ((uint64_t)time.hi << 32) | time.lo;
    return (t64/(freq.lo/1000000));
}

unsigned long millis(void)
{
    return (Clock_getTicks());
}

/* Delay for the given number of microseconds. */
void delayMicroseconds(unsigned int us)
{
    if (us <7) {
        //The overhead in calling and returning from this function takes about 6us
    }
    else if (us <=20) {
        int time;
        for (time = 5*(us-6); time > 0; time--) {
            asm("   nop");
        }
    }
    else if (us < 70) {
        int time;
        for (time = 5*us; time > 0; time--) {
            asm("   nop");
        }
    }
    else {
        uint32_t t0, deltaT;
        Types_FreqHz freq;
        
        Timestamp_getFreq(&freq);
        deltaT = us * (freq.lo/1000000);
        
        t0 = Timestamp_get32();

        while ((Timestamp_get32()-t0) < deltaT) {
            ;
        }
    }
}

void delay(uint32_t milliseconds)
{
    Task_sleep(milliseconds);
}
