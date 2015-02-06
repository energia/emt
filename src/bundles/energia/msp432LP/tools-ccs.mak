#  This file defines the installation locations of prerequisite components
#  for the MSP432 LaunchPad Energia MT wiring port:
#      CCROOT  - the TI arm compiler (version 5.1.5 or above)
#      SDKROOT - the MSP432 driverlib product install directory
#      XDCROOT - XDCtools (version 3.30.05 or above)
#      TIRTOS  - TI-RTOS for MSP432
#      EMTROOT - the packages/ folder in the EMT product
#
#  For example, on Windows, these might be set as follows:
#      CCROOT  = "C:/ti/ccs6.0.190/ccsv6/tools/compiler/arm_5.1.5"
#      SDKROOT = "C:/ti/MSP432_DriverLib_01_03_00_18"
#      XDCROOT = "C:/ti/xdctools_3_30_05_60_core"
#      TIRTOS  = "C:/ti/tirtos_simplelink_2_10_01_38"
#      EMTROOT   = "C:/ti/tink2_1_00_00_40/packages"
#

TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

CCSROOT ?= c:/Users/dr/tools/ccs6_1_0_95
CCSROOT := $(subst \,/,$(CCSROOT))

# define multiple toolchains to support side-by-side builds
CCROOT.ti  = $(lastword $(wildcard $(CCSROOT)/ccsv6/tools/compiler/ti-cgt-arm_5.2*))
CCROOT.gnu = $(lastword $(wildcard $(CCSROOT)/ccsv6/tools/compiler/gcc-arm-none-eabi*))

# set CCROOT based on the current directory's name
TOOLCHAIN = $(notdir $(CURDIR))
ifeq (,$(CCROOT.$(TOOLCHAIN)))
    $(error The $(TOOLCHAIN) compiler is not configured on this host)
endif

# define locations of all prerequisites
CCROOT  = $(CCROOT.$(TOOLCHAIN))
XDCROOT = $(lastword $(wildcard $(CCSROOT)/xdctools_3_*))
TIRTOS  = $(lastword $(wildcard c:/ti/tirtos_msp430*))
SDKROOT = $(wildcard $(TIRTOS)/products/MSP430Ware_*)
EMTROOT = $(TREE_ROOT)/src
