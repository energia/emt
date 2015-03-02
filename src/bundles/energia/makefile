# define default set of platforms, compiler toolchains, and tests to build
PLATFORMS ?= msp432LP cc3200LP
TESTS     ?= blink hello alloc

# define (optional) tools to run on output files to summarize footprint
XDCROOT ?= $(wildcard $(TREES)/xdcprod/xdcprod-t67/product/Linux/xdctools*)
OBJTOOL  = $(if $(XDCROOT),$(XDCROOT)/xs -c $(CURDIR)/objsum.xs,:)
MAPTOOL  = $(if $(XDCROOT),$(XDCROOT)/xs -c $(CURDIR)/mapsum.xs -t $1,:)

export XDCROOT
export OBJTOOL

# define top-level build command
define BUILD
    @echo ======== $1 ========
    +$(MAKE) --no-print-directory -C $1 $2 $3

endef

TREE_ROOT = $(firstword $(subst /src/, /src/,$(CURDIR)))
TREE_NAME = $(notdir $(TREE_ROOT))

# ensure "all" is the default build goal
all: makefile

%/closure.zip: | all
	@echo making $@ "[ $(TREE_NAME) ]" ...
	@echo "version: $(TREE_NAME)" > $*/closure/version.txt
	@echo "source : $(TREE_ROOT)" >> $*/closure/version.txt
	@cd $* && $(XDCROOT)/bin/zip -rq closure.zip closure

# rule to build the specified goal for all platforms, compilers, and tests
%:
	$(foreach p,$(PLATFORMS), $(call BUILD, $p, $@,))
	$(foreach p,$(PLATFORMS), \
            $(foreach t,$(TESTS), \
                $(call BUILD, $p/tests/$t, $@, MAPTOOL="$(call MAPTOOL,gnu)")))
