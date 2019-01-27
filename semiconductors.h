// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef SEMICONDUCTORS_H
#define SEMICONDUCTORS_H

#include <stdint.h>
#include "types.h"

uint8_t check_for_diodes(DiodeCheckResult *result);

uint8_t check_for_bjt(TransistorCheckResult *result,
        DiodeCheckResult diodeCheckResult);
uint8_t check_for_ujt(TransistorCheckResult *result,
        DiodeCheckResult diodeCheckResult);
uint8_t check_for_zener(ZenerCheckResult *result,
        DiodeCheckResult diodeCheckResult);

#endif