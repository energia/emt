#
# XDCtools used to build this tree
#
XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_30_05_60/$(BUILD_HOST_OS)

#
# internal TISB trees containing imports
#
ZUMAPROD_TREE = $(TREES)/zumaprod/zumaprod-f17

#
# Specific required imports (referenced by makeunix, for example)
#
TIRTOS  = $(wildcard $(ZUMAPROD_TREE)/exports/tirtos_full_*)
DRVLIB  = $(wildcard $(CURDIR)/imports/MSP43*)
DRVLIB  = $(wildcard $(TIRTOS)/products/MSP43*/msp430ware)
SYSBIOS = $(wildcard $(TIRTOS)/products/bios_6*)

TIRTOS_REPOS = $(TIRTOS)/packages;$(SYSBIOS)/packages

#
# SM-MAKE support
#
IMPORT_ARCHIVES = 
IMPORT_PACKAGES =

REFERENCED_REPOS = $(TIRTOS) $(SYSBIOS)
