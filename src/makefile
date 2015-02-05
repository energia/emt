#
# Makefile to build the entire emt tree
#

#
# include prequisite product location variables
#
include $(firstword $(wildcard products.mak ../products.mak products.info.mak))

TOP ?= ..

VERS_UNDER ?= 1_00_00_00

PRODUCT = $(TOP)/src/bundles/emt_$(VERS_UNDER)

ifneq (sh.exe,$(SHELL))
XDCROOT := $(subst \,/,$(XDCROOT))
endif

ifneq (,$(wildcard $(XDCROOT)/bin/rm.exe))
    RM    = $(XDCROOT)/bin/rm -f
    RMDIR = $(XDCROOT)/bin/rm -rf
    CPDIR = $(XDCROOT)/bin/cp -rf
else
    RM    = rm -f
    RMDIR = rm -rf
    CPDIR = cp -rf
endif

SYSBIOS ?= $(wildcard $(TIRTOS)/products/bios_6*)

#
# set XDCARGS and XDCPATH
#
export XDCARGS = ti.targets.arm.elf.M4F=$(ti.targets.arm.elf.M4F)
export XDCPATH = $(SYSBIOS)/packages;$(TIRTOS)/packages;$(DRVLIB)

.PHONY: all clean release product .buildinfo

.buildinfo:
	@echo "=============== Dependencies ==============="
	@echo "CURDIR   = $(CURDIR)"
	@echo "TIRTOS   = $(TIRTOS)"
	@echo "SYSBIOS  = $(SYSBIOS)"
	@echo "DRVLIB   = $(DRVLIB)"
	@echo "XDCROOT  = $(XDCROOT)"
	@echo "XDCARGS   = $(XDCARGS)"
	@echo "============================================"

all: .buildinfo
	$(XDCROOT)/xdc --jobs=4 .interfaces -P ti/platforms/*
	$(XDCROOT)/xdc --jobs=4 XDCOPTIONS=v .libraries -Pr .

release: .buildinfo
	$(XDCROOT)/xdc --jobs=4 XDCOPTIONS=v DRVLIB=$(DRVLIB) $@ -Pr .
	@echo making energia closures ...
	-$(MAKE) -C bundles/energia all msp432LP/closure.zip

clean:
	$(XDCROOT)/xdc --jobs=4 -k $@ -Pr .
	@echo removing generated product bundle ...
	-$(RMDIR) $(PRODUCT)
	@echo cleaning energia closures ...
	-$(MAKE) -C bundles/energia $@
	-$(RM) msp432LP/closure.zip

product: $(PRODUCT) release
	@echo building emt product bundle: $< ...
	$(XDCROOT)/xdc XDCPATH="^/..;$(XDCPATH)" release -P $<

$(PRODUCT): $(TOP)/src/bundles/emt_VERS
	@echo updating $@ ...
	$(CPDIR) $(TOP)/src/bundles/emt_VERS $@
