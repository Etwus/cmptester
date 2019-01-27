// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <stdint.h>
#include <util/delay.h>
#include "screens.h"
#include "lcd.h"
#include "constants.h"
#include "battery.h"

typedef enum {
    SCREEN_NONE, SCREEN_ZENER, SCREEN_RESISTOR, SCREEN_BJT,
    SCREEN_UJT, SCREEN_CAPACITOR, SCREEN_DIODE
} Screen;

uint8_t zenerScreenPage = 0;
Screen lastScreen = SCREEN_NONE;
ZenerCheckResult lastZenerResult;

void switch_screen_pages() {
    if (lastScreen == SCREEN_ZENER) {
        zenerScreenPage++;
        if (zenerScreenPage > 1) {
            zenerScreenPage = 0;
        }
        show_zener_result(lastZenerResult, zenerScreenPage);
    }
}

void reset_screen_pages() {
    zenerScreenPage = 0;
}

void print_resistor_symbol() {
    lcd_print_char(0b00101101);
    lcd_print_char(1);
    lcd_print_char(2);
    lcd_print_char(0b00101101);
}

void print_diode_left_symbol() {
    lcd_print_char(0b00101101);
    lcd_print_char(3);
    lcd_print_char(0b00101101);
}

void print_diode_right_symbol() {
    lcd_print_char(0b00101101);
    lcd_print_char(4);
    lcd_print_char(0b00101101);
}

void print_zener_left_symbol() {
    lcd_print_char(0b00101101);
    lcd_print_char(CHARACTER_ZENER_LEFT);
    lcd_print_char(0b00101101);
}

void print_zener_right_symbol() {
    lcd_print_char(0b00101101);
    lcd_print_char(CHARACTER_ZENER_RIGHT);
    lcd_print_char(0b00101101);
}

void print_capacitor_symbol() {
    lcd_print_char(0b00101101);
    lcd_print_char(CHARACTER_CAPACITOR);
    lcd_print_char(0b00101101);
}

void print_single_diode(uint8_t firstTP, uint8_t diodePolarity,
                        uint8_t secondTP) {
    lcd_print_int(firstTP);
    if(diodePolarity == DIODE_POLARITY_AK) {
        print_diode_right_symbol();
    } else {
        print_diode_left_symbol();
    }
    lcd_print_int(secondTP);
}

void print_diodes(uint8_t firstTP, uint8_t firstDiodePolarity, uint8_t secondTP,
                  uint8_t secondDiodePolarity, uint8_t thirdTP) {
    lcd_print_int(firstTP);
    if(firstDiodePolarity == DIODE_POLARITY_AK) {
        print_diode_right_symbol();
    } else {
        print_diode_left_symbol();
    }
    lcd_print_int(secondTP);
    if(secondDiodePolarity == DIODE_POLARITY_AK) {
        print_diode_right_symbol();
    } else {
        print_diode_left_symbol();
    }
    lcd_print_int(thirdTP);
}

void print_single_zener(uint8_t firstTP, uint8_t diodePolarity,
                        uint8_t secondTP) {
    lcd_print_int(firstTP);
    if(diodePolarity == DIODE_POLARITY_AK) {
        print_zener_right_symbol();
    } else {
        print_zener_left_symbol();
    }
    lcd_print_int(secondTP);
}

void print_multiple_zeners(uint8_t firstTP, uint8_t firstDiodePolarity,
                           uint8_t secondTP,
                  uint8_t secondDiodePolarity, uint8_t thirdTP) {
    lcd_print_int(firstTP);
    if(firstDiodePolarity == DIODE_POLARITY_AK) {
        print_zener_right_symbol();
    } else {
        print_zener_left_symbol();
    }
    lcd_print_int(secondTP);
    if(secondDiodePolarity == DIODE_POLARITY_AK) {
        print_zener_right_symbol();
    } else {
        print_zener_left_symbol();
    }
    lcd_print_int(thirdTP);
}

void print_single_capacitor(uint8_t firstTP, uint8_t secondTP) {
    lcd_print_int(firstTP);
    print_capacitor_symbol();
    lcd_print_int(secondTP);
}

void print_multiple_capacitors(uint8_t firstTP, uint8_t secondTP,
                               uint8_t thirdTP) {
    lcd_print_int(firstTP);
    print_capacitor_symbol();
    lcd_print_int(secondTP);
    print_capacitor_symbol();
    lcd_print_int(thirdTP);
}

void show_main_screen() {
    lcd_clear();
    lcd_set_cursor_position(0, 0);
    lcd_print_string("Tester soucastek");
    lcd_set_cursor_position(0, 1);
    lcd_print_string("Baterie: ");
    lcd_print_unit(get_battery_voltage(), 0, 'V', 2);
    lastScreen = SCREEN_NONE;
}

void show_component_not_found() {
    lcd_clear();
    lcd_set_cursor_position(0, 0);
    lcd_print_string("Komponent");
    lcd_set_cursor_position(0, 1);
    lcd_print_string("nebyl nalezen!");
    lastScreen = SCREEN_NONE;
}

void show_resistor_result(ResistorCheckResult result) {
    lcd_clear();
    lcd_set_cursor_position(0, 0);
    if (result.numberOfResistors == 1) {
        lcd_print_int(result.resistor[0].firstLead);
        print_resistor_symbol();
        lcd_print_int(result.resistor[0].secondLead);
        lcd_set_cursor_position(0, 1);
        lcd_print_unit(result.resistor[0].resistance, 0, CHARACTER_OHM, 1);
    } else if (result.numberOfResistors == 2) {
        lcd_print_char('1');
        print_resistor_symbol();
        lcd_print_char('2');
        print_resistor_symbol();
        lcd_print_char('3');
        if(result.resistor[0].firstLead == 1) {
            lcd_set_cursor_position(0, 1);
            lcd_print_unit(result.resistor[0].resistance, 0, CHARACTER_OHM, 1);
            lcd_print_char(' ');
            lcd_print_unit(result.resistor[1].resistance, 0, CHARACTER_OHM, 1);
        } else {
            lcd_set_cursor_position(0, 1);
            lcd_print_unit(result.resistor[1].resistance, 0, CHARACTER_OHM, 1);
            lcd_print_char(' ');
            lcd_print_unit(result.resistor[0].resistance, 0, CHARACTER_OHM, 1);
        }
    }
    lastScreen = SCREEN_RESISTOR;
}

void show_diode_result(DiodeCheckResult result) {
    lcd_clear();
    lcd_set_cursor_position(0, 0);

    if(result.numberOfDiodes == 1) {
        switch(result.diode[0].anode) {
            case 1:
                if (result.diode[0].cathode == 2) {
                    print_single_diode(TP1, DIODE_POLARITY_AK, TP2);
                } else {
                    print_single_diode(TP1, DIODE_POLARITY_AK, TP3);
                }
                break;
            case 2:
                if (result.diode[0].cathode == 1) {
                    print_single_diode(TP1, DIODE_POLARITY_KA, TP2);
                } else {
                    print_single_diode(TP2, DIODE_POLARITY_AK, TP3);
                }
                break;
            case 3:
                if (result.diode[0].cathode == 1) {
                    print_single_diode(TP1, DIODE_POLARITY_KA, TP3);
                } else {
                    print_single_diode(TP2, DIODE_POLARITY_KA, TP3);
                }
                break;
        }
        lcd_set_cursor_position(0, 1);
        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 3);
    } else if(result.numberOfDiodes == 2) {
        if(result.diode[0].anode == result.diode[1].anode) {
            if (result.diode[0].anode == 1) {
                print_diodes(TP2, DIODE_POLARITY_KA, TP1,
                             DIODE_POLARITY_AK, TP3);
                if (result.diode[0].cathode == 2) {
                    lcd_set_cursor_position(0, 1);
                    lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    lcd_print_char(' ');
                    lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                } else {
                    lcd_set_cursor_position(0, 1);
                    lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    lcd_print_char(' ');
                    lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                }
            } else if(result.diode[0].anode == 2) {
                print_diodes(TP1, DIODE_POLARITY_KA, TP2,
                             DIODE_POLARITY_AK, TP3);
                if (result.diode[0].cathode == 1) {
                    lcd_set_cursor_position(0, 1);
                    lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    lcd_print_char(' ');
                    lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                } else {
                    lcd_set_cursor_position(0, 1);
                    lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    lcd_print_char(' ');
                    lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                }
            } else {
                print_diodes(TP1, DIODE_POLARITY_KA, TP3,
                             DIODE_POLARITY_AK, TP2);
                if (result.diode[0].cathode == 1) {
                    lcd_set_cursor_position(0, 1);
                    lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    lcd_print_char(' ');
                    lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                } else {
                    lcd_set_cursor_position(0, 1);
                    lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    lcd_print_char(' ');
                    lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                }
            }

        } else if(result.diode[0].cathode == result.diode[1].anode) {
            switch (result.diode[0].cathode) {
                case 1:
                    print_diodes(TP2, DIODE_POLARITY_AK, TP1,
                             DIODE_POLARITY_AK, TP3);
                    if (result.diode[0].anode == 2) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
                case 2:
                    print_diodes(TP1, DIODE_POLARITY_AK, TP2,
                             DIODE_POLARITY_AK, TP3);
                    if (result.diode[0].anode == 1) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
                case 3:
                    print_diodes(TP1, DIODE_POLARITY_AK, TP3,
                             DIODE_POLARITY_AK, TP2);
                    if (result.diode[0].anode == 1) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
            }
        } else if (result.diode[0].anode == result.diode[1].cathode) {
            switch (result.diode[0].anode) {
                case 1:
                    print_diodes(TP2, DIODE_POLARITY_KA, TP1,
                             DIODE_POLARITY_KA, TP3);
                    if (result.diode[0].cathode == 2) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
                case 2:
                    print_diodes(TP1, DIODE_POLARITY_KA, TP2,
                             DIODE_POLARITY_KA, TP3);
                    if (result.diode[0].cathode == 1) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
                case 3:
                    print_diodes(TP1, DIODE_POLARITY_KA, TP3,
                             DIODE_POLARITY_KA, TP2);
                    if (result.diode[0].cathode == 1) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
            }
        } else if(result.diode[0].cathode == result.diode[1].cathode) {
            switch (result.diode[0].cathode) {
                case 1:
                    print_diodes(TP2, DIODE_POLARITY_AK, TP1,
                             DIODE_POLARITY_KA, TP3);
                    if (result.diode[0].anode == 2) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
                case 2:
                    print_diodes(TP1, DIODE_POLARITY_AK, TP2,
                             DIODE_POLARITY_KA, TP3);
                    if (result.diode[0].anode == 1) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
                case 3:
                    print_diodes(TP1, DIODE_POLARITY_AK, TP3,
                             DIODE_POLARITY_KA, TP2);
                    if (result.diode[0].anode == 1) {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                    } else {
                        lcd_set_cursor_position(0, 1);
                        lcd_print_unit(result.diode[1].voltageDrop, 0, 'V', 1);
                        lcd_print_char(' ');
                        lcd_print_unit(result.diode[0].voltageDrop, 0, 'V', 1);
                    }
                    break;
            }
        }
    }
    lastScreen = SCREEN_DIODE;
}

void show_bjt_result(TransistorCheckResult result) {
    lcd_clear();
    lcd_set_cursor_position(0, 0);
    if (result.type == NPN) {
        lcd_print_string("BJT-NPN 123=");
    } else {
        lcd_print_string("BJT-PNP 123=");
    }

    char buffer[3];
    buffer[result.base - 1] = 'B';
    buffer[result.emitter - 1] = 'E';
    buffer[result.collector - 1] = 'C';
    for(int i = 0; i < 3; i++) {
        lcd_print_char(buffer[i]);
    }
    lcd_set_cursor_position(0, 1);
    lcd_print_string("Hfe ");
    lcd_print_float(result.hfe, 1);
    lastScreen = SCREEN_BJT;
}

void show_ujt_result(TransistorCheckResult result) {
    lcd_clear();
    lcd_set_cursor_position(0, 0);
    if (result.type == PMOS) {
        lcd_print_string("UJT-PMOS 123=");
    } else {
        lcd_print_string("UJT-NMOS 123=");
    }
    char buffer[3];
    buffer[result.gate - 1] = 'G';
    buffer[result.drain - 1] = 'D';
    buffer[result.source - 1] = 'S';
    for(int i = 0; i < 3; i++) {
        lcd_print_char(buffer[i]);
    }
    lastScreen = SCREEN_UJT;
}

void show_zener_result(ZenerCheckResult result, uint8_t page) {
    lcd_clear();
    lcd_set_cursor_position(0, 0);

    if (result.numberOfDiodes == 1) {
        uint8_t anode = result.zener[0].anode;
        uint8_t cathode = result.zener[0].cathode;
        if(cathode > anode) {
            print_single_zener(anode, DIODE_POLARITY_AK, cathode);
        } else {
            print_single_zener(cathode, DIODE_POLARITY_KA, anode);
        }

        if (page == 0) {
            lcd_set_cursor_position(14, 0);
            lcd_print_string("Uz");
            lcd_set_cursor_position(0, 1);
            lcd_print_unit(result.zener[0].zenerVoltage, 0, 'V', 3);

        } else {
            lcd_set_cursor_position(14, 0);
            lcd_print_string("Uf");
            lcd_set_cursor_position(0, 1);
            lcd_print_unit(result.zener[0].forwardVoltage, 0, 'V', 1);
        }

    } else {
        uint8_t anode1 = result.zener[0].anode;
        uint8_t cathode1 = result.zener[0].cathode;
        uint8_t anode2 = result.zener[1].anode;
        uint8_t cathode2 = result.zener[1].cathode;

        if (anode1 == anode2) {
            switch(anode1) {
                case 1:
                    print_multiple_zeners(2, DIODE_POLARITY_KA, 1,
                                          DIODE_POLARITY_AK, 3);
                    break;
                case 2:
                    print_multiple_zeners(1, DIODE_POLARITY_KA, 2,
                                          DIODE_POLARITY_AK, 3);
                    break;
                case 3:
                    print_multiple_zeners(1, DIODE_POLARITY_KA, 3,
                                          DIODE_POLARITY_AK, 2);
                    break;
            }
        } else if (cathode1 == cathode2) {
            switch(cathode1) {
                case 1:
                    print_multiple_zeners(2, DIODE_POLARITY_AK, 1,
                                          DIODE_POLARITY_KA, 3);
                    break;
                case 2:
                    print_multiple_zeners(1, DIODE_POLARITY_AK, 2,
                                          DIODE_POLARITY_KA, 3);
                    break;
                case 3:
                    print_multiple_zeners(1, DIODE_POLARITY_AK, 3,
                                          DIODE_POLARITY_KA, 2);
                    break;
            }
        } else if (cathode1 == anode2 || cathode2 == anode1) {
            print_multiple_zeners(1, DIODE_POLARITY_AK, 2,
                                  DIODE_POLARITY_AK, 3);
        } else {
            print_multiple_zeners(1, DIODE_POLARITY_KA, 2,
                                  DIODE_POLARITY_KA, 3);
        }
        if (page == 0) {
            lcd_set_cursor_position(14, 0);
            lcd_print_string("Uz");
            if ((anode1 + cathode1) < (anode2 + cathode2)) {
                lcd_set_cursor_position(0, 1);
                lcd_print_unit(result.zener[0].zenerVoltage, 0, 'V', 3);
                lcd_print_char(' ');
                lcd_print_unit(result.zener[1].zenerVoltage, 0, 'V', 3);
            } else {
                lcd_set_cursor_position(0, 1);
                lcd_print_unit(result.zener[1].zenerVoltage, 0, 'V', 3);
                lcd_print_char(' ');
                lcd_print_unit(result.zener[0].zenerVoltage, 0, 'V', 3);
            }
        } else {
            lcd_set_cursor_position(14, 0);
            lcd_print_string("Uf");
            if ((anode1 + cathode1) < (anode2 + cathode2)) {
                lcd_set_cursor_position(0, 1);
                lcd_print_unit(result.zener[0].forwardVoltage, 0, 'V', 1);
                lcd_print_char(' ');
                lcd_print_unit(result.zener[1].forwardVoltage, 0, 'V', 1);
            } else {
                lcd_set_cursor_position(0, 1);
                lcd_print_unit(result.zener[1].forwardVoltage, 0, 'V', 1);
                lcd_print_char(' ');
                lcd_print_unit(result.zener[0].forwardVoltage, 0, 'V', 1);
            }
        }
    }

    lastScreen = SCREEN_ZENER;
    lastZenerResult = result;
}

void show_capacitor_result(CapacitorCheckResult result) {
    lcd_clear();
    lcd_set_cursor_position(0, 0);

    if (result.numberOfCapacitors > 1) {
        uint8_t common = 0;
        if(result.capacitor[0].firstLead == result.capacitor[1].firstLead) {
            common = result.capacitor[0].firstLead;
        } else if(result.capacitor[0].secondLead
            == result.capacitor[1].firstLead) {
            common = result.capacitor[0].secondLead;
        } else if(result.capacitor[0].secondLead
            == result.capacitor[1].secondLead) {
            common = result.capacitor[0].secondLead;
        } else if (result.capacitor[0].firstLead
            == result.capacitor[1].secondLead) {
            common = result.capacitor[0].firstLead;
        }

        switch (common) {
            case 1:
                print_multiple_capacitors(2, 1, 3);
                break;
            case 2:
                print_multiple_capacitors(1, 2, 3);
                break;
            case 3:
                print_multiple_capacitors(1, 3, 2);
                break;
        }

        uint8_t cap0TpSum = result.capacitor[0].firstLead
                + result.capacitor[0].secondLead;
        uint8_t cap1TpSum = result.capacitor[1].firstLead
                + result.capacitor[1].secondLead;


        lcd_set_cursor_position(0, 1);
        if (cap0TpSum < cap1TpSum) {
                lcd_print_unit(result.capacitor[0].capacityInMicroF, -6,
                               'F', 2);
                lcd_print_char(' ');
                lcd_print_unit(result.capacitor[1].capacityInMicroF, -6,
                               'F', 2);
            } else {
                lcd_print_unit(result.capacitor[1].capacityInMicroF, -6,
                               'F', 2);
                lcd_print_char(' ');
                lcd_print_unit(result.capacitor[0].capacityInMicroF, -6,
                               'F', 2);
            }
    } else {
        print_single_capacitor(result.capacitor[0].firstLead,
                               result.capacitor[0].secondLead);
        lcd_set_cursor_position(0, 1);
        lcd_print_unit(result.capacitor[0].capacityInMicroF, -6, 'F', 2);
    }

    lastScreen = SCREEN_CAPACITOR;
}

void show_capacitor_charging() {
    lcd_clear();
    lcd_set_cursor_position(0, 0);
    lcd_print_string("Nabijeni");
    lcd_set_cursor_position(0, 1);
    lcd_print_string("kondensatoru...");
}