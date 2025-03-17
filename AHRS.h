#ifndef AHRS_H
#define AHRS_H

// Define UART0 pins and baudrate
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Define SPI0 pins
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

void gpio_irq_callback(uint gpio, uint32_t events);
void gpio_event_string(char *buf, uint32_t events);

#endif
