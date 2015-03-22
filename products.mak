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

TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

# function to get latest versioned path
latest = $(lastword $(sort $(wildcard $1)))

#
#  CCS build support
#
CCSROOT ?= c:/ti
CCSROOT := $(subst \,/,$(CCSROOT))

TIRTOS  ?= $(call latest, c:/ti/tirtos_msp430_2_12_*)
XDCROOT  = $(call latest, $(CCSROOT)/xdctools_*/.)

DRVLIB.msp432 = $(wildcard $(TIRTOS)/products/MSPWare*)
DRVLIB.cc3200 = $(wildcard $(TIRTOS)/products/CC32*)

ti.targets.arm.elf.M4F = $(call latest,$(CCSROOT)/ccsv6/tools/compiler/*arm_5.*)
gnu.targets.arm.M4F    = $(call latest,$(CCSROOT)/ccsv6/tools/compiler/gcc-arm-*)

ifeq (,$(XDCROOT))
    #
    # UNIX TISB tree build support
    #
    XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_31_01_28_eng/$(BUILD_HOST_OS)
    TIRTOS  = $(wildcard $(TREES)/zumaprod/zumaprod-f20/exports/tirtos_full_*)
    SYSBIOS = $(TREE_ROOT)/imports/bios_6_41_03_49_eng
    DRVLIB.msp432 = $(wildcard $(TIRTOS)/products/MSPWare*)
    DRVLIB.cc3200 = $(wildcard $(TIRTOS)/products/CC32*)
    ti.targets.arm.elf.M4F = $(TOOLS)/vendors/ti/arm/5.2.2/$(BUILD_HOST_OS)
    gnu.targets.arm.M4F    = $(TOOLS)/vendors/linaro/4.8-2014q3/$(BUILD_HOST_OS)
endif

ti.targets.arm.elf.M4  = $(ti.targets.arm.elf.M4F)
gnu.targets.arm.M4     = $(gnu.targets.arm.M4F)

#
#  Error checks
#
ifeq (,$(wildcard $(XDCROOT)))
    $(error XDCROOT, '$(XDCROOT)', does not reference a valid directory)
endif
ifeq (,$(wildcard $(TIRTOS)))
    $(error TIRTOS, '$(TIRTOS)', does not reference a valid directory)
endif
ifeq (,$(wildcard $(DRVLIB.msp432)))
    $(error DRVLIB.msp432, '$(DRVLIB)', does not reference a valid directory)
endif
