#
# XDCtools used to build this tree
#
XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_31_01_28_eng/$(BUILD_HOST_OS)

#
# internal TISB trees containing imports
#
ZUMAPROD_TREE = $(TREES)/zumaprod/zumaprod-f20

#
# Specific required imports (referenced by makeunix, for example)
#
TIRTOS  = $(wildcard $(ZUMAPROD_TREE)/exports/tirtos_full_*)
DRVLIB  = $(wildcard $(CURDIR)/imports/MSPWare*)
DRVLIB  = $(wildcard $(TIRTOS)/products/MSPWare*)
#SYSBIOS = $(wildcard $(TIRTOS)/products/bios_6*)
SYSBIOS = $(CURDIR)/imports/bios_6_41_03_49_eng

#
# SM-MAKE support
#
IMPORT_ARCHIVES = $(CURDIR)/downloads/bios_6_41_03_49_eng.zip
IMPORT_PACKAGES =

REFERENCED_REPOS = $(TIRTOS) $(SYSBIOS)
