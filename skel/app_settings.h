// 1. This file is used by the C/C++ compiler arduino sketch compile
// 2. Is parsed at runtime by rftool in order to get the parameters (channel etc) of the upload process
// Project parameters generated with "rftool create"
// Unless specified in  the command line, the values of
// XTEAKEY RFBOOT_ADDRESS APP_ADDRESS and CHANNEL are randomly generated
// with the system randomness generator (urandom, ....)
// After the bootloader is installed to the target module, you cannot change them
// If you do this, the upload process will fail
// Note also that CE_PIN and ARDUINO_CE_PIN must show to the same pin so you must be extra carefull
// even if you make changes before you install rfboot. The same goes for CSN_PIN and ARDUINO_CSN_PIN
//

const uint8_t APP_CHANNEL = 1;
const uint8_t APP_SYNCWORD[] = {100,200};
const char RESET_STRING[] = "RST_skel";
