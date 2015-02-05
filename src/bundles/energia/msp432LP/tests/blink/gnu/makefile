#
#  Example makefile to build a simple MSP432 executable using:
#      o a "closure" of a pre-configured TI-RTOS with MT wiring
#      o the MSP432 DriverLib, and
#      o a GCC arm compiler/linker
#
TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

# external pre-requisites (modify these to suit your environment):
#    CLOSURE - location of the output of the "closure" tool applied
#              to some configuration of TI-RTOS
#    SDKROOT - the MSP432 DriverLib product installation folder
#    CCROOT  - installation directory of the TI Arm compiler
#

# (if present) include definitions of the macros described above
-include ../../../tools.mak

# if not already defined, define the macros to work in the emt repo
CLOSURE ?= ../../../gnu/closure
SDKROOT ?= $(wildcard $(TREE_ROOT)/imports/MSP432_DriverLib_*/msp430ware)
CCROOT  ?= $(TOOLS)/vendors/linaro/gcc-arm-none-eabi-4_7-2012q4

# tell make where to find source files
vpath %.c   $(CURDIR)/..
vpath %.cpp $(CURDIR)/..

# define MSP432 DriverLib libs and headers based on definitions above
SDK_LIBS = $(SDKROOT)/driverlib/MSP432P4xx/ccs/msp432p4xx_driverlib.lib

SDK_INCS = -I"$(SDKROOT)/driverlib" \
           -I"$(SDKROOT)/driverlib/inc" \
           -I"$(SDKROOT)/driverlib/inc/CMSIS" \
           -I"$(SDKROOT)/driverlib/MSP432P4xx"

# define TI-RTOS and Energia wiring headers based on CLOSURE above
CFG_INCS = -I "$(CLOSURE)" -I "$(CLOSURE)/src" \
           -I "$(CLOSURE)/ti/msp432/runtime/wiring" 

# C compiler-specific options and commands
#    --cmd-file=...  - use the options defined in the specified file
#    -g              - compile for debug
#
CCOPTS   = -Os @"$(CLOSURE)/compiler.opt" -g -D__MSP432P401R__=1
CC       = $(CCROOT)/bin/arm-none-eabi-gcc -c
LINK     = $(CCROOT)/bin/arm-none-eabi-gcc $(CCOPTS) -nostartfiles -Wl,--no-wchar-size-warning -Wl,-static -Wl,--gc-sections -L"$(CLOSURE)" -L"$(CCROOT)/lib"
BINIFY   = $(CCROOT)/bin/arm-none-eabi-objcopy -O binary

XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t61/product/Linux/xdctools*)

ifeq (,$(MAPTOOL))
    MAPTOOL = :
endif
ifeq (,$(OBJTOOL))
    OBJTOOL = :
endif

ifeq (sh.exe,$(SHELL))
    RM := cmd.exe /c DEL /F
    RMDIR := cmd.exe /c RMDIR /S/Q
else
    RM := rm -f
    RMDIR := rm -rf
endif

# build rules
all: blink.out blink.bin blink.size

blink.out: blink.obj main.obj

%.obj: %.cpp makefile
	@echo armcl $*.cpp ...
	$(CC) $(CCOPTS) -I "$(CCROOT)/include"  $(CFG_INCS) $(SDK_INCS) $< -o $@

%.out: %.obj main.obj makefile
	@echo armlink $*.obj ...
	$(LINK) $*.obj main.obj -Wl,-T"$(CLOSURE)/linker.cmd" $(SDK_LIBS) -lstdc++ -lgcc -lc -lm -lnosys -Wl,-Map=$*.map -o $@

%.bin: %.out makefile
	@echo binify $< ...
	$(BINIFY) $< $@

%.size: %.out makefile
	-@$(OBJTOOL) $<
	-@$(MAPTOOL) $*.map

clean:
	-@$(RM) *.obj
	-@$(RM) *.out
	-@$(RM) *.map
	-@$(RM) *.bin
	-@$(RM) *.size
