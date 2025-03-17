#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "SPI_LSM6DSOX.h"
#include "AHRS.h"
#include "helper.h"

void setup() {
    stdio_init_all();

    // GPIO
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    
    // SPI 
    spi_init(SPI_PORT, 1000*1000); // 1 MHz sampling rate
    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST); // LSM6dSOX uses cpol = 1, cpha = 1
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1); // CS active high

    // Check Clocks
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    printf("peri Clock Frequency is %d Hz\n", clock_get_hz(clk_peri));

    // UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    
    // //configure interrupt handlers
    // irq_set_exclusive_handler(UART0_IRQ, uart0_irq_routine);
    // irq_set_enabled(UART0_IRQ, true);

    // // Enable Interrupts
    // uart_set_irqs_enabled(UART_ID, true, false); // enable interrupts on RX
}

int main()
{
    setup();
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    uart_puts(UART_ID, " Hello, UART!\n");
    char* buf;

    printf("Hello, world!\n");
    setup_LSM6DSOX();
    while (true) {
        printf("X DATA: %04x\n", read_Accel_x());
    }
}

void uart0_irq_routine() {
    printf("Interrupt\n");

}
