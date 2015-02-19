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
#    DRVLIB  - the CC3200 DriverLib product installation folder
#    CCROOT  - installation directory of the TI Arm compiler
#

# include definitions of the macros described above
include ../../tools.mak

# if not already defined, define the macros to work in the emt repo
CLOSURE ?= ../../closure

# tell make where to find source files
vpath %.c   $(CURDIR)
vpath %.cpp $(CURDIR)

# define CC3200 DriverLib libs and headers based on definitions above
SDK_LIBS = $(DRVLIB.cc3200)/driverlib/ccs/Release/driverlib.a

# define TI-RTOS and Energia wiring headers based on CLOSURE above
CFG_INCS = -I "$(CLOSURE)" -I "$(CLOSURE)/src" \
           -I "$(CLOSURE)/ti/cc3200/runtime/wiring" 

# C compiler-specific options and commands
#    --cmd-file=...  - use the options defined in the specified file
#    -g              - compile for debug
#
CCOPTS   = -Os @"$(CLOSURE)/compiler.opt" -g
CC       = $(CCROOT)/bin/arm-none-eabi-gcc -c
LINK     = $(CCROOT)/bin/arm-none-eabi-gcc $(CCOPTS) -nostartfiles -Wl,--no-wchar-size-warning -Wl,-static -Wl,--gc-sections -L"$(CLOSURE)" -L"$(CCROOT)/lib"
BINIFY   = $(CCROOT)/bin/arm-none-eabi-objcopy -O binary

XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t67/product/Linux/xdctools*)

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
	$(CC) $(CCOPTS) -I "$(CCROOT)/include" $(CFG_INCS) $< -o $@

%.out: %.obj main.obj makefile
	@echo armlink $*.obj ...
	$(LINK) $*.obj main.obj -Wl,-T"$(CLOSURE)/linker.cmd" $(SDK_LIBS) -lstdc++ -lgcc -lc -lm -lnosys -Wl,-Map=$*.map -o $@

%.bin: %.out makefile
	@echo binify $< ...
	$(BINIFY) $< $@

%.size: %.out makefile
	-@$(OBJTOOL) -x $(CCROOT)/bin/arm-none-eabi-objdump $<
	-@$(MAPTOOL) $*.map

clean:
	-@$(RM) *.obj
	-@$(RM) *.out
	-@$(RM) *.map
	-@$(RM) *.bin
	-@$(RM) *.size
