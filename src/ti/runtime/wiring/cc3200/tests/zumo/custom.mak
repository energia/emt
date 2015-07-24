#PROGNAME = basename of the current working directory
PROGNAME = $(notdir $(CURDIR))

all: $(PROGNAME).bin

$(PROGNAME).bin: $(PROGNAME).xm4g
	@$(MSG) making $@ ...
	${gnu.targets.arm.M4.rootDir}/bin/arm-none-eabi-objcopy -O binary $< $@

clean::
	$(RM) $(PROGNAME).bin

