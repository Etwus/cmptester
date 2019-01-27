// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADMUX_TP1           0
#define ADMUX_TP2           1
#define ADMUX_TP3           2
#define ADMUX_BATTERY       5
#define REFERENCE_1V1       3
#define REFERENCE_AVCC      1

uint16_t read_adc(uint8_t admux, uint8_t reference);

#endif