// If you uncomment this rfboot will generate the IV using entropy
// from the watchdog. In this case the EPPROM is not used at all
// but the bootloader delays for 2sec to collect the entropy
// My tests show that with Crystal or resonator there is no enough entropy
// Only on internal RC oscillator the method seems to work very well, because
// it has a lot of jitter
// #define USE_ENTROPY

const uint8_t RFBOOT_CHANNEL = 0;
const uint8_t RFB_SYNCWORD[] = {54, 123};
// Note XTEAKEY is only used when updating firmware. The application code does not use it
// Note also that there is no any guarantee that the encryption offers any confidenciality
const uint32_t XTEAKEY[] = {1u, 2u, 3u, 4u};

