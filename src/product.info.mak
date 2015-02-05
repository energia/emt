#
# ======== products.mak ========
# This file defines macros for the instalation folders of all of the 
# pre-requisit products required to build this tree
#
$(error the macros defined in this file are required to build this tree)

CCSROOT ?= c:/Users/dr/tools/ccs6_0_1_39
#
# Required product installation macro definitions
#    XDCROOT - XDCtools
#    TIRTOS  - TI-RTOS
#    DRVLIB  - MSP432 Driver Lib
#
XDCROOT  = $(lastword $(sort $(wildcard $(CCSROOT)/xdctools_*)))

TIRTOS  = c:/ti/tirtos_msp430_2_12_00_09_eng
DRVLIB  = $(wildcard $(TIRTOS)/products/MSP43*/msp430ware)

#
# Options product installation macro definitions
#    SYSBIOS - MSP432 Driver Lib
#


