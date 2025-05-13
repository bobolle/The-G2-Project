#include "pico/stdlib.h"
#include "uss.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

void uss_init(uint8_t trigger_pin, uint8_t echo_pin) {
    gpio_init(trigger_pin);
    gpio_init(echo_pin);

    gpio_set_dir(trigger_pin, GPIO_OUT);
    gpio_set_dir(echo_pin, GPIO_IN);

    gpio_put(trigger_pin, 0);
}

uint get_pulse_duration(uint8_t trigger_pin, uint8_t echo_pin, uint64_t timeout) {
    gpio_put(trigger_pin, 1);
    sleep_us(10);
    gpio_put(trigger_pin, 0);

    uint64_t time = 0;

    while (gpio_get(echo_pin) == 0) {
        tight_loop_contents();
    }

    // save time of beginning of sending pulse
    absolute_time_t start_time = get_absolute_time();

    while (gpio_get(echo_pin) == 1) {
        time++;
        sleep_us(1);

        if (time > timeout) {
            return 0;
        }
    }

    // save time of receiving pulse
    absolute_time_t end_time = get_absolute_time();

    return absolute_time_diff_us(start_time, end_time);
}

uint get_pulse_cm(uint8_t trigger_pin, uint8_t echo_pin, uint64_t timeout) {
    float temp = 20;
    // shamelessly stolen from freenove
    double reciprocalVelocity = 10000 / (331.45 + (0.62 * temp));

    uint duration = get_pulse_duration(trigger_pin, echo_pin, timeout);
    uint distance = ((duration / 2) / reciprocalVelocity);

    return distance;
}
