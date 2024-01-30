#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Pin Definitions
const uint pumpPin = 22;     // Pin connected to the relay control pin
const uint sensorPin = 14;   // Pin connected to the floating sensor

// Time Constants (in microseconds)
// const uint32_t pumpInterval = 2 * 24 * 60 * 60 * 1000000; // 2 days
const uint32_t pumpInterval = 2000000; // 2 days
const uint32_t debounceDelay = 100000; // 100 ms

// Variables
bool tankFull = false;

void onTimerInterrupt(uint gpio, uint32_t events) {
  // Read the sensor state
  bool sensorState = gpio_get(!sensorPin);

  printf("Sensor state: %s\n", sensorState ? "ON" : "OFF");
  
  // Check if the tank is full
  if (sensorState == 1) {
    tankFull = true;
  } else {
    tankFull = false;
  }

  printf("tankFull state: %s\n", tankFull ? "ON" : "OFF");
  
  // Check if it's time to turn on the pump
  static uint32_t previousTime = 0;
  uint32_t currentTime = to_us_since_boot(get_absolute_time());
  printf("currentTime: %s\n", currentTime);
  printf("previousTime: %s\n", previousTime);
  printf("pumpInterval: %s\n", currentTime);

  
  if (!tankFull && (currentTime - previousTime >= pumpInterval)) {
    previousTime = currentTime;
    
    // Turn on the pump
    gpio_put(pumpPin, 0);
    sleep_ms(1000); // Optional delay for relay activation (adjust if necessary)
    gpio_put(pumpPin, 1);
  }
}

int main() {
  // Initialize GPIO and set pins as inputs/outputs
  stdio_init_all();
  printf("Initializing...");
  gpio_init(pumpPin);
  gpio_set_dir(pumpPin, GPIO_OUT);
  gpio_init(sensorPin);
  gpio_set_dir(sensorPin, GPIO_IN);
  gpio_pull_up(sensorPin);
  
  // Enable interrupts on sensor pin
  gpio_set_irq_enabled_with_callback(sensorPin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &onTimerInterrupt);
  
  // Start with the pump off
  gpio_put(pumpPin, 1);
  
  // Main loop
  while (1) {
    tight_loop_contents();
  }
  
  return 0;
}
