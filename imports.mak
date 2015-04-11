#
# XDCtools used to build this tree
#
XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_31_01_28_eng/$(BUILD_HOST_OS)

#
# internal TISB trees containing imports
#
ZUMAPROD_TREE = $(TREES)/zumaprod/zumaprod-f28

#
# Specific required imports (referenced by makeunix, for example)
#
TIRTOS  = $(wildcard $(ZUMAPROD_TREE)/exports/tirtos_full_*)
#SYSBIOS = $(wildcard $(TIRTOS)/products/bios_6*)
SYSBIOS = $(CURDIR)/imports/bios_6_41_03_51

#
# SM-MAKE support
#
IMPORT_ARCHIVES = $(CURDIR)/downloads/bios_6_41_03_51.zip
IMPORT_PACKAGES =

REFERENCED_REPOS = $(TIRTOS) $(SYSBIOS)
