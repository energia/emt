all: wifi.bin

wifi.bin: wifi.xm4g
	@$(MSG) making $@ ...
	${gnu.targets.arm.M4.rootDir}/bin/arm-none-eabi-objcopy -O binary $< $@
	cp $@ ../zumo

clean::
	$(RM) wifi.bin
