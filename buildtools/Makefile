all: download unpack patch build

download: FORCE
	wget https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases/download/arc-2016.03/arc_gnu_2016.03_sources.tar.gz

unpack: FORCE
	tar xfvz arc_gnu_2016.03_sources.tar.gz

patch: FORCE
	patch arc_gnu_2016.03_sources/gcc/gcc/config/arc/arc.c < arc_gcc.patch

build: FORCE
	cd arc_gnu_2016.03_sources/toolchain/ && ./build-all.sh --install-dir ../../arc_gnu_bin --no-uclibc --no-multilib --no-pdf

FORCE: 
