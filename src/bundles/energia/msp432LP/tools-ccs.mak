#  This file defines the installation locations of prerequisite components
#  for the MSP432 LaunchPad Energia MT wiring port:
#      CCROOT  - the TI arm compiler (version 5.1.5 or above)
#      DRVLIB  - the MSP432 driverlib product install directory
#      XDCROOT - XDCtools (version 3.30.05 or above)
#      TIRTOS  - TI-RTOS for MSP432
#      EMTROOT - the packages/ folder in the EMT product
#
#  For example, on Windows, these might be set as follows:
#      CCROOT  = "C:/ti/ccs6.0.190/ccsv6/tools/compiler/arm_5.1.5"
#      DRVLIB  = "C:/ti/MSP432_DriverLib_01_03_00_18"
#      XDCROOT = "C:/ti/xdctools_3_30_05_60_core"
#      TIRTOS  = "C:/ti/tirtos_simplelink_2_10_01_38"
#      EMTROOT = "C:/ti/emt_1_00_00_40/packages"
#

TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

CCSROOT ?= c:/Users/dr/tools/ccs6_1_0_95
CCSROOT := $(subst \,/,$(CCSROOT))

# define locations of all prerequisites
CCROOT  = $(lastword $(wildcard $(CCSROOT)/ccsv6/tools/compiler/gcc-arm-none-eabi*))
XDCROOT = $(lastword $(wildcard $(CCSROOT)/xdctools_3_*))
TIRTOS  = $(lastword $(wildcard c:/ti/tirtos_msp430*))
DRVLIB  = $(wildcard $(TIRTOS)/products/MSP430Ware_*)
