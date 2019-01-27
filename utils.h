// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define TPSTATE_HI_HIGH 1
#define TPSTATE_HI_LOW  2
#define TPSTATE_LI_HIGH 3
#define TPSTATE_LI_LOW  4
#define TPSTATE_ZI_LOW  5

void discharge_tps();

void reset_tps();

void set_tp(uint8_t tp, uint8_t tpState);

uint16_t read_adc_on_tp(uint8_t tp, uint8_t reference);


#endif