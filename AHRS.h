#ifndef AHRS_H
#define AHRS_H

// Define UART0 pins and baudrate
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void uart0_irq_routine(void);

#endif
