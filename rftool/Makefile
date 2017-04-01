#NIM := $(shell nim --version 2>/dev/null)


all:
#ifndef NIM
#	$(shell /bin/echo $(NIM) )
#	@$(error "No nim compiler found")
#else
	rm -f rftool
	nim c -d:release -d:size rftool.nim
	#nim c rftool.nim
	strip rftool
	ls -l rftool
#endif
