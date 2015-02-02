#
# XDCtools used to build this tree
#
XDCROOT = $(TOOLS)/vendors/xdc/xdctools_3_30_05_60/$(BUILD_HOST_OS)

#
#  trees containing imports
#
ZUMAPROD_TREE = $(TREES)/zumaprod/zumaprod-f07
PLATFORM_TREE = $(TREES)/platform/platform-o70

#
# Specific imports
#
TIRTOS = $(wildcard $(ZUMAPROD_TREE)/exports/tirtos_full_*)
CATALOG = $(PLATFORM_TREE)/src

SYSBIOS = $(wildcard $(TIRTOS)/products/bios_6*)
## HACK:
SYSBIOS = $(wildcard $(TREES)/avalaprod/avalaprod-p41/exports/bios_6_*/packages/..)

TIRTOS_REPOS = $(TIRTOS)/packages;$(SYSBIOS)/packages

IMPORT_ARCHIVES = $(CURDIR)/downloads/MSP432_DriverLib_01_03_00_18.zip
IMPORT_PACKAGES =

REFERENCED_REPOS = $(CATALOG)
