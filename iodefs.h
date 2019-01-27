// Copyright (c) Aleš Gajdacz
// This source code is licensed under MIT license found in the LICENSE file

#ifndef IODEFS_H
#define IODEFS_H

#include <avr/io.h>

#define TP_PORT         PORTD
#define TP_DDR          DDRD
#define TP_PIN          PIND
#define TP1_HIGH        PD1
#define TP1_LOW         PD0
#define TP2_HIGH        PD5
#define TP2_LOW         PD4
#define TP3_HIGH        PD7
#define TP3_LOW         PD6

#define LATCH_PORT      PORTB
#define LATCH_DDR       DDRB
#define LATCH_PIN       PINB
#define LATCH           PB0

#define SWSIG_PORT      PORTD
#define SWSIG_DDR       DDRD
#define SWSIG_PIN       PIND
#define SWSIG           PD2

#define CONT_PORT       PORTC
#define CONT_DDR        DDRC
#define CONT_PIN        PINC
#define CONT            PC3

#define TEST_PORT       PORTD
#define TEST_DDR        DDRD
#define TEST_PIN        PIND
#define TEST            PD3

#define ADC_PORT        PORTC
#define ADC_DDR         DDRC
#define ADC_PIN         PINC
#define ADC_TP1         PC0
#define ADC_TP2         PC1
#define ADC_TP3         PC2
#define ADC_BATTERY     PC5

#endif