// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <avr/io.h>
#include <util/delay.h>
#include "capacitors.h"
#include "adc.h"
#include "utils.h"
#include "stopwatch.h"
#include "constants.h"
#include "iodefs.h"
#include "screens.h"

uint8_t check_for_capacitors(CapacitorCheckResult *result) {
    uint16_t temp = 0;
    uint32_t elapsed = 0;
    result->numberOfCapacitors = 0;

    // Kondensator mezi 1 a 2
    read_adc(ADMUX_TP1, REFERENCE_AVCC);
    reset_tps();
    TP_DDR |= (1 << TP1_LOW) | (1 << TP2_LOW);
    TP_PORT |= (1 << TP1_LOW);
    _delay_ms(300);
    reset_tps();
    temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
    if (temp > 400 && temp <= ADC_MAX_TRESHOLD) {
        discharge_tps();
        while(read_adc(ADMUX_TP1, REFERENCE_AVCC) > 0) {

        }

        reset_tps();
        TP_DDR |= (1 << TP1_HIGH);
        ADC_DDR |= (1 << ADC_TP2);
        TP_PORT |= (1 << TP1_HIGH);
        _delay_ms(300);
        temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
        reset_tps();
        discharge_tps();
        while(read_adc(ADMUX_TP1, REFERENCE_AVCC) > 0) {

        }

        if (temp > 644) {
            reset_tps();
            TP_DDR |= (1 << TP1_HIGH);
            ADC_DDR |= (1 << ADC_TP2);
            TP_PORT |= (1 << TP1_HIGH);
            stopwatch_start();
            while(read_adc(ADMUX_TP1, REFERENCE_AVCC) < 644) {

            }
            elapsed = stopwatch_stop();
            result->capacitor[result->numberOfCapacitors].capacityInMicroF
                    = (elapsed / 16.0f) / (float)TP1_HIGH_RESISTANCE;
        } else {
            reset_tps();
            show_capacitor_charging();
            TP_DDR |= (1 << TP1_LOW);
            ADC_DDR |= (1 << ADC_TP2);
            TP_PORT |= (1 << TP1_LOW);
            stopwatch_start();
            while(read_adc(ADMUX_TP1, REFERENCE_AVCC) < 644) {

            }
            elapsed = stopwatch_stop();
            result->capacitor[result->numberOfCapacitors].capacityInMicroF
                    = (elapsed / 16.0f) / (float)TP1_LOW_RESISTANCE;
        }

        reset_tps();
        if (elapsed > 7500) {
            result->capacitor[result->numberOfCapacitors].firstLead = 1;
            result->capacitor[result->numberOfCapacitors].secondLead = 2;
            result->numberOfCapacitors++;
        }
        discharge_tps();
        while(read_adc(ADMUX_TP1, REFERENCE_AVCC) > 0) {

        }
    }

    // Kondensator mezi 2 a 3

    read_adc(ADMUX_TP2, REFERENCE_AVCC);
    reset_tps();
    TP_DDR |= (1 << TP2_LOW) | (1 << TP3_LOW);
    TP_PORT |= (1 << TP2_LOW);
    _delay_ms(300);
    reset_tps();
    temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
    if (temp > 400 && temp <= ADC_MAX_TRESHOLD) {
        discharge_tps();
        while(read_adc(ADMUX_TP2, REFERENCE_AVCC) > 0) {

        }

        reset_tps();
        TP_DDR |= (1 << TP2_HIGH);
        ADC_DDR |= (1 << ADC_TP3);
        TP_PORT |= (1 << TP2_HIGH);
        _delay_ms(300);
        temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
        reset_tps();
        discharge_tps();
        while(read_adc(ADMUX_TP2, REFERENCE_AVCC) > 0) {

        }

        if (temp > 644) {
            reset_tps();
            TP_DDR |= (1 << TP2_HIGH);
            ADC_DDR |= (1 << ADC_TP3);
            TP_PORT |= (1 << TP2_HIGH);
            stopwatch_start();
            while(read_adc(ADMUX_TP2, REFERENCE_AVCC) < 644) {

            }
            elapsed = stopwatch_stop();
            result->capacitor[result->numberOfCapacitors].capacityInMicroF
                    = (elapsed / 16.0f) / (float)TP2_HIGH_RESISTANCE;
        } else {
            reset_tps();
            show_capacitor_charging();
            TP_DDR |= (1 << TP2_LOW);
            ADC_DDR |= (1 << ADC_TP3);
            TP_PORT |= (1 << TP2_LOW);
            stopwatch_start();
            while(read_adc(ADMUX_TP2, REFERENCE_AVCC) < 644) {

            }
            elapsed = stopwatch_stop();
            result->capacitor[result->numberOfCapacitors].capacityInMicroF
                    = (elapsed / 16.0f) / (float)TP2_LOW_RESISTANCE;
        }

        reset_tps();
        if (elapsed > 7500) {
            result->capacitor[result->numberOfCapacitors].firstLead = 2;
            result->capacitor[result->numberOfCapacitors].secondLead = 3;
            result->numberOfCapacitors++;
        }
        discharge_tps();
        while(read_adc(ADMUX_TP2, REFERENCE_AVCC) > 0) {

        }
    }


    // Kondensator mezi 1 a 3
    if (result->numberOfCapacitors < 2) {
        read_adc(ADMUX_TP1, REFERENCE_AVCC);
        reset_tps();
        TP_DDR |= (1 << TP1_LOW) | (1 << TP3_LOW);
        TP_PORT |= (1 << TP1_LOW);
        _delay_ms(300);
        reset_tps();
        temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
        if (temp > 400 && temp <= ADC_MAX_TRESHOLD) {
            discharge_tps();
            while(read_adc(ADMUX_TP1, REFERENCE_AVCC) > 0) {

            }

            reset_tps();
            TP_DDR |= (1 << TP1_HIGH);
            ADC_DDR |= (1 << ADC_TP3);
            TP_PORT |= (1 << TP1_HIGH);
            _delay_ms(300);
            temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
            reset_tps();
            discharge_tps();
            while(read_adc(ADMUX_TP1, REFERENCE_AVCC) > 0) {

            }

            if (temp > 644) {
                reset_tps();
                TP_DDR |= (1 << TP1_HIGH);
                ADC_DDR |= (1 << ADC_TP3);
                TP_PORT |= (1 << TP1_HIGH);
                stopwatch_start();
                while(read_adc(ADMUX_TP1, REFERENCE_AVCC) < 644) {

                }
                elapsed = stopwatch_stop();
                result->capacitor[result->numberOfCapacitors].capacityInMicroF
                        = (elapsed / 16.0f) / (float)TP1_HIGH_RESISTANCE;
            } else {
                reset_tps();
                show_capacitor_charging();
                TP_DDR |= (1 << TP1_LOW);
                ADC_DDR |= (1 << ADC_TP3);
                TP_PORT |= (1 << TP1_LOW);
                stopwatch_start();
                while(read_adc(ADMUX_TP1, REFERENCE_AVCC) < 644) {

                }
                elapsed = stopwatch_stop();
                result->capacitor[result->numberOfCapacitors].capacityInMicroF
                        = (elapsed / 16.0f) / (float)TP1_LOW_RESISTANCE;
            }

            reset_tps();

            if (elapsed > 7500) {
                result->capacitor[result->numberOfCapacitors].firstLead = 1;
                result->capacitor[result->numberOfCapacitors].secondLead = 3;
                result->numberOfCapacitors++;
            }
                discharge_tps();
            while(read_adc(ADMUX_TP1, REFERENCE_AVCC) > 0) {

            }
        }
    }

    if (result->numberOfCapacitors > 0) {
        return 1;
    }
    return 0;
}