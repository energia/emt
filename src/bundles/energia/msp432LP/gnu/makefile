#
#  Example makefile to build a simple MSP432 LP TI-RTOS "reusable configuration";
#  a fixed configuration of TI-RTOS that can be consumed by applications as a 
#  100% self contained set of libraries and headers.
#
#  This configuration requires the installation directory of each of the
#  following prerequisite components:
#      CCROOT  - the TI arm compiler (version 5.1.5 or above)
#      SDKROOT - the MSP432 DriverLib product installation folder
#      XDCROOT - XDCtools (version 3.30.05 or above)
#      TIRTOS  - TI-RTOS for MSP432
#      EMTROOT - the EMT packages repo
#
TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

# (if present) include definitions of the macros described above
-include ../tools.mak

# if not already defined, define the macros to work in the emt repo
CCROOT  ?= $(TOOLS)/vendors/linaro/4.8-2014q3/Linux
SDKROOT ?= $(wildcard $(TREE_ROOT)/imports/MSP432_DriverLib_*)
XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t61/product/Linux/xdctools*)
TIRTOS  ?= $(wildcard $(TREES)/zumaprod/zumaprod-f07/exports/tirtos_full_*/)
EMTROOT ?= $(TREE_ROOT)/src

# define the XDC package path based on the settings above
SYSBIOS = $(wildcard $(TIRTOS)/products/bios_*)
UIA     = $(wildcard $(TIRTOS)/products/uia_*)
## HACK!!!
SYSBIOS = $(wildcard $(TREES)/avalaprod/avalaprod-p40/exports/bios_6_*/packages/..)

XDCPATH = "$(TIRTOS)/packages;$(SYSBIOS)/packages;$(UIA)/packages;$(EMTROOT);$(SDKROOT)/.." 

ifneq (,$(wildcard $(XDCROOT)/bin/rm.exe))
    RMDIR = $(XDCROOT)/bin/rm -rf
else
    RMDIR = rm -rf
endif

all: .debug closure

# output all prerequisites used to create the configuration
.debug:
	@echo "  ---- imports ----"
	@echo "    CCROOT  = $(CCROOT)"
	@echo "    SDKROOT = $(SDKROOT)"
	@echo "    XDCROOT = $(lastword $(subst /, ,$(XDCROOT)))"
	@echo "    TIRTOS  = $(lastword $(subst /, ,$(TIRTOS)))"
	@echo "    SYSBIOS = $(shell echo $(SYSBIOS) | egrep -o 'bios_6[^/]+')"
	@echo "    UIA     = $(lastword $(subst /, ,$(UIA)))"
	@echo "    EMTROOT = $(EMTROOT)"
	@echo "  -----------------"
	@echo

# create a single directory containing _everything_ referenced by configPkg
closure: configPkg
	@$(RMDIR) $@
	@echo xs xdc.tools.closure -d $@ $< ...
	@$(XDCROOT)/xs xdc.tools.closure -d $@ -i ti.drivers.bsp -i ti.drivers.ports -i ti.drivers.ports.tirtos $<

# create a fixed configuration of TI-RTOS from energia.cfg
configPkg: ../energia.cfg
	@echo xs xdc.tools.configuro -o $@ $< ...
	@$(XDCROOT)/xs --xdcpath=$(XDCPATH) xdc.tools.configuro -o $@ -t gnu.targets.arm.M4F -p ti.platforms.emt432:MSP432P401R:true -r release -c $(CCROOT) $<

# remove all generated files/directories
clean:
	@$(RMDIR) closure configPkg src
