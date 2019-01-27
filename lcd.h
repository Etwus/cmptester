// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init();

void lcd_set_cursor_position(uint8_t xPos, uint8_t yPos);

void lcd_print_char(char character);

void lcd_print_string(char* message);

void lcd_print_int(uint16_t number);

void lcd_print_float(float number, uint8_t precision);

void lcd_print_unit(float value, int8_t exponent, char unit, uint8_t precision);

void lcd_clear();

void lcd_register_symbol(uint8_t id, const volatile uint8_t symbol[8]);

#endif