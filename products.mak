#
#  XDCROOT
#  TIRTOS
#  DRVLIB
#
CCSROOT ?= c:/Users/dr/tools/ccs6_0_1_39
CCSROOT := $(subst \,/,$(CCSROOT))

TIRTOS  ?= $(lastword $(wildcard c:/ti/tirtos_msp430_2_12_*))
XDCROOT  = $(lastword $(wildcard $(CCSROOT)/xdctools_*/.))
DRVLIB   = $(wildcard $(TIRTOS)/products/MSP43*)
ti.targets.arm.elf.M4F = $(lastword $(wildcard $(CCSROOT)/ccsv6/tools/compiler/*arm_5.*))
gnu.targets.arm.M4F    = $(lastword $(wildcard $(CCSROOT)/ccsv6/tools/compiler/gcc-arm-*))

ifeq (,$(XDCROOT))
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
