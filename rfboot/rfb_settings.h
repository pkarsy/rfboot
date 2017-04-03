
const uint8_t RFBOOT_CHANNEL = 4;
const uint8_t RFB_SYNCWORD[] = {54, 123};
// Note XTEAKEY is only used when updating firmware. The application code does not use it
// This specific key is useless of course. Use "rftool create ProjectName" and rftool
// will generate new strong values for all the settings
const uint32_t XTEAKEY[] = {1u, 2u, 3u, 4u};

