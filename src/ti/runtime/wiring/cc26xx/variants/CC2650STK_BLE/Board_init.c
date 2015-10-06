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
 *  ====================== Board.c =============================================
 *  This file is responsible for setting up the board specific items for the
 *  CC2650 SensorTag.
 *
 *  NB! This board file is for PCB version 1.2
 */


/*
 *  ====================== Includes ============================================
 */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/ioc.h>
#include <driverlib/udma.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#include <ti/drivers/PIN.h>
#include "Board.h"

/*
 *  ========================= IO driver initialization =========================
 *  From main, PIN_init(BoardGpioInitTable) should be called to setup safe
 *  settings for this board.
 *  When a pin is allocated and then de-allocated, it will revert to the state
 *  configured in this table.
*/

/*
 *  ========================= IO driver initialization =========================
 *  From main, PIN_init(BoardGpioInitTable) should be called to setup safe
 *  settings for this board.
 *  When a pin is allocated and then de-allocated, it will revert to the state
 *  configured in this table
*/
PIN_Config BoardGpioInitTable[] = {
    Board_LED1       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off             */
    Board_LED2       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off             */
    Board_KEY_LEFT   | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,        /* Button is active low          */
    Board_KEY_RIGHT  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,        /* Button is active low          */
    Board_RELAY      | PIN_INPUT_EN | PIN_PULLDOWN | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,      /* Relay is active high          */
    Board_MPU_INT    | PIN_INPUT_EN | PIN_PULLDOWN | PIN_IRQ_NEGEDGE | PIN_HYSTERESIS,        /* MPU_INT is active low         */
    Board_TMP_RDY    | PIN_INPUT_EN | PIN_NOPULL | PIN_HYSTERESIS,                            /* TMP_RDY is active high        */
    Board_BUZZER     | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* Buzzer initially off          */
    Board_MPU_POWER  | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* MPU initially off             */
    Board_MIC_POWER  | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,     /* MIC initially off             */
    Board_SPI_FLASH_CS | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MIN,  /* External flash chip select    */
    Board_SPI_DEVPK_CS | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MIN,  /* DevPack chip select           */
    Board_SPI0_MOSI | PIN_INPUT_DIS | PIN_PULLDOWN,                                           /* disable input, enable pulldown */
    Board_SPI0_CLK  | PIN_INPUT_DIS | PIN_PULLDOWN,                                           /* disable input, enable pulldown */
    Board_AUDIO_DI | PIN_INPUT_EN | PIN_PULLDOWN,                                             /* Audio DI                      */
    Board_AUDIODO | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MIN,       /* Audio data out                */
    Board_AUDIO_CLK | PIN_INPUT_EN | PIN_PULLDOWN,                                            /* DevPack */
    Board_DP2 | PIN_INPUT_EN | PIN_PULLDOWN,                                                  /* DevPack */
    Board_DP1 | PIN_INPUT_EN | PIN_PULLDOWN,                                                  /* DevPack */
    Board_DP0 | PIN_INPUT_EN | PIN_PULLDOWN,                                                  /* DevPack */
    Board_DP3 | PIN_INPUT_EN | PIN_PULLDOWN,                                                  /* DevPack */
    Board_DP4_UARTRX | PIN_INPUT_EN | PIN_PULLDOWN,                                           /* DevPack */
    Board_DP5_UARTTX | PIN_INPUT_DIS | PIN_PUSHPULL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH,     /* Devpack */
    Board_DEVPK_ID | PIN_INPUT_EN | PIN_NOPULL,                                               /* Device pack ID - external PU  */

    PIN_TERMINATE
};
/*============================================================================*/

/*
 * ======== GPIO driver ========
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/* GPIO configuration structure definitions */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPIOCC26XX_config, ".const:GPIOCC26XX_config")
#pragma DATA_SECTION(gpioPinConfigs, ".data:gpioPinConfigs")
#pragma DATA_SECTION(gpioCallbackFunctions, ".data:gpioCallbackFunctions")
#endif

GPIO_PinConfig gpioPinConfigs[] = {
    /* port_pin */
    GPIOCC26XX_EMPTY_PIN | GPIO_DO_NOT_CONFIG,  /*  0  - dummy */
                          
    /* pins 1-10 */
    GPIOCC26XX_EMPTY_PIN | GPIO_DO_NOT_CONFIG,  /*  1  - VDD */
    GPIOCC26XX_EMPTY_PIN | GPIO_DO_NOT_CONFIG,  /*  2  - GND */
    GPIOCC26XX_DIO_06 | GPIO_DO_NOT_CONFIG,     /*  3  - DIO_06 SCL */
    GPIOCC26XX_DIO_05 | GPIO_DO_NOT_CONFIG,     /*  4  - DIO_05 SDA */
    GPIOCC26XX_DIO_16 | GPIO_DO_NOT_CONFIG,     /*  5  - DIO_16 DP12/AUDIO FS/TD0 */
    GPIOCC26XX_DIO_11 | GPIO_DO_NOT_CONFIG,     /*  6  - DIO_11 DP7/AUDIO CLK */
    GPIOCC26XX_DIO_20 | GPIO_DO_NOT_CONFIG,     /*  7  - DIO_20 DP11/CSN */
    GPIOCC26XX_DIO_22 | GPIO_DO_NOT_CONFIG,     /*  8  - DIO_22 DP6/AUDIO DO */
    GPIOCC26XX_EMPTY_PIN | GPIO_DO_NOT_CONFIG,  /*  9  - VDD */
    GPIOCC26XX_DIO_29 | GPIO_DO_NOT_CONFIG,     /*  10 - DIO_29 DP5/UART_TX */
                          
    /* pins 11-20 */
    GPIOCC26XX_DIO_19 | GPIO_DO_NOT_CONFIG,     /*  11 - DIO_19 DP10/MOSI */
    GPIOCC26XX_DIO_28 | GPIO_DO_NOT_CONFIG,     /*  12 - DIO_28 DP4/UART_RX */
    GPIOCC26XX_DIO_18 | GPIO_DO_NOT_CONFIG,     /*  13 - DIO_18 DP9/MISO */
    GPIOCC26XX_DIO_27 | GPIO_DO_NOT_CONFIG,     /*  14 - DIO_27 DP3 */
    GPIOCC26XX_DIO_17 | GPIO_DO_NOT_CONFIG,     /*  15 - DIO_17 DP8/SCLK/TDI */
    GPIOCC26XX_DIO_23 | GPIO_DO_NOT_CONFIG,     /*  16 - DIO_23 DP2  */
    GPIOCC26XX_DIO_30 | GPIO_DO_NOT_CONFIG,     /*  17 - DIO_30 DP_ID */
    GPIOCC26XX_DIO_24 | GPIO_DO_NOT_CONFIG,     /*  18 - DIO_24 DP1 */
    GPIOCC26XX_EMPTY_PIN | GPIO_DO_NOT_CONFIG,  /*  19 - POWER_GOOD */
    GPIOCC26XX_DIO_25 | GPIO_DO_NOT_CONFIG,     /*  20 - DIO_25 DP0 */
                          
    /* virtual pins 21-30 */
    GPIOCC26XX_DIO_02 | GPIO_DO_NOT_CONFIG,     /*  21 - DIO_02 AUDIO DI */
    GPIOCC26XX_DIO_12 | GPIO_DO_NOT_CONFIG,     /*  22 - DIO_12 MPU PWR */
    GPIOCC26XX_DIO_21 | GPIO_DO_NOT_CONFIG,     /*  23 - DIO_21 BUZZER */
    GPIOCC26XX_DIO_03 | GPIO_DO_NOT_CONFIG,     /*  24 - DIO_03 REED */
    GPIOCC26XX_DIO_04 | GPIO_DO_NOT_CONFIG,     /*  25 - DIO_04 BUTTON1 */
    GPIOCC26XX_DIO_01 | GPIO_DO_NOT_CONFIG,     /*  26 - DIO_01 TMP RDY */
    GPIOCC26XX_DIO_10 | GPIO_DO_NOT_CONFIG,     /*  27 - DIO_10 LED1 */
    GPIOCC26XX_DIO_07 | GPIO_DO_NOT_CONFIG,     /*  28 - DIO_07 MPU INT */
    GPIOCC26XX_DIO_13 | GPIO_DO_NOT_CONFIG,     /*  29 - DIO_13 MIC PWR */
    GPIOCC26XX_DIO_00 | GPIO_DO_NOT_CONFIG,     /*  30 - DIO_00 BUTTON2 */
                          
    /* virtual pin 31 */
    GPIOCC26XX_DIO_15 | GPIO_DO_NOT_CONFIG,     /*  31 - DIO_15 LED2 */
    GPIOCC26XX_DIO_14 | GPIO_DO_NOT_CONFIG,     /*  32 - DIO_14 FLASH CS */
};

GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* port_pin */
    NULL,  /*  0  - dummy */

    /* pins 1-10 */
    NULL,  /*  1  - VDD */
    NULL,  /*  2  - GND */
    NULL,  /*  3  - DIO_06 SCL */
    NULL,  /*  4  - DIO_05 SDA */
    NULL,  /*  5  - DIO_16 DP12/AUDIO FS/TD0 */
    NULL,  /*  6  - DIO_11 DP7/AUDIO CLK */
    NULL,  /*  7  - DIO_20 DP11/CSN */
    NULL,  /*  8  - DIO_22 DP6/AUDIO DO */
    NULL,  /*  9  - VDD */
    NULL,  /*  10 - DIO_29 DP5/UART_TX */
                    
    /* pins 11-20 */
    NULL,  /*  11 - DIO_19 DP10/MOSI */
    NULL,  /*  12 - DIO_28 DP4/UART_RX */
    NULL,  /*  13 - DIO_18 DP9/MISO */
    NULL,  /*  14 - DIO_27 DP3 */
    NULL,  /*  15 - DIO_17 DP8/SCLK/TDI */
    NULL,  /*  16 - DIO_23 DP2  */
    NULL,  /*  17 - DIO_30 DP_ID */
    NULL,  /*  18 - DIO_24 DP1 */
    NULL,  /*  19 - POWER_GOOD */
    NULL,  /*  20 - DIO_25 DP0 */
                    
    /* pins 21-30 */
    NULL,  /*  21 - DIO_02 AUDIO DI */
    NULL,  /*  22 - DIO_12 MPU PWR */
    NULL,  /*  23 - DIO_21 BUZZER */
    NULL,  /*  24 - DIO_03 REED */
    NULL,  /*  25 - DIO_04 BUTTON1 */
    NULL,  /*  26 - DIO_01 TMP RDY */
    NULL,  /*  27 - DIO_10 LED1 */
    NULL,  /*  28 - DIO_07 MPU INT */
    NULL,  /*  29 - DIO_13 MIC PWR */
    NULL,  /*  30 - DIO_00 BUTTON2 */
                    
    /* pin 31 */
    NULL,  /*  31 - DIO_15 LED2 */
    NULL,  /*  32 - DIO_14 FLASH CS */
};

/* User requested callback functions for the GPIO input signals */

/* The device-specific GPIO_config structure */
const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = ~0
};

/*
 *  ======== Board_initGPIO ========
 */
void Board_initGPIO(void)
{
    /* set up initial GPIO pin configurations */
    GPIO_init();
}

/*
 *  ============================= UART begin ===================================
*/
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartCC26XXHWAttrs, ".const:uartCC26XXHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

/* UART objects */
UARTCC26XX_Object uartCC26XXObjects[CC2650_UARTCOUNT];

/* UART hardware parameter structure, also used to assign UART pins */
const UARTCC26XX_HWAttrs uartCC26XXHWAttrs[CC2650_UARTCOUNT] = {
    {    /* CC2650_UART0 */
        .baseAddr = UART0_BASE,
        .powerMngrId = PERIPH_UART0,
        .intNum = INT_UART0,
        .intPriority = (~0),
        .txPin = Board_DP5_UARTTX,
        .rxPin = Board_DP4_UARTRX,
        .ctsPin = PIN_UNASSIGNED,
        .rtsPin = PIN_UNASSIGNED
    },
};

/* UART configuration structure */
const UART_Config UART_config[] = {
    { &UARTCC26XX_fxnTable, &uartCC26XXObjects[0], &uartCC26XXHWAttrs[0] },
    { NULL, NULL, NULL }
};

/*
 *  ======== Board_openUART ========
 *  Initialize the UART driver.
 *  Initialize the UART port's pins.
 *  Open the UART port.
 */
UART_Handle Board_openUART(UInt uartPortIndex, UART_Params *uartParams)
{
    /* Initialize the UART driver */
    /* By design, UART_init() is idempotent */
    UART_init();

    /* initialize the pins associated with the respective UART */
    switch(uartPortIndex) {
        case 0:
            break;

        default:
            return (NULL);
    }

    /* open the UART */
    return (UART_open(uartPortIndex, uartParams));
}

/*
 *  ============================= UART end =====================================
*/

/*
 *  ============================= UDMA begin ===================================
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UDMACC26XX_config, ".const:UDMACC26XX_config")
#pragma DATA_SECTION(udmaHWAttrs, ".const:udmaHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/dma/UDMACC26XX.h>

/* UDMA objects */
UDMACC26XX_Object UdmaObjects[CC2650_UDMACOUNT];

/* UDMA configuration structure */
const UDMACC26XX_HWAttrs udmaHWAttrs[CC2650_UDMACOUNT] = {
    {
        .baseAddr =  UDMA0_BASE, 
        .powerMngrId =  PERIPH_UDMA,
        .intNum = INT_UDMAERR,
        .intPriority = (~0)
    },
};

/* UDMA configuration structure */
const UDMACC26XX_Config UDMACC26XX_config[] = {
    {&UdmaObjects[0], &udmaHWAttrs[0]},
    {NULL, NULL},
};
/*
 *  ============================= UDMA end =====================================
*/

/*
 *  ========================== SPI DMA begin ===================================
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiCC26XXDMAHWAttrs, ".const:spiCC26XXDMAHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/spi/SPICC26XXDMA.h>

/* SPI objects */
SPICC26XX_Object spiCC26XXDMAObjects[CC2650_SPICOUNT];

/* SPI configuration structure, describing which pins are to be used */
const SPICC26XX_HWAttrs spiCC26XXDMAHWAttrs[CC2650_SPICOUNT] = {
    {   /* SENSORTAG_CC2650_SPI0 */
        .baseAddr = SSI0_BASE,
        .intNum = INT_SSI0,
        .intPriority = 0xC0,       /* make SPI interrupt one priority higher than default */
        .defaultTxBufValue = 0,
        .powerMngrId = PERIPH_SSI0,
        .rxChannelBitMask = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask = 1<<UDMA_CHAN_SSI0_TX,
        .mosiPin = Board_SPI0_MOSI,
        .misoPin = Board_SPI0_MISO,
        .clkPin = Board_SPI0_CLK,
        .csnPin = PIN_UNASSIGNED     /* External flash / DevPk uses SPI0 */
    },
    {   /* SENSORTAG_CC2650_SPI1 */
        .baseAddr = SSI1_BASE,
        .intNum = INT_SSI1,
        .intPriority = ~0,
        .defaultTxBufValue = 0,
        .powerMngrId = PERIPH_SSI1,
        .rxChannelBitMask  = 1<<UDMA_CHAN_SSI1_RX,
        .txChannelBitMask  = 1<<UDMA_CHAN_SSI1_TX,
        .mosiPin = Board_SPI1_MOSI,
        .misoPin = Board_SPI1_MISO,
        .clkPin = Board_SPI1_CLK,
        .csnPin = Board_SPI1_CSN
    }
};

/* SPI configuration structure */
const SPI_Config SPI_config[] = {
    /* SENSORTAG_CC2650_SPI0 */
    {&SPICC26XXDMA_fxnTable, &spiCC26XXDMAObjects[0], &spiCC26XXDMAHWAttrs[0]},
    /* SENSORTAG_CC2650_SPI1 */
    {&SPICC26XXDMA_fxnTable, &spiCC26XXDMAObjects[1], &spiCC26XXDMAHWAttrs[1]},
    {NULL, NULL, NULL},
};

/*
 *  ======== Board_openSPI ========
 */
SPI_Handle Board_openSPI(UInt spiPortIndex, SPI_Params *spiParams)
{
    /* Initialize the SPI driver */
    /* By design, SPI_init() is idempotent */
    SPI_init();

    /* initialize the pins associated with the respective UART */
    switch(spiPortIndex) {
        case 0:
        case 1:
            break;

        default:
            return(NULL);
    }

    /* open the SPI port */
    return (SPI_open(spiPortIndex, spiParams));
}

/*
 *  ========================== SPI DMA end =====================================
*/

/*
 *  ============================= I2C Begin=====================================
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cCC26xxHWAttrs, ".const:i2cCC26xxHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/i2c/I2CCC26XX.h>

/* I2C objects */
I2CCC26XX_Object i2cCC26xxObjects[CC2650_I2CCOUNT];

/* I2C configuration structure, describing which pins are to be used */
const I2CCC26XX_HWAttrs i2cCC26xxHWAttrs[CC2650_I2CCOUNT] = {
    {
        .baseAddr = I2C0_BASE,
        .powerMngrId = PERIPH_I2C0,
        .intNum = INT_I2C,
        .intPriority = (~0),
        .sdaPin = Board_I2C0_SDA0,
        .sclPin = Board_I2C0_SCL0,
    },
    {
        .baseAddr = I2C0_BASE,
        .powerMngrId = PERIPH_I2C0,
        .intNum = INT_I2C,
        .intPriority = (~0),
        .sdaPin = Board_I2C0_SDA1,
        .sclPin = Board_I2C0_SCL1,
    }
};

const I2C_Config I2C_config[] = {
    {&I2CCC26XX_fxnTable, &i2cCC26xxObjects[0], &i2cCC26xxHWAttrs[0]},
    {&I2CCC26XX_fxnTable, &i2cCC26xxObjects[1], &i2cCC26xxHWAttrs[1]},
    {NULL, NULL, NULL}
};

/*
 *  ======== Board_openI2C ========
 *  Initialize the I2C driver.
 *  Initialize the I2C port's pins.
 *  Open the I2C port.
 */
I2C_Handle Board_openI2C(UInt i2cPortIndex, I2C_Params *i2cParams)
{
    
    /* Initialize the I2C driver */
    /* By design, I2C_init() is idempotent */
    I2C_init();
    
    /* initialize the pins associated with the respective I2C */
    switch(i2cPortIndex) {
        case 0:
        case 1:
            break;

        default:
            return (NULL);
    }

    /* open the I2C */
    return (I2C_open(i2cPortIndex, i2cParams));
}

/*
 *  ========================== I2C end =========================================
*/

/*
 *  ========================== Crypto begin =======================================
 *  NOTE: The Crypto implementation should be considered experimental and not validated!
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(CryptoCC26XX_config, ".const:CryptoCC26XX_config")
#pragma DATA_SECTION(cryptoCC26XXHWAttrs, ".const:cryptoCC26XXHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/crypto/CryptoCC26XX.h>

/* Crypto objects */
CryptoCC26XX_Object cryptoCC26XXObjects[CC2650_CRYPTOCOUNT];

/* Crypto configuration structure, describing which pins are to be used */
const CryptoCC26XX_HWAttrs cryptoCC26XXHWAttrs[CC2650_CRYPTOCOUNT] = {
    {
        .baseAddr = CRYPTO_BASE,
        .powerMngrId = PERIPH_CRYPTO,
        .intNum = INT_CRYPTO,
        .intPriority = ~0,
    }
};

/* Crypto configuration structure */
const CryptoCC26XX_Config CryptoCC26XX_config[] = {
    {&cryptoCC26XXObjects[0], &cryptoCC26XXHWAttrs[0]},
    {NULL, NULL}
};

/*
 * ======== PWM driver ========
 */
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerCC26xx.h>
#include <driverlib/timer.h>

PWMTimerCC26xx_Object pwmCC26xxObjects[CC2650_PWMCOUNT];

const PWMTimerCC26xx_HWAttrs pwmCC26xxHWAttrs[CC2650_PWMCOUNT] = {
    {GPT0_BASE, TIMER_A, PERIPH_GPT0, IOID_0},
    {GPT0_BASE, TIMER_B, PERIPH_GPT0, IOID_1},
    {GPT1_BASE, TIMER_A, PERIPH_GPT1, IOID_2},
    {GPT1_BASE, TIMER_B, PERIPH_GPT1, IOID_3},
    {GPT2_BASE, TIMER_A, PERIPH_GPT2, IOID_4},
    {GPT2_BASE, TIMER_B, PERIPH_GPT3, IOID_5},
    {GPT3_BASE, TIMER_A, PERIPH_GPT3, IOID_6},
    {GPT3_BASE, TIMER_B, PERIPH_GPT3, IOID_7}
};

const PWM_Config PWM_config[] = {
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[0], &pwmCC26xxHWAttrs[0]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[1], &pwmCC26xxHWAttrs[1]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[2], &pwmCC26xxHWAttrs[2]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[3], &pwmCC26xxHWAttrs[3]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[4], &pwmCC26xxHWAttrs[4]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[5], &pwmCC26xxHWAttrs[5]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[6], &pwmCC26xxHWAttrs[6]},
    {&PWMTimerCC26xx_fxnTable, &pwmCC26xxObjects[7], &pwmCC26xxHWAttrs[7]},
    {NULL, NULL, NULL}
};

/*
 *  ======== Board_initPWM ========
 */
void Board_initPWM(void)
{
    PWM_init();
}

void Board_initPIN()
{
    PIN_init(BoardGpioInitTable);
}
/*
 *  ======== Board_init ========
 *  Initialialize the ti.platforms.tink2 hardware
 */
void Board_init(void)
{
    /* driver-independent initialization */

    /* driver-specific initialization */
    Board_initPIN();
    Board_initGPIO();
    Board_initPWM();
}

