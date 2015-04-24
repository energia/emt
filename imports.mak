#
# XDCtools used to build this tree
#
XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_31_01_31_eng/$(BUILD_HOST_OS)

#
# internal TISB trees containing imports
#
ZUMAPROD_TREE = $(TREES)/zumaprod/zumaprod-g02

#
# Specific required imports (referenced by makeunix, for example)
#
TIRTOS  = $(wildcard $(ZUMAPROD_TREE)/exports/tirtos_full_*)
SYSBIOS = $(wildcard $(TIRTOS)/products/bios_6*)

#
# SM-MAKE support
#
IMPORT_ARCHIVES = 
IMPORT_PACKAGES =

REFERENCED_REPOS = $(TIRTOS) $(SYSBIOS)
