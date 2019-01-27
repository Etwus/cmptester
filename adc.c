// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "iodefs.h"
#include "constants.h"

uint16_t read_adc(uint8_t admux, uint8_t reference) {
    ADMUX |= (reference << REFS0) | (admux << MUX0);
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1)
            | (1 << ADPS0);
    while (!(ADCSRA & (1 << ADIF))) {
    }
    ADMUX = 0;
    uint16_t result = ADCL + ((uint16_t)ADCH << 8);

    return result > ADC_MAX_TRESHOLD ? ADC_MAX_TRESHOLD : result;
}
