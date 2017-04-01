#NIM := $(shell nim --version 2>/dev/null)


all:
#ifndef NIM
#	$(shell /bin/echo $(NIM) )
#	@$(error "No nim compiler found")
#else
	rm -f rftool
	nim c -d:release --opt:size -x:on rftool.nim
	#nim c rftool.nim
	strip rftool
	ls -l rftool
#endif

# nim -d:release --opt:size --passL:-static --gcc.exe:diet --gcc.linkerexe:diet c rftool
# strip -s rftool
#nim -d:release --opt:size --passL:-static --gcc.exe:musl-gcc --gcc.linkerexe:musl-gcc c rftool
#strip -s rftool
