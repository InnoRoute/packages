#!/bin/bash
sed \
	-e 's#@SERIAL_CONFIG@#$(strip $(GRUB_SERIAL_CONFIG))#g' \
	-e 's#@TERMINAL_CONFIG@#$(strip $(GRUB_TERMINAL_CONFIG))#g' \
	-e 's#@CMDLINE@#$(strip $(call Image/cmdline/$(1)) $(BOOTOPTS) $(GRUB_CONSOLE_CMDLINE))#g' \
	-e 's#@TIMEOUT@#$(GRUB_TIMEOUT)#g' \
	./grub_skelet.cfg > grub.cfg
