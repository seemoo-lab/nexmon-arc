all: buildtools 

get_firmware:
	mkdir linux-firmware
	@printf "\033[0;31m  DOWNLOADING FIRMWARE VERSION 4.1.0.55\033[0m\n"
	cd linux-firmware; git init; \
		git remote add origin git://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git; \
		git fetch; \
		git checkout 936b035d5ec7225c20d86784edeb8b3146194405 wil6210.fw
	cp linux-firmware/wil6210.* /firmwares/wil6210/4-1-0_55/

buildtools: FORCE
	@printf "\033[0;31m  BUILDING BUILDTOOLS\033[0m\n"
	$(Q)make -C $@

FORCE:
