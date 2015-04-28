#ifndef pins_energia_h
#define pins_energia_h

#if defined(BOARD_CC3200LP)
#include <ti/runtime/wiring/cc3200/variants/emt3200LP/pins_energia.h>
#elif defined(BOARD_MSP432LP)
#include <ti/runtime/wiring/msp432/variants/emt432LP/pins_energia.h>
#endif

#endif
