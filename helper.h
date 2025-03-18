#ifndef HELPER_H
#define HELPER_H

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

#endif
