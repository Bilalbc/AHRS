#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "LSM6DSOX.h"
#include "AHRS.h"
#include "helper.h"

static char event_str[128];
LSM6DSOX acc;

void setup() {
    stdio_init_all();
    /* GPIO */
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_init(PIN_LSM6DSOX_INT1);

    /* SPI  */
    spi_init(SPI_PORT, 10000000); // 1 MHz sampling rate
    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST); // LSM6dSOX uses cpol = 1, cpha = 1
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1); // CS active high

    /* Check Clocks */
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    printf("peri Clock Frequency is %d Hz\n", clock_get_hz(clk_peri));

    /* UART */
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    
    // //configure interrupt handlers
    // irq_set_exclusive_handler(UART0_IRQ, uart0_irq_routine);
    // irq_set_exclusive_handler(IO_IRQ_BANK0, GPIO_irq_routine);
    // irq_set_enabled(UART0_IRQ, true);

    // // Enable Interrupts
    // gpio_set_irq_enabled(PIN_LSM6DSOX_INT1, GPIO_IRQ_LEVEL_HIGH, true);
    // uart_set_irqs_enabled(UART_ID, true, false); // enable interrupts on RX
    gpio_set_irq_enabled_with_callback(PIN_LSM6DSOX_INT1, GPIO_IRQ_EDGE_RISE, 
        true, &gpio_irq_callback);
}

int main() {
    setup();
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    LSM6DSOX_Initialize(&acc, SPI_PORT, PIN_CS);
    while (true) {
        sleep_ms(100);
        printf("Waiting...\n");
    }
}

void gpio_irq_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    gpio_event_string(event_str, events);
    printf("GPIO %d %s:\t ", gpio, event_str);
    switch (gpio) {
        case 22:
            uint16_t data[3];
            LSM6DSOX_ReadAccelerations(&acc);
            tilt_SingleAxis_Accel(&acc);
            printf("X: %f deg\t Y: %f deg\t Z: %f m/s^2\n", 
                    acc.tilt_Accel[X_AXIS_DATA], acc.tilt_Accel[Y_AXIS_DATA], acc.accel_mps2[Z_AXIS_DATA]);
            break;
            
        default: 
            break;
    }
   
}
