#
#  This file defines the installation location for products that are required
#  for the use of the Energia MT
#
#  XDCROOT - XDCtools used to build a configuration of TI-RTOS suitable for EMT
#  TIRTOS  - TI-RTOS
#  DRVLIB  - MSP432 driverlib
#  ti.targets.arm.elf.M4F - TI Arm compiler
#  gnu.targets.arm.M4F    - Gnu Arm compiler
#

# function to get latest versioned path
latest = $(lastword $(sort $(wildcard $1)))

#
#  CCS build support
#
CCSROOT ?= c:/ti
CCSROOT := $(subst \,/,$(CCSROOT))

TIRTOS  ?= $(call latest, c:/ti/tirtos_msp430_2_12_*)
XDCROOT  = $(call latest, $(CCSROOT)/xdctools_*/.)
DRVLIB   = $(wildcard $(TIRTOS)/products/MSP43*)
ti.targets.arm.elf.M4F = $(call latest,$(CCSROOT)/ccsv6/tools/compiler/*arm_5.*)
gnu.targets.arm.M4F    = $(call latest,$(CCSROOT)/ccsv6/tools/compiler/gcc-arm-*)

ifeq (,$(XDCROOT))
    #
    # UNIX TISB tree build support
    #
    XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_30_05_60/$(BUILD_HOST_OS)
    TIRTOS  = $(wildcard $(TREES)/zumaprod/zumaprod-f09/exports/tirtos_full_*)
    DRVLIB  = $(wildcard $(TIRTOS)/products/MSP43*/msp430ware)
    ti.targets.arm.elf.M4F = $(TOOLS)/vendors/ti/arm/5.2.2/$(BUILD_HOST_OS)
    gnu.targets.arm.M4F    = $(TOOLS)/vendors/linaro/4.8-2014q3/$(BUILD_HOST_OS)
endif

#
#  Error checks
#
ifeq (,$(wildcard $(XDCROOT)))
    $(error XDCROOT, '$(XDCROOT)', does not reference a valid directory)
endif
ifeq (,$(wildcard $(TIRTOS)))
    $(error TIRTOS, '$(TIRTOS)', does not reference a valid directory)
endif
ifeq (,$(wildcard $(DRVLIB)))
    $(error DRVLIB, '$(DRVLIB)', does not reference a valid directory)
endif
