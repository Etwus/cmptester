// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"
#include "iodefs.h"
#include "adc.h"

void discharge_tps() {
    TP_PORT &= ~((1 << TP1_LOW) | (1 << TP1_HIGH) | (1 << TP2_LOW)
            | (1 << TP2_HIGH) | (1 << TP3_LOW) | (1 << TP3_HIGH));
    TP_DDR |= ((1 << TP1_LOW) | (1 << TP1_HIGH) | (1 << TP2_LOW)
            | (1 << TP2_HIGH) | (1 << TP3_LOW) | (1 << TP3_HIGH));
}

void reset_tps() {
    TP_PORT &= ~((1 << TP1_LOW) | (1 << TP1_HIGH) | (1 << TP2_LOW)
            | (1 << TP2_HIGH) | (1 << TP3_LOW) | (1 << TP3_HIGH));
    TP_DDR &= ~((1 << TP1_LOW) | (1 << TP1_HIGH) | (1 << TP2_LOW)
            | (1 << TP2_HIGH) | (1 << TP3_LOW) | (1 << TP3_HIGH));
    ADC_PORT &= ~((1 << ADC_TP1) | (1 << ADC_TP2) | (1 << ADC_TP3));
    ADC_DDR &= ~((1 << ADC_TP1) | (1 << ADC_TP2) | (1 << ADC_TP3));

}

void set_tp(uint8_t tp, uint8_t tpState) {
    switch(tp) {
        case 1:
            switch(tpState) {
                case TPSTATE_HI_HIGH:
                    TP_DDR |= (1 << TP1_HIGH);
                    TP_PORT |= (1 << TP1_HIGH);
                    break;
                case TPSTATE_HI_LOW:
                    TP_DDR |= (1 << TP1_HIGH);
                    TP_PORT &= ~(1 << TP1_HIGH);
                    break;
                case TPSTATE_LI_HIGH:
                    TP_DDR |= (1 << TP1_LOW);
                    TP_PORT |= (1 << TP1_LOW);
                    break;
                case TPSTATE_LI_LOW:
                    TP_DDR |= (1 << TP1_LOW);
                    TP_PORT &= ~(1 << TP1_LOW);
                    break;
                case TPSTATE_ZI_LOW:
                    ADC_DDR |= (1 << ADC_TP1);
                    ADC_PORT &= ~(1 << ADC_TP1);
                    break;
            }
            break;
        case 2:
            switch(tpState) {
                case TPSTATE_HI_HIGH:
                    TP_DDR |= (1 << TP2_HIGH);
                    TP_PORT |= (1 << TP2_HIGH);
                    break;
                case TPSTATE_HI_LOW:
                    TP_DDR |= (1 << TP2_HIGH);
                    TP_PORT &= ~(1 << TP2_HIGH);
                    break;
                case TPSTATE_LI_HIGH:
                    TP_DDR |= (1 << TP2_LOW);
                    TP_PORT |= (1 << TP2_LOW);
                    break;
                case TPSTATE_LI_LOW:
                    TP_DDR |= (1 << TP2_LOW);
                    TP_PORT &= ~(1 << TP2_LOW);
                    break;
                case TPSTATE_ZI_LOW:
                    ADC_DDR |= (1 << ADC_TP2);
                    ADC_PORT &= ~(1 << ADC_TP2);
                    break;
            }
            break;
        case 3:
            switch(tpState) {
                case TPSTATE_HI_HIGH:
                    TP_DDR |= (1 << TP3_HIGH);
                    TP_PORT |= (1 << TP3_HIGH);
                    break;
                case TPSTATE_HI_LOW:
                    TP_DDR |= (1 << TP3_HIGH);
                    TP_PORT &= ~(1 << TP3_HIGH);
                    break;
                case TPSTATE_LI_HIGH:
                    TP_DDR |= (1 << TP3_LOW);
                    TP_PORT |= (1 << TP3_LOW);
                    break;
                case TPSTATE_LI_LOW:
                    TP_DDR |= (1 << TP3_LOW);
                    TP_PORT &= ~(1 << TP3_LOW);
                    break;
                case TPSTATE_ZI_LOW:
                    ADC_DDR |= (1 << ADC_TP3);
                    ADC_PORT &= ~(1 << ADC_TP3);
                    break;
            }
            break;
    }
}

uint16_t read_adc_on_tp(uint8_t tp, uint8_t reference) {
    switch(tp) {
        case 1:
            return read_adc(ADMUX_TP1, reference);
            break;
        case 2:
            return read_adc(ADMUX_TP2, reference);
            break;
        case 3:
            return read_adc(ADMUX_TP3, reference);
            break;
        default:
            return 0;
            break;
    }
}