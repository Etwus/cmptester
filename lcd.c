// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

#define MOSI        PB3
#define SCK         PB5
#define CS          PB2
#define RS          PB1

void lcd_send_command(uint8_t command, uint16_t delayMicroSecs) {
    PORTB &= ~((1 << RS) | (1 << CS));
    SPDR = command;
    while (!(SPSR & (1 << SPIF))) {

    }
    PORTB |= 1 << CS;
    while((delayMicroSecs--) > 0) {
        _delay_us(1);
    }
}

void lcd_set_cursor_position(uint8_t xPos, uint8_t yPos) {
    if (yPos == 0) {
        lcd_send_command(0x80 | xPos, 27);
    } else if (yPos == 1) {
        lcd_send_command(0x80 | (0x40 + xPos), 27);
    }
}

void lcd_write_ram(uint8_t data) {
    PORTB &= ~(1 << CS);
    PORTB |= 1 << RS;
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {

    }
    PORTB |= 1 << CS;
    _delay_us(27);
}

void lcd_print_char(char character) {
    lcd_write_ram(character);
}

void lcd_print_string(char* message) {
    for (uint8_t i = 0; i < strlen(message); i++) {
        lcd_print_char(message[i]);
    }
}

void lcd_clear() {
    lcd_send_command(0x01, 1100);
}

void lcd_set_cgram_address(uint8_t address) {
    lcd_send_command(0x40 | address, 27);
}

void lcd_set_ddram_address(uint8_t address) {
    lcd_send_command(0x80 | address, 27);
}

void lcd_register_symbol(uint8_t id, const volatile uint8_t symbol[8]) {
    for(int i = 0; i < 8; i++) {
        lcd_set_cgram_address(i | (id << 3));
        lcd_write_ram(symbol[i]);
    }
}

void lcd_print_int(uint16_t number) {
    char buffer[12];
    sprintf(buffer, "%d", number);
    lcd_print_string(buffer);
}

void lcd_print_float(float number, uint8_t precision) {
    char buffer[12];
    dtostrf(number, 4-precision, precision, buffer);
    lcd_print_string(buffer);
}

void lcd_print_unit(float value, int8_t exponent, char unit,
        uint8_t precision) {
    if (value >= 1000000) {
        value /= 1000000;
        exponent += 6;
    } else if (value >= 1000) {
        value /= 1000;
        exponent += 3;
    } else if (value >= 1) {

    } else if (value >= 0.001) {
        value *= 1000;
        exponent -= 3;
    } else if (value >= 0.000001) {
        value *= 1000000;
        exponent -=6;
    }

    lcd_print_float(value, precision);
    switch(exponent) {
        case -12:
            lcd_print_char('p');
            break;
        case -9:
            lcd_print_char('n');
            break;
        case -6:
            lcd_print_char('u');
            break;
        case -3:
            lcd_print_char('m');
            break;
        case 0:
            break;
        case 3:
            lcd_print_char('k');
            break;
        case 6:
            lcd_print_char('M');
            break;
    }
    lcd_print_char(unit);
}

void lcd_init() {
    DDRB |= (1 << RS) | (1 << CS) | (1 << MOSI) | (1 << SCK);
    PORTB |= 1 << CS;
    SPCR = (1 << SPE) | (1<<MSTR) | (1 << SPR0);

    lcd_send_command(0x39, 27); // Function Set
    lcd_send_command(0x1C, 27); // Bias
    lcd_send_command(0x52, 27); // Power Control
    lcd_send_command(0x69, 27); // Follower Control
    lcd_send_command(0x74, 27); // Contrast
    lcd_send_command(0x38, 27); // Function Set
    lcd_send_command(0x0C, 27); // Display On
    lcd_send_command(0x01, 1100); // Clear
    lcd_send_command(0x06, 27); //Mode Set
}
