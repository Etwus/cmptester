// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stopwatch.h"

volatile uint16_t numberOfOverflows;

void stopwatch_start() {
    numberOfOverflows = 0;
    TCNT1H = 0;
    TCNT1L = 0;
    TIMSK1 |= (1 << TOIE1);
    TCCR1B = (1 << CS10);
}

uint32_t stopwatch_stop() {
    uint8_t low = TCNT1L;
    uint8_t high = TCNT1H;
    uint32_t numberOfClockCycles = (numberOfOverflows * 65535)
            + (low + ((uint16_t)high << 8));
    return numberOfClockCycles;
}

ISR(TIMER1_OVF_vect) {
    numberOfOverflows++;
}