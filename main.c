// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "lcd.h"
#include "resistors.h"
#include "iodefs.h"
#include "battery.h"
#include "semiconductors.h"
#include "constants.h"
#include "stopwatch.h"
#include "capacitors.h"
#include "screens.h"
#include "symbols.h"

uint16_t counterToAutoPowerOff = 0;

void init_buttons() {
    TEST_DDR &= ~(1 << TEST);
    TEST_PORT |= (1 << TEST);
    CONT_DDR &= ~(1 << CONT);
    CONT_PORT |= (1 << CONT);
}

void register_character_patterns() {
    lcd_register_symbol(CHARACTER_RESISTOR1, SYMBOL_RESISTOR1);
    lcd_register_symbol(CHARACTER_RESISTOR2, SYMBOL_RESISTOR2);
    lcd_register_symbol(CHARACTER_DIODE_LEFT, SYMBOL_DIODE_LEFT);
    lcd_register_symbol(CHARACTER_DIODE_RIGHT, SYMBOL_DIODE_RIGHT);
    lcd_register_symbol(CHARACTER_ZENER_LEFT, SYMBOL_ZENER_LEFT);
    lcd_register_symbol(CHARACTER_ZENER_RIGHT, SYMBOL_ZENER_RIGHT);
    lcd_register_symbol(CHARACTER_CAPACITOR, SYMBOL_CAPACITOR);
}

void init() {
    LATCH_DDR |= (1 << LATCH);
    LATCH_PORT |= (1 << LATCH);
    init_buttons();
    _delay_ms(500);
    lcd_init();
    register_character_patterns();
    sei();
}

void start_testing() {
    ResistorCheckResult resistorCheckResult;
    DiodeCheckResult diodeCheckResult;
    CapacitorCheckResult capacitorCheckResult;
    TransistorCheckResult transistorCheckResult;
    ZenerCheckResult zenerCheckResult;

    uint8_t resistorFound = 0;
    uint8_t capacitorFound = 0;
    uint8_t diodeFound = 0;
    uint8_t ujtFound = 0;
    uint8_t bjtFound = 0;
    uint8_t zenerFound = 0;

    if (check_for_capacitors(&capacitorCheckResult)) {
        capacitorFound = 1;
    }

    if (check_for_resistors(&resistorCheckResult)) {
        resistorFound = 1;
    }

    if (check_for_diodes(&diodeCheckResult)) {
        diodeFound = 1;
        if (check_for_ujt(&transistorCheckResult, diodeCheckResult)) {
            ujtFound = 1;
        }

        if (check_for_zener(&zenerCheckResult, diodeCheckResult)) {
            zenerFound = 1;
        }

        if(diodeCheckResult.numberOfDiodes > 1) {
            if(check_for_bjt(&transistorCheckResult, diodeCheckResult)) {
                bjtFound = 1;
            }
        }
    }

    if (ujtFound) {
        show_ujt_result(transistorCheckResult);
    } else if (zenerFound && !capacitorFound) {
        reset_screen_pages();
        show_zener_result(zenerCheckResult, 0);
    } else if (bjtFound) {
        show_bjt_result(transistorCheckResult);
    } else if (capacitorFound) {
        show_capacitor_result(capacitorCheckResult);
    } else if (diodeFound) {
        show_diode_result(diodeCheckResult);
    } else if (resistorFound) {
        show_resistor_result(resistorCheckResult);
    } else {
        show_component_not_found();
    }
}

void enable_watchdog() {
    WDTCSR = (1 << WDIE) | (0b10 << WDP0);
}

void disable_watchdog() {
    WDTCSR = 0;
}


int main() {
    init();
    show_main_screen();

    enable_watchdog();

    while(1) {
        SMCR = (1 << SM1);
        sleep_mode();
    }
}

ISR(WDT_vect) {
    disable_watchdog();
    sei();
    if (!(SWSIG_PIN & (1 << SWSIG))) {
        _delay_ms(1000);
        LATCH_PORT &= ~(1 << LATCH);
    }

    if (!(TEST_PIN & (1 << TEST))) {
        counterToAutoPowerOff = 0;
        start_testing();
        _delay_ms(300);
    }

    if(!(CONT_PIN & (1 << CONT))) {
        counterToAutoPowerOff = 0;
        switch_screen_pages();
        _delay_ms(300);
    }
    counterToAutoPowerOff++;

    if (counterToAutoPowerOff > 5100) {
        LATCH_PORT &= ~(1 << LATCH);
    }

    enable_watchdog();
}
