#ifndef HELPER_H
#define HELPER_H

#define RAD_TO_DEG 57.2958f  // 180 / π

static const char *gpio_irq_str[] = {
    "LEVEL_LOW", // 0x1
    "LEVEL_HIGH", // 0x2
    "EDGE_FALL", // 0x4
    "EDGE_RISE" // 0x8
};

#define MASK(x) (1UL << x)
#define MASK_VAL(x, y) (x << y)

void printSerialPacket(unsigned char packet_upper, unsigned char packet_lower);
void printbuf(uint8_t buf[], size_t len);
void gpio_event_string(char *buf, uint32_t events);

void tilt_SingleAxis_Accel(LSM6DSOX *dev);

#endif
