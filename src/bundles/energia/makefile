# define default set of platforms, compiler toolchains, and tests to build
PLATFORMS ?= msp432LP
COMPILERS ?= gnu ti
TESTS     ?= blink

COMPILERS = gnu

# define (optional) tools to run on output files to summarize footprint
XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t61/product/Linux/xdctools*)
OBJTOOL  = $(if $(XDCROOT),$(XDCROOT)/xs -c $(CURDIR)/objsum.xs,:)
MAPTOOL  = $(if $(XDCROOT),$(XDCROOT)/xs -c $(CURDIR)/mapsum.xs -t $1,:)

export XDCROOT
export OBJTOOL

# define top-level build command
define BUILD
    @echo ======== $1 ========
    @$(MAKE) --no-print-directory -C $1 $2 $3

endef

# ensure "all" is the default build goal
all: makefile

msp432LP/closure.zip: | all
	@echo making $@ from gnu build ...
	@cd msp432LP/gnu && $(XDCROOT)/bin/zip -rq ../closure.zip closure

# rule to build the specified goal for all platforms, compilers, and tests
%:
	$(foreach p,$(PLATFORMS), \
            $(foreach c,$(COMPILERS),$(call BUILD, $p/$c, $@,)))
	$(foreach p,$(PLATFORMS), \
            $(foreach t,$(TESTS), \
	        $(foreach c,$(COMPILERS), $(call BUILD, $p/tests/$t/$c, $@, MAPTOOL="$(call MAPTOOL,$c)"))))
