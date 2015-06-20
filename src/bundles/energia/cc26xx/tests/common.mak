#
#  Example makefile to build a simple CC26xx executable using:
#      o a "closure" of a pre-configured TI-RTOS with MT wiring
#      o the CC26xx DriverLib, and
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
#    CC2650STK_BLE  - TI CC2650 SensorTag
VARIANT ?= CC2650STK_BLE
PROGNAME ?= = blink

# define CC26xx DriverLib libs and headers based on definitions above
SDK_LIBS = $(DRVLIB.cc26xx)/driverlib/bin/ccs/driverlib.lib

#define board-specific library path
BRD_LIBS = $(CLOSURE)/ti/runtime/wiring/cc26xx/variants/$(VARIANT)
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
#all: $(patsubst %.cpp,%.obj,$(SOURCES)) $(PROGNAME).size
all: $(patsubst %.cpp,%.obj,$(SOURCES))

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
