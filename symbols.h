// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef SYMBOLS_H
#define SYMBOLS_H

volatile const uint8_t SYMBOL_RESISTOR1[8] = {
    0b00000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    0b00000,
};

volatile const uint8_t SYMBOL_RESISTOR2[8] = {
    0b00000,
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11111,
    0b00000,
};

volatile const uint8_t SYMBOL_DIODE_RIGHT[8] = {
    0b10001,
    0b11001,
    0b11101,
    0b11111,
    0b11101,
    0b11001,
    0b10001,
    0b00000,
};

volatile const uint8_t SYMBOL_DIODE_LEFT[8] = {
    0b10001,
    0b10011,
    0b10111,
    0b11111,
    0b10111,
    0b10011,
    0b10001,
    0b00000,
};

volatile const uint8_t SYMBOL_ZENER_RIGHT[8] = {
    0b10111,
    0b11001,
    0b11101,
    0b11111,
    0b11101,
    0b11001,
    0b10001,
    0b00000,
};

volatile const uint8_t SYMBOL_ZENER_LEFT[8] = {
    0b10001,
    0b10011,
    0b10111,
    0b11111,
    0b10111,
    0b10011,
    0b11101,
    0b00000,
};

volatile const uint8_t SYMBOL_CAPACITOR[8] = {
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
};

#endif