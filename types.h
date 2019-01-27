// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef TYPES_H
#define TYPES_H

typedef enum {
    INTERNAL2V56, AVCC
} VoltageReference;

typedef enum {
    NPN, PNP, PMOS, NMOS
} TransistorType;

typedef struct {
    uint8_t firstLead;
    uint8_t secondLead;
    float resistance;
} Resistor;

typedef struct {
    uint8_t numberOfResistors;
    Resistor resistor[2];
} ResistorCheckResult;

typedef struct {
    uint8_t anode;
    uint8_t cathode;
    float voltageDrop;
} Diode;

typedef struct {
    uint8_t numberOfDiodes;
    Diode diode[2];
} DiodeCheckResult;

typedef struct {
    TransistorType type;
    union {
        uint8_t base;
        uint8_t gate;
    };
    union {
        uint8_t collector;
        uint8_t source;
    };
    union {
        uint8_t emitter;
        uint8_t drain;
    };
    float hfe;
} TransistorCheckResult;

typedef struct {
    uint8_t firstLead;
    uint8_t secondLead;
    float capacityInMicroF;
} Capacitor;

typedef struct {
    uint8_t numberOfCapacitors;
    Capacitor capacitor[2];
} CapacitorCheckResult;

typedef struct {
    uint8_t anode;
    uint8_t cathode;
    float forwardVoltage;
    float zenerVoltage;
} Zener;

typedef struct {
    uint8_t numberOfDiodes;
    Zener zener[2];
} ZenerCheckResult;

#endif