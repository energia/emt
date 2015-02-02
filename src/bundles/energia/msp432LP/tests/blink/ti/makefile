#
#  Example makefile to build a simple CC3200 executable using:
#      o a "closure" of a pre-configured TI-RTOS with MT wiring
#      o the CC3200 simplelink SDK, and
#      o the TI arm compiler
#
TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))

# external pre-requisites (modify these to suit your environment):
#    CLOSURE - location of the output of the "closure" tool applied
#              to some configuration of TI-RTOS
#    SDKROOT - installation directory of the CC3200 SDK
#    CCROOT  - installation directory of the TI Arm compiler
#

# (if present) include definitions of the macros described above
-include ../../../tools.mak

vpath %.c   $(CURDIR)/..
vpath %.cpp $(CURDIR)/..

# if not already defined, define the macros to work in the tink2 repo
CLOSURE ?= ../../../ti/closure
SDKROOT ?= $(wildcard $(TREE_ROOT)/imports/CC3200SDK*)
CCROOT  ?= $(TOOLS)/vendors/ti/arm/5.1.5/Linux

# define CC3200 SDK libs and headers based on definitions above
SDK_LIBS = -l"$(SDKROOT)/cc3200-sdk/simplelink/ccs/OS/simplelink.a" \
           -l"$(SDKROOT)/cc3200-sdk/oslib/ccs/ti_rtos/ti_rtos.a" \
           -l"$(SDKROOT)/cc3200-sdk/driverlib/ccs/Release/driverlib.a"

SDK_INCS = -I"$(SDKROOT)/cc3200-sdk/simplelink/include" \
           -I"$(SDKROOT)/cc3200-sdk"

# define TI-RTOS and Energia wiring headers based on CLOSURE above
CFG_INCS = -I "$(CLOSURE)" -I "$(CLOSURE)/src" \
           -I "$(CLOSURE)/tink/runtime/wiring" 

# C compiler-specific options and commands
#    --cmd-file=...  - use the options defined in the specified file
#    -g              - compile for debug
#    --code-state=16 - allow use of thumb mode instructions
#    --relaxed_ansi  - allow non-ANSI (gcc) C++'isms
#
CCOPTS  = --cmd_file="$(CLOSURE)/compiler.opt" -g --code_state=16 --relaxed_ansi -D cc3200=1 -D CCWARE=1
CC      = $(CCROOT)/bin/armcl
LINK    = $(CCROOT)/bin/armcl $(CCOPTS) -z

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
all: blink.out blink.size

blink.out: blink.obj main.obj

%.obj: %.cpp makefile
	@echo armcl $*.cpp ...
	$(CC) $(CCOPTS) -I "$(CCROOT)/include"  $(CFG_INCS) $(SDK_INCS) $<

%.out: %.obj main.obj CC3200.cmd makefile
	@echo armlink $*.obj ...
	@$(LINK) -m$*.map -i"$(CLOSURE)" -i"$(CCROOT)/lib" -i"$(CCROOT)/include" --reread_libs --warn_sections --rom_model -o $@  $*.obj main.obj CC3200.cmd -l"$(CLOSURE)/linker.cmd" $(SDK_LIBS) -llibc.a

%.size: %.out makefile
	-@$(OBJTOOL) $<
	-@$(MAPTOOL) $*.map

clean:
	-@$(RM) *.obj
	-@$(RM) *.out
	-@$(RM) *.map
	-@$(RM) *.bin
	-@$(RM) *.size
