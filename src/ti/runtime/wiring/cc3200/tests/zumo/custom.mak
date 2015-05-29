all: energia.bin

energia.bin: energia.xm4g
	@$(MSG) making $@ ...
	${gnu.targets.arm.M4.rootDir}/bin/arm-none-eabi-objcopy -O binary $< $@

clean::
	$(RM) energia.bin
