#ifndef SPI_LSM6DSOX_H
#define SPI_LSM6DSOX_H

#define PIN_LSM6DSOX_INT1 22

#define NO_DATA 0x00

// Define SPI0 pins
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// Define LSM6DSOX register addresses
#define STATUS_REG 0x1E
#define WHO_AM_I_REF 0x0f
#define XLDA 0x01
#define GDA 0x02

// INT1
#define INT1_CTRL 0x0D

// Accelerometer
#define CTRL1_XL 0x10 // Accelerometer control register
#define OUTX_L_A 0x28  // X Axis
#define OUTX_H_A 0x29
#define OUTY_L_A 0x2A  // Y Axis
#define OUTY_H_A 0x2B
#define OUTZ_L_A 0x2C  // Z Axis
#define OUTZ_H_A 0x2D

void setup_LSM6DSOX();
uint8_t spiCreateAndSendRequest(short reg, char data, bool read);
void read_Accel_Data(uint16_t* data);
bool check_data_ready(uint8_t data_rdy_bit);

static void createSPIWriteReq(uint8_t* request, short reg, char data);
static void createSPIReadReq(uint8_t* request, char reg);
static uint8_t spiSendRequest(uint8_t* request, int request_size);


#endif