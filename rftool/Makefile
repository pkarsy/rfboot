NIM := $(shell which nim)

ifeq ($(NIM),)
@$(error No "nim" compiler found. Install it from https://nim-lang.org )
endif

all:
	rm -f rftool
	nim c -d:release --opt:size -x:on rftool.nim
	strip rftool
	ls -l rftool

# nim -d:release --opt:size --passL:-static --gcc.exe:diet --gcc.linkerexe:diet c rftool
# strip -s rftool
#nim -d:release --opt:size --passL:-static --gcc.exe:musl-gcc --gcc.linkerexe:musl-gcc c rftool
#strip -s rftool

musl:
	nim -d:release --opt:size --passL:-static --gcc.exe:/usr/local/musl/bin/musl-gcc --gcc.linkerexe:/usr/local/musl/bin/musl-gcc c rftool
	strip rftool
	ls -l rftool

