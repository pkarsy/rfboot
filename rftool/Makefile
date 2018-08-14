#NIM := $(shell which nim)

#ifeq ($(NIM),)
#@$(error No "nim" compiler found. Install it from https://nim-lang.org )
#endif

help:
	@echo "Targets are:"
	@echo
	@echo "make bin # Creates a rftool binary of the current architecture. requires nim-lang: https://nim-lang.org"
	@echo
	@echo "make musl # Creates a statically linked binary, requires nim-lang and musl-dev"
	@echo
	@echo "make clean"
	@echo
	@echo "make vagga # Creates a i386 container with all dependencies and generates a i386 static rftool binary. Requires vagga and some disk space (1.4G last checked), but the process is fully automatic. The advandage of this binary is that it is running on EVERY i386 or amd64 linux box"
	@echo

bin:
	rm -f rftool
	nim c -d:release --opt:size -x:on rftool.nim
	strip rftool
	ls -l rftool

musl:
	nim -d:release --opt:size --passL:-static --gcc.exe:musl-gcc --gcc.linkerexe:musl-gcc c rftool
	strip rftool
	ls -l rftool

vagga:
	git clone https://github.com/euantorano/serial.nim.git --branch v1.1.0
	vagga nim --path:./serial.nim/src -d:release --opt:size --passL:-static --gcc.exe:musl-gcc --gcc.linkerexe:musl-gcc c rftool
	strip rftool
	ls -l rftool

clean:
	rm -rf nimcache rftool
