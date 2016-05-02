/*
 * File:   main.c
 * Author: norman
 *
 */

// required by __delay - functions
#define _XTAL_FREQ 20000000
 
#include <xc.h>
#include "config.h"
#include "mux.h"
#include "fsm.h"

/**
 Z-Y-X array holding the LED state 
 */
pixel_t cube[3][3][3]= {  
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

void init_ports() {
    ADCON1bits.VCFG0 = 0;   // Vref = Vdd
    ADCON1bits.VCFG1 = 0;   // Vref- = Vss
    ADCON1bits.PCFG = 0b1010;   // AN0-AN3 = analog in
    
    //ADCON0bits.CHS = 0x7;   // select AN0-AN3
    ADCON2bits.ACQT = 0x04; // 6 Tads to charge C
    ADCON2bits.ADCS = 0b110; // Fosc/64
    
    ADCON0bits.ADON = 1;    // turn on ADC
    
    PORTA = 0x00; //Initial PORTA
    TRISA = 0xFF; //Define PORTA as input
    PORTB = 0x00; //Initial PORTB
    TRISB = 0x00; //Define PORTB as output
    // TRISBbits.RB0 = 0; // SDI
    PORTC = 0x00; //Initial PORTC
    TRISC = 0x02; //Define PORTC as output except C1
    // OE
    PORTBbits.RB3 = 1;
}

void init_timer() {
    T0CONbits.T08BIT=1; // 8 bit counter
    T0CONbits.T0PS=0x7  // prescaler 256 
}

void interrupt ISR() {
    
    
    
}

void main(void) {
    init_ports();
    fsm_init();
    
    
    while(1)
    {
        fsm_loop();
    }
    return;
}
