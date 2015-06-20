#
#  Example makefile to build a simple CC3200 executable using:
#      o a "closure" of a pre-configured TI-RTOS with MT wiring
#      o the CC3200 DriverLib, and
#      o a GCC arm compiler/linker
#
TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

# external pre-requisites (modify these to suit your environment):
#    CLOSURE - location of the output of the "closure" tool applied
#              to some configuration of TI-RTOS
#    DRVLIB  - the DriverLib product installation folder
#    CCROOT  - installation directory of the TI Arm compiler
#

# include definitions of the macros described above
include ../../tools.mak

# if not already defined, define the macros to work in the emt repo
CLOSURE ?= ../../closure

# tell make where to find source files
vpath %.c   $(CURDIR)
vpath %.cpp $(CURDIR)

# Supported variants include:
#    CC3200_LAUNCHXL	  - TI LaunchPad
#    RedBearLab_WiFi_Mini - RedBear WiFi Mini
#    RedBearLab_CC3200    - RedBear CC3200 (arduino UNO clone)
VARIANT ?= CC3200_LAUNCHXL
PROGNAME ?= = blink

# define CC3200 DriverLib libs and headers based on definitions above
SDK_LIBS = $(DRVLIB.cc3200)/driverlib/ccs/Release/driverlib.a

#define board-specific library path
BRD_LIBS = $(CLOSURE)/ti/runtime/wiring/cc3200/variants/$(VARIANT)
BRD_DEFS = -DBOARD_$(VARIANT)

# define TI-RTOS and Energia wiring headers based on CLOSURE above
CFG_INCS = -I "$(CLOSURE)" -I "$(CLOSURE)/src" \
           -I "$(CLOSURE)/ti/runtime/wiring"

# C compiler-specific options and commands
#    --cmd-file=...  - use the options defined in the specified file
#    -g              - compile for debug
#
CCOPTS   = -Os @"$(CLOSURE)/compiler.opt" -gdwarf-3 -gstrict-dwarf -g -Dxdc__nolocalnames=1 -fno-exceptions $(BRD_DEFS)
CC       = $(CCROOT)/bin/arm-none-eabi-gcc -c
LINK     = $(CCROOT)/bin/arm-none-eabi-gcc $(CCOPTS) -nostartfiles -Wl,--no-wchar-size-warning -Wl,-static -Wl,--gc-sections -L"$(CLOSURE)" -L"$(BRD_LIBS)"

XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t67/product/Linux/xdctools*)

ifeq (,$(MAPTOOL))
    MAPTOOL = $(XDCROOT)/xs -c $(TREE_ROOT)/src/bundles/energia/mapsum.xs -t gnu
endif
ifeq (,$(OBJTOOL))
    OBJTOOL = $(XDCROOT)/xs -c $(TREE_ROOT)/src/bundles/energia/objsum.xs
endif

ifeq (sh.exe,$(SHELL))
    RM := cmd.exe /c DEL /F
    RMDIR := cmd.exe /c RMDIR /S/Q
else
    RM := rm -f
    RMDIR := rm -rf
endif

# build rules
all: $(patsubst %.cpp,%.obj,$(SOURCES)) $(PROGNAME).size

$(PROGNAME).out: $(PROGNAME).obj main.obj

%.obj: %.cpp makefile
	@echo armcl $*.cpp ...
	$(CC) $(CCOPTS) -I "$(CCROOT)/include" $(CFG_INCS) $< -o $@

%.out: %.obj main.obj makefile
	@echo armlink $*.obj ...
	$(LINK) $*.obj main.obj -Wl,-T"$(CLOSURE)/linker.cmd" $(SDK_LIBS) -lstdc++ -lgcc -lc -lm -lnosys -Wl,-Map=$*.map -o $@

%.size: %.out makefile
	-@$(OBJTOOL) -x $(CCROOT)/bin/arm-none-eabi-objdump $<
	-@$(MAPTOOL) $*.map

clean:
	-@$(RM) *.obj
	-@$(RM) *.out
	-@$(RM) *.map
	-@$(RM) *.size
