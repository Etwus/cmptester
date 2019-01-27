// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "semiconductors.h"
#include "adc.h"
#include "utils.h"
#include "iodefs.h"
#include "constants.h"

uint8_t check_for_diodes(DiodeCheckResult *result) {
    uint16_t temp1;
    uint16_t temp2;
    uint8_t numberOfDiodes = 0;
    Diode *diode = result->diode;

    // Dioda mezi 1 a 2
    reset_tps();
    TP_DDR |= (1 << TP1_LOW);
    ADC_DDR |= (1 << ADC_TP2);
    TP_PORT |= (1 << TP1_LOW);
    _delay_ms(1);
    temp1 = read_adc(ADMUX_TP1, REFERENCE_AVCC);

    reset_tps();
    TP_DDR |= (1 << TP2_LOW);
    ADC_DDR |= (1 << ADC_TP1);
    TP_PORT |= (1 << TP2_LOW);
    _delay_ms(1);
    temp2 = read_adc(ADMUX_TP2, REFERENCE_AVCC);

    if((temp1 < ADC_MAX_TRESHOLD)
            || (temp2 < ADC_MAX_TRESHOLD)) {
        if((temp1 < temp2) && (abs(temp1 - temp2) > 100)) {
            numberOfDiodes++;
            diode[numberOfDiodes-1].voltageDrop = (5.0f/1023.0f) * temp1;
            diode[numberOfDiodes-1].anode = 1;
            diode[numberOfDiodes-1].cathode = 2;
        } else if ((temp1 > temp2) && (abs(temp1 - temp2) > 100)) {
            numberOfDiodes++;
            diode[numberOfDiodes-1].voltageDrop = (5.0f/1023.0f) * temp2;
            diode[numberOfDiodes-1].anode = 2;
            diode[numberOfDiodes-1].cathode = 1;
        }
    }

    // Dioda mezi 2 a 3
    reset_tps();
    TP_DDR |= (1 << TP2_LOW);
    ADC_DDR |= (1 << ADC_TP3);
    TP_PORT |= (1 << TP2_LOW);
    _delay_ms(1);
    temp1 = read_adc(ADMUX_TP2, REFERENCE_AVCC);

    reset_tps();
    TP_DDR |= (1 << TP3_LOW);
    ADC_DDR |= (1 << ADC_TP2);
    TP_PORT |= (1 << TP3_LOW);
    _delay_ms(1);
    temp2 = read_adc(ADMUX_TP3, REFERENCE_AVCC);

    if((temp1 < ADC_MAX_TRESHOLD)
            || (temp2 < ADC_MAX_TRESHOLD)) {
        if((temp1 < temp2) && (abs(temp1 - temp2) > 100)) {
            numberOfDiodes++;
            diode[numberOfDiodes-1].voltageDrop = (5.0f/1023.0f) * temp1;
            diode[numberOfDiodes-1].anode = 2;
            diode[numberOfDiodes-1].cathode = 3;
        } else if ((temp1 > temp2) && (abs(temp1 - temp2) > 100)) {
            numberOfDiodes++;
            diode[numberOfDiodes-1].voltageDrop = (5.0f/1023.0f) * temp2;
            diode[numberOfDiodes-1].anode = 3;
            diode[numberOfDiodes-1].cathode = 2;
        }
    }

    // Dioda mezi 1 a 3
    reset_tps();
    TP_DDR |= (1 << TP1_LOW);
    ADC_DDR |= (1 << ADC_TP3);
    TP_PORT |= (1 << TP1_LOW);
    _delay_ms(1);
    temp1 = read_adc(ADMUX_TP1, REFERENCE_AVCC);

    reset_tps();
    TP_DDR |= (1 << TP3_LOW);
    ADC_DDR |= (1 << ADC_TP1);
    TP_PORT |= (1 << TP3_LOW);
    _delay_ms(1);
    temp2 = read_adc(ADMUX_TP3, REFERENCE_AVCC);

    if((temp1 < ADC_MAX_TRESHOLD)
            || (temp2 < ADC_MAX_TRESHOLD)) {
        if((temp1 < temp2) && (abs(temp1 - temp2) > 100)) {
            numberOfDiodes++;
            diode[numberOfDiodes-1].voltageDrop = (5.0f/1023.0f) * temp1;
            diode[numberOfDiodes-1].anode = 1;
            diode[numberOfDiodes-1].cathode = 3;
        } else if ((temp1 > temp2) && (abs(temp1 - temp2) > 100)) {
            numberOfDiodes++;
            diode[numberOfDiodes-1].voltageDrop = (5.0f/1023.0f) * temp2;
            diode[numberOfDiodes-1].anode = 3;
            diode[numberOfDiodes-1].cathode = 1;

        }
    }

    reset_tps();

    result->numberOfDiodes = numberOfDiodes;

    if (numberOfDiodes > 0) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t check_for_bjt(TransistorCheckResult *result,
                      DiodeCheckResult diodeCheckResult) {
    // NPN?
    if (diodeCheckResult.diode[0].anode == diodeCheckResult.diode[1].anode) {
        uint8_t base = diodeCheckResult.diode[0].anode;
        uint8_t emitter = 0;
        uint8_t collector = 0;
        if (diodeCheckResult.diode[0].voltageDrop > diodeCheckResult.diode[1]
            .voltageDrop) {
            emitter = diodeCheckResult.diode[0].cathode;
            collector = diodeCheckResult.diode[1].cathode;
        } else {
            emitter = diodeCheckResult.diode[1].cathode;
            collector = diodeCheckResult.diode[0].cathode;
        }

        uint16_t baseMeasurement = 0;
        uint16_t collectorMeasurement = 0;

        reset_tps();
        switch (base) {
            case TP1:
                if (emitter == TP2) {
                    TP_DDR |= (1 << TP1_HIGH) | (1 << TP3_LOW);
                    ADC_DDR |= (1 << ADC_TP2);
                    TP_PORT |= (1 << TP1_HIGH) | (1 << TP3_LOW);
                    baseMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    collectorMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                } else {
                    TP_DDR |= (1 << TP1_HIGH) | (1 << TP2_LOW);
                    ADC_DDR |= (1 << ADC_TP3);
                    TP_PORT |= (1 << TP1_HIGH) | (1 << TP2_LOW);
                    baseMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    collectorMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                }
                break;
            case TP2:
                if (emitter == TP1) {
                    TP_DDR |= (1 << TP2_HIGH) | (1 << TP3_LOW);
                    ADC_DDR |= (1 << ADC_TP1);
                    TP_PORT |= (1 << TP2_HIGH) | (1 << TP3_LOW);
                    baseMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    collectorMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                } else {
                    TP_DDR |= (1 << TP2_HIGH) | (1 << TP1_LOW);
                    ADC_DDR |= (1 << ADC_TP3);
                    TP_PORT |= (1 << TP2_HIGH) | (1 << TP1_LOW);
                    baseMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    collectorMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                }
                break;
            case TP3:
                if (emitter == TP1) {
                    TP_DDR |= (1 << TP3_HIGH) | (1 << TP2_LOW);
                    ADC_DDR |= (1 << ADC_TP1);
                    TP_PORT |= (1 << TP3_HIGH) | (1 << TP2_LOW);
                    baseMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    collectorMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                } else {
                    TP_DDR |= (1 << TP3_HIGH) | (1 << TP1_LOW);
                    ADC_DDR |= (1 << ADC_TP2);
                    TP_PORT |= (1 << TP3_HIGH) | (1 << TP1_LOW);
                    baseMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    collectorMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                }
                break;
        }
        reset_tps();

        float baseResistorVoltage = ((ADC_MAX_TRESHOLD - baseMeasurement)
                / (float)ADC_MAX_TRESHOLD) * ((ADC_MAX_TRESHOLD / 1023.0f)
                * 5.0f);
        float collectorResistorVoltage = ((ADC_MAX_TRESHOLD
                - collectorMeasurement) / (float)ADC_MAX_TRESHOLD)
                * ((ADC_MAX_TRESHOLD / 1023.0f) * 5.0f);

        float amplificationFactor = (collectorResistorVoltage / 680.0f)
                / (baseResistorVoltage / 470000.0f);

        if (amplificationFactor > 10) {
            result->type = NPN;
            result->base = base;
            result->collector = collector;
            result->emitter = emitter;
            result->hfe = amplificationFactor;
            return 1;
        }
    // PNP ?
    } else if (diodeCheckResult.diode[0].cathode == diodeCheckResult
            .diode[1].cathode) {
        uint8_t base = diodeCheckResult.diode[0].cathode;
        uint8_t emitter = 0;
        uint8_t collector = 0;
        if (diodeCheckResult.diode[0].voltageDrop > diodeCheckResult.diode[1]
            .voltageDrop) {
            emitter = diodeCheckResult.diode[0].anode;
            collector = diodeCheckResult.diode[1].anode;
        } else {
            emitter = diodeCheckResult.diode[1].anode;
            collector = diodeCheckResult.diode[0].anode;
        }

        uint16_t baseMeasurement = 0;
        uint16_t emitterMeasurement = 0;

        reset_tps();
        switch (base) {
            case TP1:
                if (emitter == TP2) {
                    TP_DDR |= (1 << TP1_HIGH) | (1 << TP2_LOW);
                    ADC_DDR |= (1 << ADC_TP3);
                    TP_PORT |= (1 << TP2_LOW);
                    baseMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    emitterMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                } else {
                    TP_DDR |= (1 << TP1_HIGH) | (1 << TP3_LOW);
                    ADC_DDR |= (1 << ADC_TP2);
                    TP_PORT |= (1 << TP3_LOW);
                    baseMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    emitterMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                }
                break;
            case TP2:
                if (emitter == TP1) {
                    TP_DDR |= (1 << TP2_HIGH) | (1 << TP1_LOW);
                    ADC_DDR |= (1 << ADC_TP3);
                    TP_PORT |= (1 << TP1_LOW);
                    baseMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    emitterMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                } else {
                    TP_DDR |= (1 << TP2_HIGH) | (1 << TP3_LOW);
                    ADC_DDR |= (1 << ADC_TP1);
                    TP_PORT |= (1 << TP3_LOW);
                    baseMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    emitterMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                }
                break;
            case TP3:
                if (emitter == TP1) {
                    TP_DDR |= (1 << TP3_HIGH) | (1 << TP1_LOW);
                    ADC_DDR |= (1 << ADC_TP2);
                    TP_PORT |= (1 << TP1_LOW);
                    baseMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    emitterMeasurement = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                } else {
                    TP_DDR |= (1 << TP3_HIGH) | (1 << TP2_LOW);
                    ADC_DDR |= (1 << ADC_TP1);
                    TP_PORT |= (1 << TP2_LOW);
                    baseMeasurement = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    emitterMeasurement = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                }
                break;
        }
        reset_tps();

        float baseResistorVoltage = (baseMeasurement / (float)ADC_MAX_TRESHOLD)
                * ((ADC_MAX_TRESHOLD/1023.0f) * 5.0f);
        float emitterResistorVoltage = ((ADC_MAX_TRESHOLD - emitterMeasurement)
                / (float)ADC_MAX_TRESHOLD) * ((ADC_MAX_TRESHOLD/1023.0f)
                * 5.0f);
        float amplificationFactor = (emitterResistorVoltage / 680.0f)
                / (baseResistorVoltage / 470000.0f);

        if (amplificationFactor > 10) {
            result->type = PNP;
            result->base = base;
            result->collector = collector;
            result->emitter = emitter;
            result->hfe = amplificationFactor;
            return 1;
        }
    }
    return 0;
}

uint8_t check_for_ujt(TransistorCheckResult *result,
        DiodeCheckResult diodeCheckResult) {
    Diode *diode = diodeCheckResult.diode;
    uint8_t bodyDiodeIndex = 0;
    uint8_t source = 0;
    uint8_t drain = 0;
    uint8_t gate = 0;
    uint16_t temp = 0;

    if (diodeCheckResult.numberOfDiodes > 1) {
        if (diode[0].voltageDrop < diode[1].voltageDrop) {
            bodyDiodeIndex = 0;
        } else {
            bodyDiodeIndex = 1;
        }
    }

    // P-MOS

    source = diode[bodyDiodeIndex].cathode;
    drain = diode[bodyDiodeIndex].anode;
    switch(diode[bodyDiodeIndex].anode) {
        case 1:
            switch(diode[bodyDiodeIndex].cathode) {
                case 2:
                    gate = 3;
                    break;
                case 3:
                    gate = 2;
                    break;
            }
            break;
        case 2:
            switch(diode[bodyDiodeIndex].cathode) {
                case 1:
                    gate = 3;
                    break;
                case 3:
                    gate = 1;
                    break;
            }
            break;
        case 3:
            switch(diode[bodyDiodeIndex].cathode) {
                case 1:
                    gate = 2;
                    break;
                case 2:
                    gate = 1;
                    break;
            }
            break;
    }

    switch(gate) {
        case 1:
            if (source == 2) {
                reset_tps();
                TP_DDR |= (1 << TP1_LOW) | (1 << TP2_LOW);
                ADC_DDR |= (1 << ADC_TP3);
                TP_PORT |= (1 << TP1_LOW) | (1 << TP2_LOW);
                temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                if (temp == ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP1_LOW);
                    temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    if (temp < ADC_MAX_TRESHOLD) {
                        result->type = PMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            } else {
                reset_tps();
                TP_DDR |= (1 << TP1_LOW) | (1 << TP3_LOW);
                ADC_DDR |= (1 << ADC_TP2);
                TP_PORT |= (1 << TP1_LOW) | (1 << TP3_LOW);
                temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                if (temp == ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP1_LOW);
                    temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    if (temp < ADC_MAX_TRESHOLD) {
                        result->type = PMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            }
            break;
        case 2:
            if (source == 1) {
                reset_tps();
                TP_DDR |= (1 << TP2_LOW) | (1 << TP1_LOW);
                ADC_DDR |= (1 << ADC_TP3);
                TP_PORT |= (1 << TP2_LOW) | (1 << TP1_LOW);
                temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                if (temp == ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP2_LOW);
                    temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    if (temp < ADC_MAX_TRESHOLD) {
                        result->type = PMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            } else {
                reset_tps();
                TP_DDR |= (1 << TP2_LOW) | (1 << TP3_LOW);
                ADC_DDR |= (1 << ADC_TP1);
                TP_PORT |= (1 << TP2_LOW) | (1 << TP3_LOW);
                temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                if (temp == ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP2_LOW);
                    temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    if (temp < ADC_MAX_TRESHOLD) {
                        result->type = PMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            }
            break;
        case 3:
            if (source == 1) {
                reset_tps();
                TP_DDR |= (1 << TP3_LOW) | (1 << TP1_LOW);
                ADC_DDR |= (1 << ADC_TP2);
                TP_PORT |= (1 << TP3_LOW) | (1 << TP1_LOW);
                temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                if (temp == ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP3_LOW);
                    temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    if (temp < ADC_MAX_TRESHOLD) {
                        result->type = PMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            } else {
                reset_tps();
                TP_DDR |= (1 << TP3_LOW) | (1 << TP2_LOW);
                ADC_DDR |= (1 << ADC_TP1);
                TP_PORT |= (1 << TP3_LOW) | (1 << TP2_LOW);
                temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                if (temp == ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP3_LOW);
                    temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    if (temp < ADC_MAX_TRESHOLD) {
                        result->type = PMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            }
            break;
    }

    // NMOS

    drain = diode[bodyDiodeIndex].cathode;
    source = diode[bodyDiodeIndex].anode;

        switch(gate) {
        case 1:
            if (drain == 2) {
                reset_tps();
                TP_DDR |= (1 << TP1_LOW) | (1 << TP2_LOW);
                ADC_DDR |= (1 << ADC_TP3);
                TP_PORT |= (1 << TP1_LOW) | (1 << TP2_LOW);
                temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                if (temp < ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP1_LOW);
                    temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    if (temp == ADC_MAX_TRESHOLD) {
                        result->type = NMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            } else {
                reset_tps();
                TP_DDR |= (1 << TP1_LOW) | (1 << TP3_LOW);
                ADC_DDR |= (1 << ADC_TP2);
                TP_PORT |= (1 << TP1_LOW) | (1 << TP3_LOW);
                temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                if (temp < ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP1_LOW);
                    temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    if (temp == ADC_MAX_TRESHOLD) {
                        result->type = NMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            }
            break;
        case 2:
            if (drain == 1) {
                reset_tps();
                TP_DDR |= (1 << TP2_LOW) | (1 << TP1_LOW);
                ADC_DDR |= (1 << ADC_TP3);
                TP_PORT |= (1 << TP2_LOW) | (1 << TP1_LOW);
                temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                if (temp < ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP2_LOW);
                    temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    if (temp == ADC_MAX_TRESHOLD) {
                        result->type = NMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            } else {
                reset_tps();
                TP_DDR |= (1 << TP2_LOW) | (1 << TP3_LOW);
                ADC_DDR |= (1 << ADC_TP1);
                TP_PORT |= (1 << TP2_LOW) | (1 << TP3_LOW);
                temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                if (temp < ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP2_LOW);
                    temp = read_adc(ADMUX_TP3, REFERENCE_AVCC);
                    if (temp == ADC_MAX_TRESHOLD) {
                        result->type = NMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            }
            break;
        case 3:
            if (drain == 1) {
                reset_tps();
                TP_DDR |= (1 << TP3_LOW) | (1 << TP1_LOW);
                ADC_DDR |= (1 << ADC_TP2);
                TP_PORT |= (1 << TP3_LOW) | (1 << TP1_LOW);
                temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                if (temp < ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP3_LOW);
                    temp = read_adc(ADMUX_TP1, REFERENCE_AVCC);
                    if (temp == ADC_MAX_TRESHOLD) {
                        result->type = NMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            } else {
                reset_tps();
                TP_DDR |= (1 << TP3_LOW) | (1 << TP2_LOW);
                ADC_DDR |= (1 << ADC_TP1);
                TP_PORT |= (1 << TP3_LOW) | (1 << TP2_LOW);
                temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                if (temp < ADC_MAX_TRESHOLD) {
                    TP_PORT &= ~(1<<TP3_LOW);
                    temp = read_adc(ADMUX_TP2, REFERENCE_AVCC);
                    if (temp == ADC_MAX_TRESHOLD) {
                        result->type = NMOS;
                        result->gate = gate;
                        result->source = source;
                        result->drain = drain;
                        reset_tps();
                        return 1;
                    }
                }
            }
            break;
    }

    reset_tps();
    return 0;

}

uint8_t check_for_zener(ZenerCheckResult *result,
        DiodeCheckResult diodeCheckResult) {

    result->numberOfDiodes = 0;

    for (uint8_t i = 0; i < diodeCheckResult.numberOfDiodes; i++) {
        uint8_t anode = diodeCheckResult.diode[i].anode;
        uint8_t cathode = diodeCheckResult.diode[i].cathode;
        reset_tps();
        set_tp(anode, TPSTATE_ZI_LOW);
        set_tp(cathode, TPSTATE_LI_HIGH);
        uint16_t temp = read_adc_on_tp(cathode, REFERENCE_AVCC);
        if (temp < ADC_MAX_TRESHOLD) {
            result->numberOfDiodes++;
            result->zener[i].forwardVoltage
                    = diodeCheckResult.diode[0].voltageDrop;
            result->zener[i].anode = anode;
            result->zener[i].cathode = cathode;
            result->zener[i].zenerVoltage = (temp / 1023.0f) * 5.0f;
        }
    }

    reset_tps();

    if (result->numberOfDiodes > 0) {
        return 1;
    }

    return 0;
}


