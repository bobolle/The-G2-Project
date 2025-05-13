#ifndef __ULTRASONICSENSOR_LIBRARY_H
#define __ULTRASONICSENSOR_LIBRARY_H

void uss_init(uint8_t trigger_pin, uint8_t echo_pin);

// send and get duration of a pulse
// going out of range of timeout will return 0
uint get_pulse_duration(uint8_t trigger_pin, uint8_t echo_pin, uint64_t timeout);

uint get_pulse_cm(uint8_t trigger_pin, uint8_t echo_pin, uint64_t timeout);

#endif
