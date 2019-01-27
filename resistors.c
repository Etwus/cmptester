// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <stdlib.h>
#include "resistors.h"
#include "utils.h"
#include "iodefs.h"
#include "adc.h"
#include "constants.h"

float calculateResistance(float adcReferenceVoltage, uint16_t adcValue,
        uint32_t knownResistance) {
    float current = (5 - ((adcValue / 1023.0f) * adcReferenceVoltage))
            / (float)knownResistance;
    return ((adcValue / 1023.0f) * adcReferenceVoltage) / current;
}

uint8_t check_for_single_resistor(Resistor* resistor,
        uint8_t tp1, uint8_t tp2) {
    uint16_t temp1 = 0;

    // 0 - 190 ohmu
    reset_tps();
    set_tp(tp1, TPSTATE_LI_HIGH);
    set_tp(tp2, TPSTATE_ZI_LOW);
    temp1 = read_adc_on_tp(tp1, REFERENCE_1V1);

    if (temp1 < ADC_MAX_TRESHOLD) {
        resistor->firstLead = tp1;
        resistor->secondLead = tp2;
        resistor->resistance = calculateResistance(1.1, temp1,
                TP1_LOW_RESISTANCE);
        reset_tps();
        return 1;
    } else {
        // 190 - 7k ohmu
        temp1 = read_adc_on_tp(tp1, REFERENCE_AVCC);

        if (temp1 < 934 && temp1 > 0) {
            resistor->firstLead = tp1;
            resistor->secondLead = tp2;
            resistor->resistance = calculateResistance(5, temp1,
                    TP1_LOW_RESISTANCE);
            reset_tps();
            return 1;
        } else {
            // 7k+ ohmu
            reset_tps();
            set_tp(tp1, TPSTATE_HI_HIGH);
            set_tp(tp2, TPSTATE_ZI_LOW);
            temp1 = read_adc_on_tp(tp1, REFERENCE_1V1);
            if (temp1 < ADC_MAX_TRESHOLD && temp1 > 0) {
                resistor->firstLead = tp1;
                resistor->secondLead = tp2;
                resistor->resistance = calculateResistance(1.1, temp1,
                        TP1_HIGH_RESISTANCE);
                reset_tps();
                return 1;
            } else {
                temp1 = read_adc_on_tp(tp1, REFERENCE_AVCC);
                if ((temp1 < 990) && (temp1 > 0)) {
                    resistor->firstLead = tp1;
                    resistor->secondLead = tp2;
                    resistor->resistance = calculateResistance(5, temp1,
                            TP1_HIGH_RESISTANCE);
                    reset_tps();
                    return 1;
                }
            }
        }
    }

    reset_tps();
    return 0;

}

uint8_t check_for_resistors(ResistorCheckResult* result) {
        Resistor resistor[3] = {{0}};

        uint8_t numberOfResistorsFound = 0;

        if(check_for_single_resistor(&resistor[numberOfResistorsFound], 1, 2)) {
            result->resistor[numberOfResistorsFound]
                    = resistor[numberOfResistorsFound];
            numberOfResistorsFound++;
        }

        if(check_for_single_resistor(&resistor[numberOfResistorsFound], 2, 3)) {
            result->resistor[numberOfResistorsFound]
                    = resistor[numberOfResistorsFound];
            numberOfResistorsFound++;
        }

        if(check_for_single_resistor(&resistor[numberOfResistorsFound], 1, 3)) {
            if (numberOfResistorsFound > 1) {
                uint8_t isResistor1CombinationOfResistors = (resistor[0]
                        .resistance > resistor[2].resistance)
                        && (resistor[0].resistance > resistor[1].resistance);
                uint8_t isResistor2CombinationOfResistors = (resistor[1]
                        .resistance > resistor[2].resistance)
                        && (resistor[1].resistance > resistor[0].resistance);
                if (isResistor1CombinationOfResistors) {
                    result->resistor[0] = resistor[2];
                } else if (isResistor2CombinationOfResistors) {
                    result->resistor[1] = resistor[2];
                }
            } else {
                result->resistor[numberOfResistorsFound]
                        = resistor[numberOfResistorsFound];
                numberOfResistorsFound++;
            }
        }

        if (numberOfResistorsFound > 0) {
            result->numberOfResistors = numberOfResistorsFound;
            return 1;
        }
        return 0;
}
