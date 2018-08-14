- 2018-08-14 rftool : Now an additional rftool.i386 file is provided for the few still using i386 linuxes.

- 2018-08-12 rftool : Now uses the serial library

- 2018-08-08 rftool: If a serial port is in use, rftool does not try to open a new serial terminal.

- 2018-08-08 The-First-Project: Documentation improvements.

- 2018-05-26 rftool: The default rftool binary is now 64 bit. The binary is statically linked and probably will work on any 64-bit kernel. The reason for this change is that it is quite inconvenient to generate the 32-bit binaries and at the same time 32-bit linuxes are almost extint nowdays. If anyone really needs the 32 bit binary, it can easily compiled in a 32 bit box.

- 2018-05-26 rftool: Now rftool uses the standard /bin/fuser utility to stop/start the serial terminal program (gtkterm, putty etc.). Until now rftool looked at /var/lock/LCK.. files. This method however has the major disadvantage that only programs creating lockfiles are working. Gtkterm worked with ubuntu 16.04. The newer gtkterm shipped with ubuntu 18.04 does not create a lockfile anymore. The old method was very restrictive anyway, and the new method should work with any GUI serial terminal.

- 2018-04-19 README.md Installation.md: Documentation improvements.

- 2017-10-03 rftool: Fixed minor bug in rftool about the maximum firmware size.
Improved rftool Makefile.
Optional use of "vagga" container for compilation.
Now rftool accepts "new" as synonymous to "create"

- 2017-09-13 Installation.md: A lot of improvements in the documentation, new photos uploaded.

- 2017-09-10 rftool: Now rftool accept "terminal" as synonymous to "monitor"

- 2017-09-10 rftool: The special reset string "MANUAL" can be used inside app_settings.h to instruct rftool to not warn us about missing reset string. Useful if we use an external tool to reset the remote target.

- 2017-09-09 rftool: Major code cleanup

- 2017-09-09 rftool: Now upload supports .elf and .hex files, not only binary files.
This in practice means that there is no need to convert the ELF file
in binary format in the Makefile.

- 2017-09-09 CHANGELOG file created: Maybe too late, as almost all functionality is
there.
