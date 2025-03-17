#ifndef LSM6DSOX_H
#define LSM6DSOX_H

#define REQUEST_SIZE 2
/* Sensitivity value taken from LSM6DSOX datasheet */
#define SENSITIVITY_2_G 0.061f
#define GRAVITY_F 9.81f

#define PIN_LSM6DSOX_INT1 22
#define LSM6DSOX_WHO_AM_I 0x6C
#define NO_DATA 0x00

// Define LSM6DSOX register addresses
#define STATUS_REG 0x1E
#define WHO_AM_I_REG 0x0f
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

/* struct used to decouple the implementation of LSM6DSOX from spi port */
typedef struct {
    /* SPI Instance */
    spi_inst_t *spi_handle;
    /* CS Pin*/
    uint cs_pin;
    /* Acceleration Data (X, Y, Z) converted to m/s^2 */
    float accel_msp2[3];
    /* Gyroscope Data (X, Y, Z) converted to dps */
    float gyro_dps[3];
} LSM6DSOX;

void LSM6DSOX_Initialize(LSM6DSOX *dev, spi_inst_t *spi, uint CS_PIN);
void LSM6DSOX_ReadAccelerations(LSM6DSOX *dev);
static void spiCreateAndSendRequest(LSM6DSOX *dev, uint8_t reg, uint8_t *data, bool read);

static void createSPIWriteReq(uint8_t* request, uint8_t reg, uint8_t data);
static void createSPIReadReq(uint8_t* request, uint8_t reg);

void LSM6DSOX_ReadRegister(LSM6DSOX *dev, uint8_t reg, uint8_t *data);
void LSM6DSOX_WriteRegister(LSM6DSOX *dev, uint8_t reg, uint8_t *data);



#endif