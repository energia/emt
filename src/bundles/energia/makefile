# define default set of platforms, compiler toolchains, and tests to build
PLATFORMS ?= msp432LP
TESTS     ?= blink

# define (optional) tools to run on output files to summarize footprint
XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t67/product/Linux/xdctools*)
OBJTOOL  = $(if $(XDCROOT),$(XDCROOT)/xs -c $(CURDIR)/objsum.xs,:)
MAPTOOL  = $(if $(XDCROOT),$(XDCROOT)/xs -c $(CURDIR)/mapsum.xs -t $1,:)

export XDCROOT
export OBJTOOL

# define top-level build command
define BUILD
    @echo ======== $1 ========
    $(MAKE) --no-print-directory -C $1 $2 $3

endef

# ensure "all" is the default build goal
all: makefile

msp432LP/closure.zip: | all
	@echo making $@ from gnu build ...
	@cd msp432LP && $(XDCROOT)/bin/zip -rq closure.zip closure

# rule to build the specified goal for all platforms, compilers, and tests
%:
	$(foreach p,$(PLATFORMS), $(call BUILD, $p, $@,))
	$(foreach p,$(PLATFORMS), \
            $(foreach t,$(TESTS), \
                $(call BUILD, $p/tests/$t, $@, MAPTOOL="$(call MAPTOOL,gnu)")))
