// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef SCREENS_H
#define SCREENS_H

#include <stdint.h>
#include "types.h"

void show_main_screen();
void show_resistor_result(ResistorCheckResult result);
void show_diode_result(DiodeCheckResult result);
void show_bjt_result(TransistorCheckResult result);
void show_ujt_result(TransistorCheckResult result);
void show_zener_result(ZenerCheckResult result, uint8_t page);
void show_capacitor_result(CapacitorCheckResult result);
void show_component_not_found();
void show_capacitor_charging();
void switch_screen_pages();
void reset_screen_pages();
#endif