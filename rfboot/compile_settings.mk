# F_CPU = 8000000L

# This does not allow code to be uploaded, unless the reset button is pressed
# can be useful if the project has stable software but there is need for some
# rare updades
# ENABLE_AT_HW_RESET_ONLY = 1

# There may be cases we want the bootloader disabled unless a jumber is connected.
# The typical use is to set the pin as INPUT with PULLUP enabled. At boot rfboot checks
# the state of the PIN, and if it is LOW then rfboot enables the RF and waits for code
# ENABLE_AT_LOW_PIN_ONLY = 1
# ENABLE_PIN = B,0
