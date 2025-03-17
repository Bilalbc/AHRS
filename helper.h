#ifndef HELPER_H
#define HELPER_H

#define NO_DATA 0x00

#define MASK(x) (1UL << x)
#define MASK_VAL(x, y) (x << y)

static void printSerialPacket(unsigned char packet_upper, unsigned char packet_lower);
void printbuf(uint8_t buf[], size_t len);

#endif
