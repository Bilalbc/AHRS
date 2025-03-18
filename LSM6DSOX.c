#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "helper.h"
#include "LSM6DSOX.h"

void LSM6DSOX_Initialize(LSM6DSOX *dev, spi_inst_t *spi, uint CS_PIN) {
    uint8_t reg_data;

    dev->spi_handle = spi;
    dev->cs_pin = CS_PIN;

    printf("--------- Initialize LSM6DSOX --------- \n");
    LSM6DSOX_ReadRegister(dev, WHO_AM_I_REG, &reg_data);
    printf("%02x", reg_data);
    if(reg_data & LSM6DSOX_WHO_AM_I) printf("LSM6DSOX Found\n");
    else while(1); // replace with error code return

    printf("Setting Up LSM6DSOX XL\n");
    reg_data = 0x21;
    LSM6DSOX_WriteRegister(dev, CTRL1_XL, &reg_data);

    printf("Setting Up INT1 Data Ready Interrupt \n");
    reg_data = 0x01;
    LSM6DSOX_WriteRegister(dev, INT1_CTRL, &reg_data);

    printf("Configuring Offsets: X: %d,\t Y: %d\t Z: %d\n", X_OFS_VAL, Y_OFS_VAL, Z_OFS_VAL);
    /* enable accel offset correction block */
    reg_data = 0x2;
    LSM6DSOX_WriteRegister(dev, CTRL7_G, &reg_data); 

    reg_data = (uint8_t) X_OFS_VAL;
    LSM6DSOX_WriteRegister(dev, X_OFS_USR, &reg_data);
    reg_data = (uint8_t) Y_OFS_VAL;
    LSM6DSOX_WriteRegister(dev, Y_OFS_USR, &reg_data);
    reg_data = (uint8_t) Z_OFS_VAL;
    LSM6DSOX_WriteRegister(dev, Z_OFS_USR, &reg_data);

    /* struct parameters */
    dev->accel_msp2[0] = 0.0f;
    dev->accel_msp2[1] = 0.0f;
    dev->accel_msp2[2] = 0.0f;
    /* TODO Return error codes as uint8_t */

}

void LSM6DSOX_ReadAccelerations(LSM6DSOX *dev) {
    uint16_t accelRaw[3];
    uint8_t regData[2]; 

    /* X Data */
    LSM6DSOX_ReadRegister(dev, OUTX_H_A, &regData[0]);
    LSM6DSOX_ReadRegister(dev, OUTX_L_A, &regData[1]);
    accelRaw[0] = (regData[0] << 8 ) | regData[1];

    /* Y Data */
    LSM6DSOX_ReadRegister(dev, OUTY_H_A, &regData[0]);
    LSM6DSOX_ReadRegister(dev, OUTY_L_A, &regData[1]);
    accelRaw[1] = (regData[0] << 8 ) | regData[1];

    /* Z Data */
    LSM6DSOX_ReadRegister(dev, OUTZ_H_A, &regData[0]);
    LSM6DSOX_ReadRegister(dev, OUTZ_L_A, &regData[1]);
    accelRaw[2] = (regData[0] << 8 ) | regData[1];

    int16_t accelRawSigned[3];
    /* Take the two's complement */
    accelRawSigned[0] = (int16_t) accelRaw[0];
    accelRawSigned[1] = (int16_t) accelRaw[1];
    accelRawSigned[2] = (int16_t) accelRaw[2];

    /* Conversion to m/s^2 based on Datasheet sensitivity  */
    float sensitivity = SENSITIVITY_2_G / 1000.0f;
    dev->accel_msp2[0] = GRAVITY_F * sensitivity * accelRawSigned[0];
    dev->accel_msp2[1] = GRAVITY_F * sensitivity * accelRawSigned[1];
    dev->accel_msp2[2] = GRAVITY_F * sensitivity * accelRawSigned[2];
}

void LSM6DSOX_ReadRegister(LSM6DSOX *dev, uint8_t reg, uint8_t *data) {
    uint8_t request[2];
    uint8_t response[2];
    createSPIReadReq(request, reg);

    gpio_put(dev->cs_pin, 0); // pull CS low
    /* 2*sizeof since we are sending two bytes (request[0], and request[1]) */
    spi_write_read_blocking(dev->spi_handle, request, response, REQUEST_SIZE*sizeof(uint8_t)); 
    gpio_put(dev->cs_pin, 1); // pull CS high

    *data = response[1]; 
}

void LSM6DSOX_WriteRegister(LSM6DSOX *dev, uint8_t reg, uint8_t *data) {
    uint8_t request[2];
    createSPIWriteReq(request, reg, *data);

    gpio_put(dev->cs_pin, 0); // pull CS low
    /* 2*sizeof since we are sending two bytes (request[0], and request[1]) */
    spi_write_blocking(dev->spi_handle, request, REQUEST_SIZE*sizeof(uint8_t)); 
        // printbuf(inbuf, 2);
    gpio_put(dev->cs_pin, 1); // pull CS high
}

static void spiCreateAndSendRequest(LSM6DSOX *dev, uint8_t reg, uint8_t *data, bool read) {
    if(read) LSM6DSOX_ReadRegister(dev, reg, data);
    else LSM6DSOX_WriteRegister(dev, reg, data);
}

static void createSPIWriteReq(uint8_t* request, uint8_t reg, uint8_t data) {
    request[0] = (0x00 | reg); //MSB = 0, register address (8-14)
    request[1] = data; // add Data to write (0-7)
    
    // printf("\nWRITE REQUEST:");
    // printSerialPacket(request[0], request[1]);
}

static void createSPIReadReq(uint8_t* request, uint8_t reg) {
    request[0] = 0x80 | reg; //MSB = 1, register address (8-14)
    request[1] = 0x00; 
    
    // printf("\nREAD REQUEST:");
    // printSerialPacket(request[0], request[1]);
}
