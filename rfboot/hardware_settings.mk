# Uncomment to enable external crystal.
# If you uncomment this setting you MUST include a crystal in your project.
# Default is internal oscillator @ 8Mhz
# Only "1" is accepted as true
#CRYSTAL = 1

# The default MCU clock rate is 8MHz
# WARNING: Change this if you are using external crystal
# with the apropriate freequency
#F_CPU = 8000000L

# only if CRYSTAL above is NOT set.
# Uncomment to enable automatic calibration of the
# internal RC oscillator (only 8Mhz without clock divider)
# https://github.com/pkarsy/rcCalibrator
# If this is enabled, rfboot will set the OSCCAL register, so application
# will have CPU clock close to 8Mhz. The OSCCAL value will be
# compiled in the rfboot code, so no worries about reserved EEPROM locations.
# If the executable is in the PATH, "osccal" is enough.
# To be extra sure, check if the compilation emits the message :
# #pragma message "NOTICE: rfboot will set OSCCAL value"
#RC_CALIBRATOR = osccal

# Uncomment to set usbtiny as ISP programmer
# Default is USBasp.
# usbasp and usbtiny are supported.
# "usbtiny" is in lower case
#PROGRAMMER = usbtiny
