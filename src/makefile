#
# Makefile to build the entire emt tree
#

TOP ?= ..

#
# include prequisite product/compiler location variables
#
include $(firstword $(wildcard products.mak $(PRODUCTS) products.info.mak))

#
# perform some basic error checks
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
export XDCARGS := ti.targets.arm.elf.M4F=$(ti.targets.arm.elf.M4F) \
                  gnu.targets.arm.M4F=$(gnu.targets.arm.M4F)
export XDCPATH = $(SYSBIOS)/packages;$(TIRTOS)/packages;$(DRVLIB)

## find all packages provided by this tree
ALLPKGS := $(shell "$(XDCROOT)/bin/xdcpkg" -r $(TOP)/src)

## categorize packages
NONTESTPKGS := $(patsubst ~%,,$(patsubst %~,,$(subst /tests,~ ~,$(ALLPKGS))))
TESTPKGS    := $(filter-out $(NONTESTPKGS),$(ALLPKGS))
BASEPKGS    := $(NONTESTPKGS)

.PHONY: all clean release product .buildinfo

.buildinfo:
	@echo "--------------- Dependencies ---------------"
	@echo "    CURDIR   = $(CURDIR)"
	@echo "    TIRTOS   = $(TIRTOS)"
	@echo "    SYSBIOS  = $(SYSBIOS)"
	@echo "    DRVLIB   = $(DRVLIB)"
	@echo "    XDCROOT  = $(XDCROOT)"
	@echo "    XDCARGS  = $(XDCARGS)"
	@echo "    CCSROOT  = $(CCSROOT)"
	@echo "--------------------------------------------"

export CCSROOT := $(subst \,/,$(CCSROOT))

all: .buildinfo
	$(XDCROOT)/xdc --jobs=4 .interfaces -P ti/platforms/*
	$(XDCROOT)/xdc --jobs=4 .libraries -P $(BASEPKGS)

release: .buildinfo all
	$(XDCROOT)/xdc --jobs=4 $@ -P $(BASEPKGS)
	@echo making energia closures ...
	-$(MAKE) --no-print-directory -C bundles/energia all msp432LP/closure.zip

clean:
	$(XDCROOT)/xdc --jobs=4 -k $@ -Pr .
	@echo removing generated product bundle ...
	-$(RMDIR) $(PRODUCT)
	@echo cleaning energia closures ...
	-$(MAKE) --no-print-directory -C bundles/energia $@
	-$(RM) msp432LP/closure.zip

product: $(PRODUCT) release
	@echo building emt product bundle: $< ...
	$(XDCROOT)/xdc XDCPATH="^/..;$(XDCPATH)" release -P $<

$(PRODUCT): $(TOP)/src/bundles/emt_VERS
	@echo updating $@ ...
	$(CPDIR) $(TOP)/src/bundles/emt_VERS $@
