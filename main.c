/*
 * File:   main.c
 * Author: norman
 *
 */

// required by __delay - functions
#define _XTAL_FREQ 20000000
 
#include <xc.h>
#include "config.h"

pixel_t cube[][][]= {  
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

void main(void) {
    PORTA = 0x00; //Initial PORTA
    TRISA = 0xFF; //Define PORTA as input
    ADCON1 = 0x0F; //Turn off ADcon
    CMCON = 0x07; //Turn off Comparator
    PORTB = 0x00; //Initial PORTB
    TRISB = 0x00; //Define PORTB as output
    // TRISBbits.RB0 = 0; // SDI
    PORTC = 0x00; //Initial PORTC
    TRISC = 0x00; //Define PORTC as output
    // OE
    PORTBbits.RB3 = 1;
    
    
    return;
}
