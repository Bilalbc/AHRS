#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "helper.h"
#include "SPI_LSM6DSOX.h"

void setup_LSM6DSOX() {
    printf("Reading WHO_AM_I: ");
    uint8_t response = spiCreateAndSendRequest(0x0f, NO_DATA, true);
    printf("%02x\n", response);

    printf("Setting Up LSM6DSOX XL");
    spiCreateAndSendRequest(0x10, 0x40, false);

    printf("checking setup of XL: ");
    uint8_t responseAccel = spiCreateAndSendRequest(0x10, NO_DATA, true);
    printf("%02x\n", responseAccel);
}

uint8_t spiCreateAndSendRequest(short reg, char data, bool read) {
    int requestSize = 2;
    uint8_t request[requestSize];
    if(read) createSPIReadReq(request, reg);
    else createSPIWriteReq(request, reg, data);

    return spiSendRequest(request, 2);
}

uint16_t read_Accel_x() {
    printf("ACCELEROMETER READ INFORMATION");
    while(!check_data_ready(XLDA)) sleep_ms(100);
    
    return ((spiCreateAndSendRequest(OUTX_H_A, NO_DATA, true) << 8 ) | 
                spiCreateAndSendRequest(OUTX_L_A, NO_DATA, true));
}

bool check_data_ready(uint8_t data_rdy_bit) {
    if(spiCreateAndSendRequest(STATUS_REG, NO_DATA, true) & data_rdy_bit) {
        return true;
    }
    return false;
}

static void createSPIWriteReq(uint8_t* request, short reg, char data) {
    request[0] = (0x00 | reg); //MSB = 0, register address (8-14)
    request[1] = data; // add Data to write (0-7)
    
    // printf("\nWRITE REQUEST:");
    // printSerialPacket(request[0], request[1]);
}

static void createSPIReadReq(uint8_t* request, char reg) {
    request[0] = 0x80 | reg; //MSB = 1, register address (8-14)
    request[1] = 0x00; 
    
    // printf("\nREAD REQUEST:");
    // printSerialPacket(request[0], request[1]);
}


static uint8_t spiSendRequest(uint8_t* request, int request_size) {
    uint8_t inbuf[request_size]; // needs to be allocated memory
    for(int i = 0; i < request_size; i++) inbuf[i] = 0; // initialize to all 0
    
    gpio_put(PIN_CS, 0); // pull CS low

    // If the request is a read request
    if(request[0] & MASK(7)) {
        // printf("PREPARING READ REQUEST\n");
        // 2*sizeof since we are sending two bytes (request[0], and request[1])
        spi_write_read_blocking(SPI_PORT, request, inbuf, request_size*sizeof(uint8_t)); 
        // printbuf(inbuf, 2);
        
    } else { // if the request is a write request
        // printf("PREPARING WRITE REQUEST\n");
        spi_write_blocking(SPI_PORT, request, request_size*sizeof(uint8_t));
    }

    gpio_put(PIN_CS, 1); // pull CS high

    return inbuf[1];
}
