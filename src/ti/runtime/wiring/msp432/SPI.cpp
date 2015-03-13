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

#include "wiring_private.h"
#include "SPI.h"

SPIClass::SPIClass(void)
{
    init(0);
}

SPIClass::SPIClass(unsigned long module)
{
    init(module);
}

/*
 * Private Methods
 */
void SPIClass::init(unsigned long module)
{
    spiModule = module;
    begun = false;
}

/*
 * Public Methods
 */
void SPIClass::begin(uint8_t ssPin)
{
    SPI_Params params;

    /* return if SPI already started */
    if (begun == TRUE) return;

    Board_initSPI();

    SPI_Params_init(&params);

    spi = SPI_open(spiModule, &params);

    if (spi != NULL) {
        slaveSelect = ssPin;
        if (slaveSelect != 0) {
            pinMode(slaveSelect, OUTPUT); //set SS as an output
        }

        GateMutex_construct(&gate, NULL);
        begun = TRUE;
    }
}

void SPIClass::begin() {
    /* default CS is under user control */
    begin(0);
}

void SPIClass::end(uint8_t ssPin) {
    SPI_close(spi);
    if (slaveSelect != 0) {
        pinMode(slaveSelect, INPUT);
    }
}

void SPIClass::end() {
    end(slaveSelect);
}

void SPIClass::setBitOrder(uint8_t ssPin, uint8_t bitOrder)
{
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
}

void SPIClass::setDataMode(uint8_t mode)
{
}

void SPIClass::setClockDivider(uint8_t divider)
{
}

uint8_t SPIClass::transfer(uint8_t ssPin, uint8_t data_out, uint8_t transferMode)
{
    char data_in;

    GateMutex_enter(GateMutex_handle(&gate));

    if (slaveSelect != 0) {
        digitalWrite(ssPin, LOW);
    }
    transaction.txBuf = &data_out;
    transaction.rxBuf = &data_in;
    transaction.count = 1;
    SPI_transfer(spi, &transaction);

    if (transferMode == SPI_LAST && slaveSelect != 0) {
        digitalWrite(ssPin, HIGH);
    }

    GateMutex_leave(GateMutex_handle(&gate), 0);
    return ((uint8_t)data_in);
}

uint8_t SPIClass::transfer(uint8_t ssPin, uint8_t data)
{
    return (transfer(ssPin, data, SPI_LAST));
}

uint8_t SPIClass::transfer(uint8_t data)
{
    return (transfer(slaveSelect, data, SPI_LAST));
}

void SPIClass::setModule(uint8_t module) {
    spiModule = module;
    begin(slaveSelect);
}

/*
 * Pre-Initialize a SPI instances
 */
SPIClass SPI(0);
//SPIClass SPI1(1);
