# The default MCU clock rate is 8MHz
# WARNING: Change this if you are using external crystal
# with the apropriate freequency
#F_CPU = 8000000L

# Uncomment to enable external crystal
# if you uncomment this setting you MUST
# include a crystal in your project
# default is internal oscillator @ 8Mhz
# only 1 is accepted as true
#CRYSTAL = 1

# Uncomment to set usbtiny as ISP programmer
# default is usbasp
# At the moment usbasp and usbtiny are supported
#PROGRAMMER = usbtiny

# only if CRYSTAL above is not set
# Uncomment to enable automatic calibration of the
# internal RC oscillator (only 8Mhz without clock divider)
# See github .... rcCalibrator TODO
# You have to give the correct path of the calibrator executable
# If this is enabled, rfboot will set the OSCCAL register so application
# will have CPU clock close to 8Mhz. The OSCCAL value will be
# compiled in the rfboot code, so no worries about reserved EEPROM locations
RC_CALIBRATOR = $(HOME)/Projects/rcCalibrator/osccal
