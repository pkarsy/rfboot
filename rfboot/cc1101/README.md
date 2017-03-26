This is the panStamp library converted from C++ to C. Also a lot of simplifications have been done.

rfboot (the bootloader part) is written in C, so there is the need to have a C library for the CC1101 chip

The file pin_macros.h contains code to replace pinMode, digitalRead, digitalWrite in a C environment. The code circulates in many places
in the Internet, and should be Public Domain.
