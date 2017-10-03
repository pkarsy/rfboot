NIM := $(shell which nim)

ifeq ($(NIM),)
@$(error No "nim" compiler found. Install it from https://nim-lang.org )
endif

all:
	rm -f rftool
	nim c -d:release --opt:size -x:on rftool.nim
	strip rftool
	ls -l rftool

musl:
	nim -d:release --opt:size --passL:-static --gcc.exe:musl-gcc --gcc.linkerexe:musl-gcc c rftool
	strip rftool
	ls -l rftool

vagga:
	vagga nim -d:release --opt:size --passL:-static --gcc.exe:musl-gcc --gcc.linkerexe:musl-gcc c rftool
	strip rftool
	ls -l rftool

clean:
	rm -rf nimcache rftool
