#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


int main() {
    const uint SWITCH_PIN = 14;    // GPIO for water level input
    const uint RELAY_PIN = 22;         // GPIO for relay control
    
    stdio_init_all(); // Initialize UART for debugging (optional)
    printf("Initializing...");

    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);

    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);

    bool led_state = false;
    bool cw_state = false;

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    
    while (true) {

        // Read the button state
        bool button_state = gpio_get(BUTTON_PIN);
        printf("Button state: %s\n", button_state ? "ON" : "OFF");

        // Button is pressed
        led_state = button_state;
        // Control the LED based on the water level switch
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
        // Control the relay based the water level switch
        gpio_put(RELAY_PIN, led_state);


        sleep_ms(1000);
    }

    return 0;
}   