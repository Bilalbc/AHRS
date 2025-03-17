#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "helper.h"

void printbuf(uint8_t buf[], size_t len) {
    size_t i;
    for (i = 0; i < len; ++i) {
        if (i % 8 == 7)
            printf("%02x\n", buf[i]);
        else
            printf("%02x ", buf[i]);
    }

    // append trailing newline if there isn't one
    if (i % 8) {
        putchar('\n');
    }
}

void printSerialPacket(unsigned char packet_upper, unsigned char packet_lower) {
    char hexChars[] = "0123456789ABCDEF"; // List of hex characters to map with
    unsigned char packetAsString[5];

    packetAsString[0] = hexChars[(packet_upper & 0xF0) >> 4]; // get the upper nibble
    packetAsString[1] = hexChars[packet_upper & 0xF]; // get the lower nibble

    packetAsString[2] = hexChars[(packet_lower & 0xF0) >> 4]; // get the upper nibble
    packetAsString[3] = hexChars[packet_lower & 0xF]; // get the lower nibble

    packetAsString[4] = '\0'; // add string terminator
    printf("Serial Packet: %s\n", packetAsString);
}

// Copied from Datasheet - used for debugging gpio interrupt events
void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }   
            events &= ~mask;
        
            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }   
    *buf++ = '\0';
}