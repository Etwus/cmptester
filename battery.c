// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <stdint.h>

#include "battery.h"
#include "adc.h"
#include "iodefs.h"

float get_battery_voltage() {
    return (5.0f/1023.0f) * read_adc(ADMUX_BATTERY, REFERENCE_AVCC) * 2;
}