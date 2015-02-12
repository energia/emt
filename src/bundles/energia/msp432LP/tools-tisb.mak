#  This file defines the installation locations of prerequisite components
#  for the MSP432 LaunchPad Energia MT wiring port:
#      CCROOT  - an arm compiler (TI 5.2.5 or gcc 4.8)
#      DRVLIB  - the MSP432 driverlib product install directory
#      XDCROOT - XDCtools (version 3.30.05 or above)
#      TIRTOS  - TI-RTOS for MSP432
#      EMTROOT - the packages/ folder in the EMT product
#
#  For example, on Windows, these might be set as follows:
#      CCROOT  = "C:/ti/ccs6.0.190/ccsv6/tools/compiler/arm_5.1.5"
#      DRVLIB  = "C:/ti/MSP430Ware_01_03_00_18/msp430ware"
#      XDCROOT = "C:/ti/xdctools_3_30_05_60_core"
#      TIRTOS  = "C:/ti/tirtos_simplelink_2_10_01_38"
#      EMTROOT = "C:/ti/emt_1_00_00_40/packages"
#

TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

# define locations of all prerequisites
CCROOT  = $(TOOLS)/vendors/linaro/4.8-2014q3/Linux
XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_31_00_19_core_eng/Linux
TIRTOS  = $(wildcard $(TREES)/zumaprod/zumaprod-f09/exports/tirtos_full_*)
DRVLIB  = $(wildcard $(TIRTOS)/products/MSP430Ware_*/msp430ware)
