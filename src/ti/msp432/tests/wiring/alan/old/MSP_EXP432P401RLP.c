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

/*
 *  ======== MSP_EXP432P401RLP.c ========
 *  This file is responsible for setting up the board specific items for the
 *  MSP_EXP432P401RLP board.
 */

#include <stdbool.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/interfaces/IHwi.h>

#include <msp432.h>
#include <rom.h>
#include <rom_map.h>
#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <spi.h>
#include <timer_a.h>
#include <uart.h>
#include <wdt_a.h>

#include "MSP_EXP432P401RLP.h"

/*
 *  =============================== DMA ===============================
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[32];
static Bool DMA_initialized = false;

/* Hwi_Struct used in the initDMA Hwi_construct call */
static Hwi_Struct dmaHwiStruct;

/*
 *  ======== MSP_EXP432P401RLP_errorDMAHwi ========
 */
static Void MSP_EXP432P401RLP_errorDMAHwi(UArg arg)
{
    System_printf("DMA error code: %d\n", MAP_DMA_getErrorStatus());
    MAP_DMA_clearErrorStatus();
    System_abort("DMA error!!");
}

/*
 *  ======== MSP_EXP432P401RLP_initDMA ========
 */
void MSP_EXP432P401RLP_initDMA(void)
{
    Error_Block eb;
    Hwi_Params  hwiParams;

    if (!DMA_initialized) {
        Error_init(&eb);
        Hwi_Params_init(&hwiParams);
        Hwi_construct(&(dmaHwiStruct), INT_DMA_ERR,
                      MSP_EXP432P401RLP_errorDMAHwi, &hwiParams, &eb);
        if (Error_check(&eb)) {
            System_abort("Couldn't construct DMA error hwi");
        }
        MAP_DMA_enableModule();
        MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);

        DMA_initialized = true;
    }
}

/*
 *  =============================== General ===============================
 */

/*
 *  ======== MSP_EXP432P401RLP_init ========
 */
void MSP_EXP432P401RLP_init(void) {
	MSP_EXP432P401RLP_initGPIO();
	MSP_EXP432P401RLP_initI2C();
	MSP_EXP432P401RLP_initSPI();
	MSP_EXP432P401RLP_initUART();
}

/*
 *  ======== MSP_EXP432P401RLP_initGeneral ========
 */
void MSP_EXP432P401RLP_initGeneral(void) {
}

/*
 *  =============================== GPIO ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPIO_config, ".const:GPIO_config")
//#pragma DATA_SECTION(gpioHWAttrs, ".const:gpioHWAttrs")
#endif

#include <ti/drivers/GPIO.h>

/* GPIO configuration structure */
GPIO_HWAttrs gpioHWAttrs[] = {
    /* {port, pin, direction} */
    {NULL,         NULL,      GPIO_INPUT},  /*  0  - dummy */

	/* pins 1-10 */
    {NULL,         NULL,      GPIO_INPUT},  /*  1  - 3.3V */
    {GPIO_PORT_P6, GPIO_PIN0, GPIO_INPUT},  /*  2  - P6.0_A15 */
    {GPIO_PORT_P3, GPIO_PIN2, GPIO_INPUT},  /*  3  - P3.2_URXD */
    {GPIO_PORT_P3, GPIO_PIN3, GPIO_INPUT},  /*  4  - P3.3_UTXD */
    {GPIO_PORT_P4, GPIO_PIN1, GPIO_INPUT},  /*  5  - P4.1_IO_A12 */
    {GPIO_PORT_P4, GPIO_PIN3, GPIO_INPUT},  /*  6  - P4.3_A10 */
    {GPIO_PORT_P1, GPIO_PIN5, GPIO_INPUT},  /*  7  - P1.5_SPICLK */
    {GPIO_PORT_P4, GPIO_PIN6, GPIO_INPUT},  /*  8  - P4.6_IO_A7 */
    {GPIO_PORT_P6, GPIO_PIN5, GPIO_INPUT},  /*  9  - P6.5_I2CSCL */
    {GPIO_PORT_P6, GPIO_PIN4, GPIO_INPUT},  /*  10 - P6.4_I2CSDA */

	/* pins 11-20 */
    {GPIO_PORT_P3, GPIO_PIN6, GPIO_INPUT},  /*  11 - P3.6_IO */
    {GPIO_PORT_P5, GPIO_PIN2, GPIO_INPUT},  /*  12 - P5.2_IO */
    {GPIO_PORT_P5, GPIO_PIN0, GPIO_INPUT},  /*  13 - P5.0_IO */
    {GPIO_PORT_P1, GPIO_PIN7, GPIO_INPUT},  /*  14 - P1.7_SPIMISO */
    {GPIO_PORT_P1, GPIO_PIN6, GPIO_INPUT},  /*  15 - P1.6_SPIMOSI */
    {NULL,         NULL,      GPIO_INPUT},  /*  16 - RESET */
    {GPIO_PORT_P5, GPIO_PIN7, GPIO_INPUT},  /*  17 - P5.7_IO */
    {GPIO_PORT_P3, GPIO_PIN0, GPIO_INPUT},  /*  18 - P3.0_IO */
    {GPIO_PORT_P2, GPIO_PIN5, GPIO_INPUT},  /*  19 - P2.5_IO_PWM */
    {NULL,         NULL,      GPIO_INPUT},  /*  20 - GND */

	/* pins 21-30 */
    {NULL,         NULL,      GPIO_INPUT},  /*  21 - 5V */
    {NULL,         NULL,      GPIO_INPUT},  /*  22 - GND */
	{GPIO_PORT_P6, GPIO_PIN1, GPIO_INPUT},  /*  23 - P6.1_A14 */
	{GPIO_PORT_P4, GPIO_PIN0, GPIO_INPUT},  /*  24 - P4.0_A13 */
	{GPIO_PORT_P4, GPIO_PIN2, GPIO_INPUT},  /*  25 - P4.2_A11 */
	{GPIO_PORT_P4, GPIO_PIN4, GPIO_INPUT},  /*  26 - P4.4_A9 */
	{GPIO_PORT_P4, GPIO_PIN5, GPIO_INPUT},  /*  27 - P4.5_A8 */
	{GPIO_PORT_P4, GPIO_PIN7, GPIO_INPUT},  /*  28 - P4.7_A6 */
	{GPIO_PORT_P5, GPIO_PIN4, GPIO_INPUT},  /*  29 - P5.4_IO */
	{GPIO_PORT_P5, GPIO_PIN5, GPIO_INPUT},  /*  30 - P5.5_IO */

	/* pins 31-40 */
	{GPIO_PORT_P3, GPIO_PIN7, GPIO_INPUT},  /*  31 - P3.7_IO */
	{GPIO_PORT_P3, GPIO_PIN5, GPIO_INPUT},  /*  32 - P3.5_IO */
	{GPIO_PORT_P5, GPIO_PIN1, GPIO_INPUT},  /*  33 - P5.1_IO */
	{GPIO_PORT_P2, GPIO_PIN3, GPIO_INPUT},  /*  34 - P2.3_IO */
	{GPIO_PORT_P6, GPIO_PIN7, GPIO_INPUT},  /*  35 - P6.7_IO_CAPT */
	{GPIO_PORT_P6, GPIO_PIN6, GPIO_INPUT},  /*  36 - P6.6_IO_CAPT */
	{GPIO_PORT_P5, GPIO_PIN6, GPIO_INPUT},  /*  37 - P5.6_PWM */
	{GPIO_PORT_P2, GPIO_PIN4, GPIO_INPUT},  /*  38 - P2.4_PWM */
	{GPIO_PORT_P2, GPIO_PIN6, GPIO_INPUT},  /*  39 - P2.6_PWM */
	{GPIO_PORT_P2, GPIO_PIN7, GPIO_INPUT},  /*  40 - P2.7_PWM */

    /* virtual pins 41-46 */
    {GPIO_PORT_P1, GPIO_PIN1, GPIO_INPUT},  /*  41 - P1.1 SW1 */
    {GPIO_PORT_P1, GPIO_PIN4, GPIO_INPUT},  /*  42 - P1.4 SW2 */
    {GPIO_PORT_P2, GPIO_PIN0, GPIO_INPUT},  /*  43 - P2.0 RED_LED */
    {GPIO_PORT_P2, GPIO_PIN1, GPIO_INPUT},  /*  44 - P2.1 GREEN_LED */
    {GPIO_PORT_P2, GPIO_PIN2, GPIO_INPUT},  /*  45 - P2.2 BLUE_LED */
    {GPIO_PORT_P1, GPIO_PIN0, GPIO_INPUT},  /*  46 - P1.0 LED1 */
};

GPIO_Callbacks MSP_EXP432P401RLP_gpioPort1Callbacks = {
    .port = GPIO_PORT_P1,
    .intNum = INT_PORT1,
    .hwiStruct = NULL,
    .callbackFxn = {0, 0, 0, 0, 0, 0, 0, 0}
};

GPIO_Callbacks MSP_EXP432P401RLP_gpioPort2Callbacks = {
    .port = GPIO_PORT_P2,
    .intNum = INT_PORT2,
    .hwiStruct = NULL,
    .callbackFxn = {0, 0, 0, 0, 0, 0, 0, 0}
};

GPIO_Callbacks MSP_EXP432P401RLP_gpioPort3Callbacks = {
    .port = GPIO_PORT_P3,
    .intNum = INT_PORT3,
    .hwiStruct = NULL,
    .callbackFxn = {0, 0, 0, 0, 0, 0, 0, 0}
};

GPIO_Callbacks MSP_EXP432P401RLP_gpioPort4Callbacks = {
    .port = GPIO_PORT_P4,
    .intNum = INT_PORT4,
    .hwiStruct = NULL,
    .callbackFxn = {0, 0, 0, 0, 0, 0, 0, 0}
};

GPIO_Callbacks MSP_EXP432P401RLP_gpioPort5Callbacks = {
    .port = GPIO_PORT_P5,
    .intNum = INT_PORT5,
    .hwiStruct = NULL,
    .callbackFxn = {0, 0, 0, 0, 0, 0, 0, 0}
};

GPIO_Callbacks MSP_EXP432P401RLP_gpioPort6Callbacks = {
    .port = GPIO_PORT_P6,
    .intNum = INT_PORT6,
    .hwiStruct = NULL,
    .callbackFxn = {0, 0, 0, 0, 0, 0, 0, 0}
};

const GPIO_Config GPIO_config[] = {
    /* {GPIO_HWAttrs} */
    {&gpioHWAttrs[0]},
    {&gpioHWAttrs[1]},
    {&gpioHWAttrs[2]},
    {&gpioHWAttrs[3]},
    {&gpioHWAttrs[4]},
    {&gpioHWAttrs[5]},
    {&gpioHWAttrs[6]},
    {&gpioHWAttrs[7]},
    {&gpioHWAttrs[8]},
    {&gpioHWAttrs[9]},
    {&gpioHWAttrs[10]},
    {&gpioHWAttrs[11]},
    {&gpioHWAttrs[12]},
    {&gpioHWAttrs[13]},
    {&gpioHWAttrs[14]},
    {&gpioHWAttrs[15]},
    {&gpioHWAttrs[16]},
    {&gpioHWAttrs[17]},
    {&gpioHWAttrs[18]},
    {&gpioHWAttrs[19]},
    {&gpioHWAttrs[20]},
    {&gpioHWAttrs[21]},
    {&gpioHWAttrs[22]},
    {&gpioHWAttrs[23]},
    {&gpioHWAttrs[24]},
    {&gpioHWAttrs[25]},
    {&gpioHWAttrs[26]},
    {&gpioHWAttrs[27]},
    {&gpioHWAttrs[28]},
    {&gpioHWAttrs[29]},
    {&gpioHWAttrs[30]},
    {&gpioHWAttrs[31]},
    {&gpioHWAttrs[32]},
    {&gpioHWAttrs[33]},
    {&gpioHWAttrs[34]},
    {&gpioHWAttrs[35]},
    {&gpioHWAttrs[36]},
    {&gpioHWAttrs[37]},
    {&gpioHWAttrs[38]},
    {&gpioHWAttrs[39]},
    {&gpioHWAttrs[40]},
    {&gpioHWAttrs[41]},
    {&gpioHWAttrs[42]},
    {&gpioHWAttrs[43]},
    {&gpioHWAttrs[44]},
    {&gpioHWAttrs[45]},
    {&gpioHWAttrs[46]},
    {NULL},
};

/*
 *  ======== MSP_EXP432P401RLP_initGPIO ========
 */
void MSP_EXP432P401RLP_initGPIO(void)
{
    /* Once GPIO_init is called, GPIO_config cannot be changed */
    GPIO_init();

    /* setup callbacks for all GPIO ports used */
    GPIO_setupCallbacks(&MSP_EXP432P401RLP_gpioPort1Callbacks);
    GPIO_setupCallbacks(&MSP_EXP432P401RLP_gpioPort2Callbacks);
    GPIO_setupCallbacks(&MSP_EXP432P401RLP_gpioPort3Callbacks);
    GPIO_setupCallbacks(&MSP_EXP432P401RLP_gpioPort4Callbacks);
    GPIO_setupCallbacks(&MSP_EXP432P401RLP_gpioPort5Callbacks);
    GPIO_setupCallbacks(&MSP_EXP432P401RLP_gpioPort6Callbacks);

    __extern void Wiring_GPIO_hwiIntFxn(UArg callbacks);
  
    /* hijack the common hwi func to point to Wiring's handler that clears the GPIO interrupt */
    Hwi_setFunc(Hwi_getHandle(MSP_EXP432P401RLP_gpioPort1Callbacks.intNum), (IHwi_FuncPtr)Wiring_GPIO_hwiIntFxn, (UArg)&MSP_EXP432P401RLP_gpioPort1Callbacks);
    Hwi_setFunc(Hwi_getHandle(MSP_EXP432P401RLP_gpioPort2Callbacks.intNum), (IHwi_FuncPtr)Wiring_GPIO_hwiIntFxn, (UArg)&MSP_EXP432P401RLP_gpioPort2Callbacks);
    Hwi_setFunc(Hwi_getHandle(MSP_EXP432P401RLP_gpioPort3Callbacks.intNum), (IHwi_FuncPtr)Wiring_GPIO_hwiIntFxn, (UArg)&MSP_EXP432P401RLP_gpioPort3Callbacks);
    Hwi_setFunc(Hwi_getHandle(MSP_EXP432P401RLP_gpioPort4Callbacks.intNum), (IHwi_FuncPtr)Wiring_GPIO_hwiIntFxn, (UArg)&MSP_EXP432P401RLP_gpioPort4Callbacks);
    Hwi_setFunc(Hwi_getHandle(MSP_EXP432P401RLP_gpioPort5Callbacks.intNum), (IHwi_FuncPtr)Wiring_GPIO_hwiIntFxn, (UArg)&MSP_EXP432P401RLP_gpioPort5Callbacks);
    Hwi_setFunc(Hwi_getHandle(MSP_EXP432P401RLP_gpioPort6Callbacks.intNum), (IHwi_FuncPtr)Wiring_GPIO_hwiIntFxn, (UArg)&MSP_EXP432P401RLP_gpioPort6Callbacks);
}

/*
 *  =============================== I2C ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cMSP432HWAttrs, ".const:i2cMSP432HWAttrs")
#endif

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CMSP432.h>

/* I2C objects */
I2CMSP432_Object i2cMSP432Objects[MSP_EXP432P401RLP_I2CCOUNT];

/* I2C configuration structure */
const I2CMSP432_HWAttrs i2cMSP432HWAttrs[MSP_EXP432P401RLP_I2CCOUNT] = {
    {
        .baseAddr = EUSCI_B1_BASE,
        .intNum = INT_EUSCIB1,
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK
    }
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CMSP432_fxnTable,
        .object = &i2cMSP432Objects[0],
        .hwAttrs = &i2cMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401RLP_initI2C ========
 */
void MSP_EXP432P401RLP_initI2C(void)
{
    /* Configure Pins 6.4 & 6.5 as SDA & SCL, respectively. */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                                   GPIO_PIN4 | GPIO_PIN5,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initialize the I2C driver */
    I2C_init();
}

#if 0
/*
 *  =============================== PWM ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PWM_config, ".const:PWM_config")
#pragma DATA_SECTION(pwmTimerMSP432HWAttrs, ".const:pwmTimerMSP432HWAttrs")
#endif
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerMSP432.h>

PWMTimerMSP432_Object pwmTimerMSP432Objects[MSP_EXP432P401RLP_PWMCOUNT];

/* PWM configuration structure */
const PWMTimerMSP432_HWAttrs pwmTimerMSP432HWAttrs[MSP_EXP432P401RLP_PWMCOUNT] = {
    {
        .baseAddr = TIMER_A0_BASE,
        .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1
    }
};

const PWM_Config PWM_config[] = {
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[0],
        .hwAttrs = &pwmTimerMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401RLP_initPWM ========
 */
void MSP_EXP432P401RLP_initPWM(void)
{
    /* Enable PWM output on GPIO pins */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                                                    GPIO_PIN4,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);

    PWM_init();
}
#endif
/*
 *  =============================== SDSPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SDSPI_config, ".const:SDSPI_config")
#pragma DATA_SECTION(sdspiMSP432HWAttrs, ".const:sdspiMSP432HWAttrs")
#endif

#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPIMSP432.h>

/* SDSPI objects */
SDSPIMSP432_Object sdspiMSP432Objects[MSP_EXP432P401RLP_SDSPICOUNT];

/* SDSPI configuration structure, describing which pins are to be used */
const SDSPIMSP432_HWAttrs sdspiMSP432HWAttrs[MSP_EXP432P401RLP_SDSPICOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        /* CLK, MOSI & MISO ports & pins */
        .portSCK = GPIO_PORT_P1,
        .pinSCK = GPIO_PIN5,
        .sckMode = GPIO_PRIMARY_MODULE_FUNCTION,

        .portMISO = GPIO_PORT_P1,
        .pinMISO = GPIO_PIN7,
        .misoMode = GPIO_PRIMARY_MODULE_FUNCTION,

        .portMOSI = GPIO_PORT_P1,
        .pinMOSI = GPIO_PIN6,
        .mosiMode = GPIO_PRIMARY_MODULE_FUNCTION,

        /* Chip select port & pin */
        .portCS = GPIO_PORT_P4,
        .pinCS = GPIO_PIN6
    }
};

const SDSPI_Config SDSPI_config[] = {
    {
        .fxnTablePtr = &SDSPIMSP432_fxnTable,
        .object = &sdspiMSP432Objects[0],
        .hwAttrs = &sdspiMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401RLP_initSDSPI ========
 */
void MSP_EXP432P401RLP_initSDSPI(void)
{
    SDSPI_init();
}

/*
 *  =============================== SPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiMSP432DMAHWAttrs, ".const:spiMSP432DMAHWAttrs")
#endif

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIMSP432DMA.h>

/* SPI objects */
SPIMSP432DMA_Object spiMSP432DMAObjects[MSP_EXP432P401RLP_SPICOUNT];
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(spiMSP432DMAscratchBuf, 32)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=32
#elif defined(__GNUC__)
__attribute__ ((aligned (32)))
#endif
uint8_t spiMSP432DMAscratchBuf[MSP_EXP432P401RLP_SPICOUNT];

/* SPI configuration structure, describing which pins are to be used */
const SPIMSP432DMA_HWAttrs spiMSP432DMAHWAttrs[MSP_EXP432P401RLP_SPICOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .bitOrder = EUSCI_B_SPI_MSB_FIRST,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        .scratchBufPtr = &spiMSP432DMAscratchBuf[0],
        .defaultTxBufValue = 0,

        .dmaIntNum = INT_DMA_INT1,
        .rxDMAChannelIndex = DMA_CH1_EUSCIB0RX0,
        .txDMAChannelIndex = DMA_CH0_EUSCIB0TX0
    },
    {
        .baseAddr = EUSCI_B2_BASE,
        .bitOrder = EUSCI_B_SPI_MSB_FIRST,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,

        .scratchBufPtr = &spiMSP432DMAscratchBuf[1],
        .defaultTxBufValue = 0,

        .dmaIntNum = INT_DMA_INT2,
        .rxDMAChannelIndex = DMA_CH5_EUSCIB2RX0,
        .txDMAChannelIndex = DMA_CH4_EUSCIB2TX0
    }
};

const SPI_Config SPI_config[] = {
    {
        .fxnTablePtr = &SPIMSP432DMA_fxnTable,
        .object = &spiMSP432DMAObjects[0],
        .hwAttrs = &spiMSP432DMAHWAttrs[0]
    },
    {
        .fxnTablePtr = &SPIMSP432DMA_fxnTable,
        .object = &spiMSP432DMAObjects[1],
        .hwAttrs = &spiMSP432DMAHWAttrs[1]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP432P401RLP_initSPI ========
 */
void MSP_EXP432P401RLP_initSPI(void)
{
    /*
     * NOTE: TI-RTOS examples configure EUSCIB0 as either SPI or I2C.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    /* Configure CLK, MOSI & MISO for SPI0 (EUSCI_B0) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
                                                    GPIO_PIN5 | GPIO_PIN6,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN7,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configure CLK, MOSI & MISO for SPI1 (EUSCI_B2) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,
                                                    GPIO_PIN5 | GPIO_PIN6,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                                                   GPIO_PIN7,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    MSP_EXP432P401RLP_initDMA();
    SPI_init();
}

/*
 *  =============================== UART ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartMSP432HWAttrs, ".const:uartMSP432HWAttrs")
#endif

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>

/* UART objects */
UARTMSP432_Object uartMSP432Objects[MSP_EXP432P401RLP_UARTCOUNT];

/*
 * The baudrate dividers were determined by using the MSP430 baudrate
 * calculator
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const UARTMSP432_BaudrateConfig uartMSP432Baudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    {115200, 12000000,  6,  8,  32, 1},
    {115200, 6000000,   3,  4,   2, 1},
    {115200, 3000000,   1, 10,   0, 1},
    {9600,   12000000, 78,  2,   0, 1},
    {9600,   6000000,  39,  1,   0, 1},
    {9600,   3000000,  19,  8,  85, 1},
    {9600,   32768,     3,  0, 146, 0}
};

/* UART configuration structure */
const UARTMSP432_HWAttrs uartMSP432HWAttrs[MSP_EXP432P401RLP_UARTCOUNT] = {
    {
        .baseAddr = EUSCI_A0_BASE,
        .intNum = INT_EUSCIA0,
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
                              sizeof(UARTMSP432_BaudrateConfig),
        .baudrateLUT = uartMSP432Baudrates
    }
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object = &uartMSP432Objects[0],
        .hwAttrs = &uartMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401RLP_initUART ========
 */
void MSP_EXP432P401RLP_initUART(void)
{
    /* Set P1.2 & P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN2 | GPIO_PIN3,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initialize the UART driver */
    UART_init();
}

/*
 *  =============================== Watchdog ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogMSP432HWAttrs, ".const:watchdogMSP432HWAttrs")
#endif

#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP432.h>

/* Watchdog objects */
WatchdogMSP432_Object watchdogMSP432Objects[MSP_EXP432P401RLP_WATCHDOGCOUNT];

/* Watchdog configuration structure */
const WatchdogMSP432_HWAttrs watchdogMSP432HWAttrs[MSP_EXP432P401RLP_WATCHDOGCOUNT] = {
    {
        .baseAddr = WDT_A_BASE,
        .intNum = INT_WDT_A,
        .clockSource = WDT_A_CLOCKSOURCE_SMCLK,
        .clockDivider = WDT_A_CLOCKDIVIDER_8192K
    },
};

const Watchdog_Config Watchdog_config[] = {
    {
        .fxnTablePtr = &WatchdogMSP432_fxnTable,
        .object = &watchdogMSP432Objects[0],
        .hwAttrs = &watchdogMSP432HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP432P401RLP_initWatchdog ========
 */
void MSP_EXP432P401RLP_initWatchdog(void)
{
    /* Initialize the Watchdog driver */
    Watchdog_init();
}

#if 0
/*
 *  =============================== WiFi ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(WiFi_config, ".const:WiFi_config")
#pragma DATA_SECTION(wiFiCC3100HWAttrs, ".const:wiFiCC3100HWAttrs")
#endif
#include <ti/drivers/WiFi.h>
#include <ti/drivers/wifi/WiFiCC3100.h>

/* WiFi objects */
WiFiCC3100_Object wiFiCC3100Objects[MSP_EXP432P401RLP_WIFICOUNT];

/* WiFi configuration structure */
const WiFiCC3100_HWAttrs wiFiCC3100HWAttrs[MSP_EXP432P401RLP_WIFICOUNT] = {
    {
        .irqPort = GPIO_PORT_P2,
        .irqPin = GPIO_PIN5,
        .irqIntNum = INT_PORT2,

        .csPort = GPIO_PORT_P3,
        .csPin = GPIO_PIN0,

        .enPort = GPIO_PORT_P4,
        .enPin = GPIO_PIN1
    }
};

const WiFi_Config WiFi_config[] = {
    {
        .fxnTablePtr = &WiFiCC3100_fxnTable,
        .object = &wiFiCC3100Objects[0],
        .hwAttrs = &wiFiCC3100HWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP432P401RLP_initWiFi ========
 */
void MSP_EXP432P401RLP_initWiFi(void)
{
    /* Configure EN & CS pins to disable CC3100 */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);

    /* Configure CLK, MOSI & MISO for SPI0 (EUSCI_B0) */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
                                                    GPIO_PIN5 | GPIO_PIN6,
                                                    GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN7,
                                                   GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configure IRQ pin */
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN5,
                                 GPIO_LOW_TO_HIGH_TRANSITION);

    /* Initialize SPI and WiFi drivers */
    MSP_EXP432P401RLP_initDMA();
    SPI_init();
    WiFi_init();
}
#endif
