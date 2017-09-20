all: download unpack patch build gcc-nexmon-plugin

download: FORCE
	if [ ! -f "arc_gnu_2016.03_sources.tar.gz" ]; then \
		wget https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases/download/arc-2016.03/arc_gnu_2016.03_sources.tar.gz; \
	fi

unpack: FORCE
	if [ ! -d "arc_gnu_2016.03_sources/" ]; then \
		tar xfvz arc_gnu_2016.03_sources.tar.gz; \
	fi

patch: FORCE
	patch arc_gnu_2016.03_sources/gcc/gcc/config/arc/arc.c < arc_gcc.patch

build: FORCE
	cd arc_gnu_2016.03_sources/toolchain/ && ./build-all.sh --cpu arc600 --install-dir ../../arc_gnu_bin --no-uclibc --no-multilib --no-pdf

gcc-nexmon-plugin: FORCE
	$(Q)make -C $@

FORCE: 
