#
# ======== products.mak ========
# This file defines macros for the instalation folders of all of the 
# prerequisite products required to build this tree
#
prefix = **
$(info $(prefix) In order to build this tree, you must first create the file)
$(info $(prefix)     $(CURDIR)/products.mak)
$(info $(prefix) which defines macros for each external product required by )
$(info $(prefix) this tree.  See $(CURDIR)/products.info.mak for an example.)
$(error $(CURDIR)/products.mak does not exist)

#
# Required product installation macro definitions:
#    XDCROOT - XDCtools
#    TIRTOS  - TI-RTOS
#    DRVLIB  - MSP432 Driver Lib
#
# Required compiler macro definitions:
#    ti.targets.arm.elf.M4F - GCC Arm compiler
#    gnu.targets.arm.M4F    - TI Arm compiler
#
#  The settings below can be used if you're are using CCS and
#      o CCSROOT, defined below, points to the folder where CCS6 is installed
#      o TI-RTOS is installed in the same folder (typically c:/ti)
#
CCSROOT ?= c:/Users/dr/tools/ccs6_0_1_39

latest = $(lastword $(sort $(wildcard $1)))

XDCROOT = $(latest $(CCSROOT)/xdctools_*)
TIRTOS  = $(latest $(CCSROOT)/tirtos_msp430_2_12*)
DRVLIB  = $(latest $(TIRTOS)/products/MSP43*/msp430ware)

#
# Options product installation macro definitions
#    SYSBIOS - MSP432 Driver Lib
#
ti.targets.arm.elf.M4F = $(latest $(CCSROOT)/ccsv6/tools/compiler/ti-*-arm_5.*)
gnu.targets.arm.M4F    = $(latest $(CCSROOT)/ccsv6/tools/compiler/gcc-arm-*)
