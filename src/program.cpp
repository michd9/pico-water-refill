#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"


// Constants
const uint8_t RELAY_PIN = 22;         // GPIO pin connected to the relay
const uint8_t FLOAT_SWITCH_PIN = 14;  // GPIO pin connected to the float switch

// State machine
enum State {
    INIT,
    NOT_FULL,
    FULL,
    FAILURE
};

// Function prototypes
void openTap();
void closeTap();
bool isTankFull();
void enterState(State newState);
void updateStateMachine();
void failLoop();

// State machine variables
State currentState = INIT;
absolute_time_t failureStartTime;

// Configuration
const uint32_t FAILURE_TIMEOUT = 120000;     // Timeout for transitioning to the failure state [ms]

int main() {
    stdio_init_all();
    sleep_ms(1000);
    printf("Initializing...\n");

    // Initialize GPIO pins
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    gpio_init(FLOAT_SWITCH_PIN);
    gpio_set_dir(FLOAT_SWITCH_PIN, GPIO_IN);
    gpio_pull_up(FLOAT_SWITCH_PIN);

    closeTap();  // Start with the tap closed

    while (true) {
        // printf("Looping...\n");
        updateStateMachine();
        sleep_ms(100);
    }

    return 0;
}

void openTap() {
    gpio_put(RELAY_PIN, 0);  // Activate the relay to open the tap
    printf("Tap opened\n");
}

void closeTap() {
    gpio_put(RELAY_PIN, 1);  // Deactivate the relay to close the tap
    printf("Tap closed\n");
}

bool isTankFull() {
    // Read the sensor state
    bool switch_state = gpio_get(FLOAT_SWITCH_PIN); // Read the float switch state (0 = full, 1 = not full)
    // Debug prints:
    // printf("Level state: %d\n", switch_state);
    // printf("Level state: %s\n", switch_state) ? "FULL" : "NOTFULL");
    return !switch_state;
}

void enterState(State newState) {
    currentState = newState;
    switch (newState) {
        case INIT:
            printf("Entered state: INIT\n");
            break;
        case NOT_FULL:
            printf("Entered state: NOT_FULL\n");
            failureStartTime = get_absolute_time();
            openTap();
            break;
        case FULL:
            printf("Entered state: FULL\n");
            closeTap();
            break;
        case FAILURE:
            printf("Entered state: FAILURE\n");
            closeTap();
            failLoop();
            break;
    }
}

void updateStateMachine() {
    switch (currentState) {
        case INIT:
            if (isTankFull()) {
                enterState(FULL);
            } else {
                enterState(NOT_FULL);
            }
            break;
        case NOT_FULL:
            if (isTankFull()) {
                enterState(FULL);
            } else if (!isTankFull() && absolute_time_diff_us(failureStartTime, get_absolute_time()) >= FAILURE_TIMEOUT * 1000) {
                enterState(FAILURE);
            }
            break;
        case FULL:
            if (!isTankFull()) {
                enterState(NOT_FULL);
            }
            break;
        case FAILURE:
            if (isTankFull()) {
                enterState(FULL);
            }
            break;
    }
}

void failLoop() {
    // Infinite loop in case of failure
    while (1){
        printf("FAILURE state\n");
        sleep_ms(5000);
    }
}