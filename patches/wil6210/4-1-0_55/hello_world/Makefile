QFIM=../../../../buildtools/qfim.py
FW_PATH=$(NEXMON_ROOT)/firmwares/wil6210/4-1-0_55
include $(FW_PATH)/definitions.mk

LOCAL_SRCS=$(wildcard src/*.c)
COMMON_SRCS=$(wildcard $(NEXMON_ROOT)/patches/common/*.c)
FW_SRCS=$(wildcard $(FW_PATH)/*.c)

OBJS=$(addprefix obj/,$(notdir $(LOCAL_SRCS:.c=.o)) $(notdir $(COMMON_SRCS:.c=.o)) $(notdir $(FW_SRCS:.c=.o)))

CFLAGS= \
	-fplugin=$(CCPLUGIN) \
	-fplugin-arg-nexmon-objfile=$@ \
	-fplugin-arg-nexmon-prefile=gen/nexmon.pre \
	-fplugin-arg-nexmon-chipver=$(NEXMON_CHIP_NUM) \
	-fplugin-arg-nexmon-fwver=$(NEXMON_FW_VERSION_NUM) \
	-DNEXMON_CHIP=$(NEXMON_CHIP) \
	-DNEXMON_FW_VERSION=$(NEXMON_FW_VERSION) \
    -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=$(NEXMON_ARCH) \
	-ffunction-sections -fdata-sections -msize-level=3 -mno-sdata \
	-I$(NEXMON_ROOT)/patches/include \
	-I$(FW_PATH) \
	-Isrc

all: $(FW_FILE)

init: FORCE
	$(Q)make -s -f $(NEXMON_ROOT)/patches/common/header.mk
	$(Q)mkdir -p obj gen log libgcc

#$(Q)$(CC)gcc $(CFLAGS) -c $< -o $@ >>log/compiler.log
obj/%.o: src/%.c
	@printf "\033[0;31m  COMPILING\033[0m %s => %s (details: log/compiler.log)\n" $< $@
	$(Q)cat gen/nexmon.pre 2>>log/error.log | gawk '{ if ($$3 != "$@") print; }' > tmp && mv tmp gen/nexmon.pre
	$(Q)$(CC)gcc $(CFLAGS) -c $< -o $@ >>log/compiler.log

#$(Q)$(CC)gcc $(CFLAGS) -c $< -o $@ >>log/compiler.log
obj/%.o: $(NEXMON_ROOT)/patches/common/%.c
	@printf "\033[0;31m  COMPILING\033[0m %s => %s (details: log/compiler.log)\n" $< $@
	$(Q)cat gen/nexmon.pre 2>>log/error.log | gawk '{ if ($$3 != "$@") print; }' > tmp && mv tmp gen/nexmon.pre
	$(Q)$(CC)gcc $(CFLAGS) -c $< -o $@ >>log/compiler.log

#$(Q)$(CC)gcc $(CFLAGS) -c $< -o $@ >>log/compiler.log
obj/%.o: $(FW_PATH)/%.c
	@printf "\033[0;31m  COMPILING\033[0m %s => %s (details: log/compiler.log)\n" $< $@
	$(Q)cat gen/nexmon.pre 2>>log/error.log | gawk '{ if ($$3 != "$@") print; }' > tmp && mv tmp gen/nexmon.pre
	$(Q)$(CC)gcc $(CFLAGS) -c $< -o $@ >>log/compiler.log

libgcc: FORCE
	@printf "\033[0;31m  EXTRACTING LIBGCC\033[0m %s => libgcc/...\n" $<
	$(Q)cd libgcc && $(CC)ar -xv `$(CC)gcc -mcpu=$(NEXMON_ARCH) -print-libgcc-file-name` >> /dev/null

gen/fw.ld: $(OBJS) libgcc
	@printf "\033[0;31m  ADDING LIBGCC\033[0m => gen/nexmon.pre\n"
	$(Q)ls -1 libgcc | gawk '{ print "fwpatch TARGETREGION libgcc/" $$1; }' >> gen/nexmon.pre
	$(Q)gawk '!a[$$0]++' gen/nexmon.pre > tmp && mv tmp gen/nexmon.pre
	@printf "\033[0;31m  GENERATING LINKER FILE\033[0m gen/nexmon.pre => %s\n" $@
	$(Q)sort gen/nexmon.pre | gawk -f $(NEXMON_ROOT)/buildtools/scripts/fw.ld.awk > $@

gen/uc.ld: $(OBJS) libgcc
	@printf "\033[0;31m  ADDING LIBGCC\033[0m => gen/nexmon.pre\n"
	$(Q)ls -1 libgcc | gawk '{ print "ucpatch TARGETREGION libgcc/" $$1; }' >> gen/nexmon.pre
	$(Q)gawk '!a[$$0]++' gen/nexmon.pre > tmp && mv tmp gen/nexmon.pre
	@printf "\033[0;31m  GENERATING LINKER FILE\033[0m gen/nexmon.pre => %s\n" $@
	$(Q)sort gen/nexmon.pre | gawk -f $(NEXMON_ROOT)/buildtools/scripts/uc.ld.awk > $@

gen/fw.mk: $(OBJS) $(FW_PATH)/definitions.mk
	@printf "\033[0;31m  GENERATING MAKE FILE\033[0m gen/nexmon.pre => %s (details: log/compiler.log)\n" $@
	$(Q)printf "gen/$(FW_CODE_FILE): gen/fw_patch.elf FORCE\n" > $@
	$(Q)sort gen/nexmon.pre | \
		gawk -v src_file=gen/fw_patch.elf -f $(NEXMON_ROOT)/buildtools/scripts/fw.mk.1.awk 2>>log/compiler.log | \
		gawk -v ramstart=$(FW_CODE_START) -f $(NEXMON_ROOT)/buildtools/scripts/fw.mk.2.awk >> $@ 2>>log/compiler.log
	$(Q)printf "\nFORCE:\n" >> $@
	$(Q)gawk '!a[$$0]++' $@ > tmp && mv tmp $@

gen/uc.mk: $(OBJS) $(FW_PATH)/definitions.mk
	@printf "\033[0;31m  GENERATING MAKE FILE\033[0m gen/nexmon.pre => %s (details: log/compiler.log)\n" $@
	$(Q)printf "gen/$(UC_CODE_FILE): gen/uc_patch.elf FORCE\n" > $@
	$(Q)sort gen/nexmon.pre | \
		gawk -v src_file=gen/uc_patch.elf -f $(NEXMON_ROOT)/buildtools/scripts/uc.mk.1.awk 2>>log/compiler.log | \
		gawk -v ramstart=$(UC_CODE_START) -f $(NEXMON_ROOT)/buildtools/scripts/uc.mk.2.awk >> $@ 2>>log/compiler.log
	$(Q)printf "\nFORCE:\n" >> $@
	$(Q)gawk '!a[$$0]++' $@ > tmp && mv tmp $@

gen/memory.ld: $(FW_PATH)/definitions.mk
	@printf "\033[0;31m  GENERATING LINKER FILE\033[0m %s\n" $@
	$(Q)printf "fwcode : ORIGIN = 0x%08x, LENGTH = 0x%08x\n" $(FW_CODE_START) $(FW_CODE_SIZE) >> $@
	$(Q)printf "uccode : ORIGIN = 0x%08x, LENGTH = 0x%08x\n" $(UC_CODE_START) $(UC_CODE_SIZE) >> $@
	$(Q)printf "fwpatch : ORIGIN = 0x%08x, LENGTH = 0x%08x\n" $(FW_PATCH_START) $(FW_PATCH_SIZE) >> $@
	$(Q)printf "ucpatch : ORIGIN = 0x%08x, LENGTH = 0x%08x\n" $(UC_PATCH_START) $(UC_PATCH_SIZE) >> $@

gen/fw_patch.elf: fw_patch.ld gen/fw.ld gen/memory.ld $(OBJS)
	@printf "\033[0;31m  LINKING OBJECTS\033[0m => %s (details: log/linker.log, log/linker.err)\n" $@
	$(Q)$(CC)ld -T $< -o $@ --gc-sections --print-gc-sections -M >>log/linker.log 2>>log/linker.err

gen/uc_patch.elf: uc_patch.ld gen/uc.ld gen/memory.ld $(OBJS)
	@printf "\033[0;31m  LINKING OBJECTS\033[0m => %s (details: log/linker.log, log/linker.err)\n" $@
	$(Q)$(CC)ld -T $< -o $@ --gc-sections --print-gc-sections -M >>log/linker.log 2>>log/linker.err

gen/$(FW_CODE_FILE): $(FW_PATH)/$(FW_FILE) gen/fw_patch.elf gen/fw.mk
	@printf "\033[0;31m  EXTRACTING\033[0m %s => %s (details: log/qfim.log)\n" $(FW_PATH)/$(FW_FILE) $@
	$(Q)$(QFIM) get data --filein $(FW_PATH)/$(FW_FILE) -r $(FW_CODE_PARTITION) -d $@ >>log/qfim.log
	@printf "\033[0;31m  APPLYING PATCHES\033[0m gen/fw.mk => %s (details: log/fw_patches.log)\n" $@
	$(Q)make -f gen/fw.mk >>log/fw_patches.log 2>>log/fw_patches.err

gen/$(UC_CODE_FILE): $(FW_PATH)/$(FW_FILE) gen/uc_patch.elf gen/uc.mk
	@printf "\033[0;31m  EXTRACTING\033[0m %s => %s (details: log/qfim.log)\n" $(FW_PATH)/$(FW_FILE) $@
	$(Q)$(QFIM) get data --filein $(FW_PATH)/$(FW_FILE) -r $(UC_CODE_PARTITION) -d $@ >>log/qfim.log
	@printf "\033[0;31m  APPLYING PATCHES\033[0m gen/uc.mk => %s (details: log/uc_patches.log)\n" $@
	$(Q)make -f gen/uc.mk >>log/uc_patches.log 2>>log/uc_patches.err

gen/$(FW_DATA_FILE): $(FW_PATH)/$(FW_FILE)
	@printf "\033[0;31m  EXTRACTING\033[0m %s => %s (details: log/qfim.log)\n" $(FW_PATH)/$(FW_FILE) $@
	$(Q)$(QFIM) get data --filein $(FW_PATH)/$(FW_FILE) -r $(FW_DATA_PARTITION) -d $@ >>log/qfim.log

gen/$(UC_DATA_FILE): $(FW_PATH)/$(FW_FILE)
	@printf "\033[0;31m  EXTRACTING\033[0m %s => %s (details: log/qfim.log)\n" $(FW_PATH)/$(FW_FILE) $@
	$(Q)$(QFIM) get data --filein $(FW_PATH)/$(FW_FILE) -r $(UC_DATA_PARTITION) -d $@ >>log/qfim.log

# WARNING qfim set data does override the contents of 'filein' in case it gets called multiple times
$(FW_FILE): init gen/$(FW_CODE_FILE) gen/$(UC_CODE_FILE) gen/$(FW_DATA_FILE) gen/$(UC_DATA_FILE)
	$(Q)cp $(FW_PATH)/$(FW_FILE) .
	@printf "\033[0;31m  INSERTING\033[0m %s => %s (details: log/qfim.log)\n" gen/$(FW_DATA_FILE) $@
	$(Q)$(QFIM) set data --filein $(FW_FILE) -r $(FW_DATA_PARTITION) --fileout $(FW_FILE) -d gen/$(FW_DATA_FILE) --dsize $$(($(FW_DATA_SIZE))) >>log/qfim.log
	@printf "\033[0;31m  INSERTING\033[0m %s => %s (details: log/qfim.log)\n" gen/$(FW_CODE_FILE) $@
	$(Q)$(QFIM) set data --filein $(FW_FILE) -r $(FW_CODE_PARTITION) --fileout $(FW_FILE) -d gen/$(FW_CODE_FILE) --dsize $$(($(FW_CODE_SIZE))) >>log/qfim.log
	@printf "\033[0;31m  INSERTING\033[0m %s => %s (details: log/qfim.log)\n" gen/$(UC_CODE_FILE) $@
	$(Q)$(QFIM) set data --filein $(FW_FILE) -r $(UC_CODE_PARTITION) --fileout $(FW_FILE) -d gen/$(UC_CODE_FILE) --dsize $$(($(UC_CODE_SIZE))) >>log/qfim.log
	@printf "\033[0;31m  INSERTING\033[0m %s => %s (details: log/qfim.log)\n" gen/$(UC_DATA_FILE) $@
	$(Q)$(QFIM) set data --filein $(FW_FILE) -r $(UC_DATA_PARTITION) --fileout $(FW_FILE) -d gen/$(UC_DATA_FILE) --dsize $$(($(UC_DATA_SIZE))) >>log/qfim.log
	@printf "\033[0;31m  SETTING CAPABILITIES\033[0m 64 => %s (details: log/qfim.log)\n" $@
	$(Q)$(QFIM) set capabilities --filein wil6210.fw --fileout wil6210.fw -c 64 >>log/qfim.log

gen/$(basename $(FW_CODE_FILE)).fw_code.elf: $(FW_PATH)/$(FW_FILE) Makefile
	$(Q)mkdir -p gen log
	@printf "\033[0;31m  EXTRACTING\033[0m %s => %s (details: log/qfim.log)\n" $(FW_PATH)/$(FW_FILE) gen/tmp
	$(Q)$(QFIM) get data --filein $(FW_PATH)/$(FW_FILE) -r $(FW_CODE_PARTITION) -d gen/tmp >>log/qfim.log
	@printf "\033[0;31m  CREATING TEXT SECTION\033[0m %s => %s\n" gen/tmp $@
	$(Q)$(CC)objcopy --rename-section .data=.text --set-section-flags .text=code,alloc,load,readonly -O elf32-littlearc -Barc600 -I binary gen/tmp $@

gen/$(basename $(FW_CODE_FILE)).fw_data.elf: $(FW_PATH)/$(FW_FILE) Makefile
	$(Q)mkdir -p gen log
	@printf "\033[0;31m  EXTRACTING\033[0m %s => %s (details: log/qfim.log)\n" $(FW_PATH)/$(FW_FILE) gen/tmp
	$(Q)$(QFIM) get data --filein $(FW_PATH)/$(FW_FILE) -r $(FW_DATA_PARTITION) -d gen/tmp >>log/qfim.log
	@printf "\033[0;31m  CREATING DATA SECTION\033[0m %s => %s\n" gen/tmp $@
	$(Q)$(CC)objcopy -O elf32-littlearc -Barc700 -I binary gen/tmp $@

$(basename $(FW_CODE_FILE)).elf: gen/$(basename $(FW_CODE_FILE)).fw_code.elf gen/$(basename $(FW_CODE_FILE)).fw_data.elf Makefile
	@printf "\033[0;31m  COPYING FILE\033[0m %s => %s\n" gen/$(basename $(FW_CODE_FILE)).fw_code.elf $@
	$(Q)cp gen/$(basename $(FW_CODE_FILE)).fw_code.elf $@
	@printf "\033[0;31m  CHANGING DATA SECTION ADDRESS\033[0m to %s\n" "0x800000"
	$(Q)$(CC)objcopy --change-section-address .data=0x800000 --add-section .data=gen/wil6210.fw_data.elf $@

gdb: $(basename $(FW_CODE_FILE)).elf
	@printf "\033[0;31m  STARTING nSIM SIMULATOR\033[0m on port 1234\n"
	$(Q)$(NEXMON_ROOT)/buildtools/nSIM/bin/nsimdrv -gdb -port 1234 -props prop=1,nsim_isa_family=a700,nsim_trace=1,trace_enabled=1 &
	@printf "\033[0;31m  STARTING GDB\033[0m with %s\n" $<
	$(Q)$(CC)gdb -ex "target remote 127.0.0.1:1234" -ex "load" $<

install-firmware-router-3: $(FW_FILE)
	scp $< root@192.168.1.103:/lib/firmware
	ssh root@192.168.1.103 "ifconfig wlan2 down && sleep 1 && ifconfig wlan2 up"

test-wmi-0x864-router-3:
	ssh root@192.168.1.103 "python3 ./wt.py sendwmi 0x864"
	ssh root@192.168.1.103 "cat /sys/kernel/debug/ieee80211/phy2/wil6210/console_dump_fw"
	ssh root@192.168.1.103 "cat /sys/kernel/debug/ieee80211/phy2/wil6210/console_dump_uc"
	
enable-debugging-router-3:
	ssh root@192.168.1.103 "echo -n 'module wil6210 +p' > /sys/kernel/debug/dynamic_debug/control"


###################################################################

check-nexmon-setup-env:
ifndef NEXMON_SETUP_ENV
	$(error run 'source setup_env.sh' first in the repository\'s root directory)
endif

clean: FORCE
	@printf "\033[0;31m  CLEANING\033[0m\n"
	$(Q)rm -fr $(FW_FILE) obj gen log libgcc $(FW_FILE) $(basename $(FW_CODE_FILE)).elf

FORCE:
