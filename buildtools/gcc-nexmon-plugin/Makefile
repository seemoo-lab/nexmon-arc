all: nexmon.so

nexmon.so: nexmon.o
	g++ -m64 -shared -o $@ $<

nexmon.o: nexmon.c
	g++ -std=c++11 -Wall -fno-rtti -Wno-literal-suffix -m64 -fPIC -I../arc_gnu_bin/lib/gcc/arc-elf32/4.8.5/plugin/include -I./include -c -o $@ $<

clean:
	rm -f *.o *.so
