/*
 * File:   main.c
 * Author: norman
 *
 * Created on 15. November 2015, 10:07
 */
#define _XTAL_FREQ 20000000

#include <xc.h>

typedef unsigned char uint8_t;

uint8_t counter = 0;

void shift_out( uint8_t data) {
    //clock
    PORTBbits.RB1 = 0;
    // strobe
    PORTBbits.RB0 = 0;
    for (uint8_t i = 0; i<8; i++) {
        PORTCbits.RC7 = (data >> i) & 0x01;
        if (i==7) {
            // strobe
            PORTBbits.RB0 = 1;
        }
        //__delay_us(1);
        //clock
        PORTBbits.RB1 = 1;
        NOP();
        //__delay_us(1);
        PORTBbits.RB1 = 0;
    
    }
    
    //__delay_us(1);
    // strobe
    PORTBbits.RB0 = 0;

    return;
}

void main(void) {
    PORTA = 0x00; //Initial PORTA
    TRISA = 0xFF; //Define PORTA as input
    TRISAbits.RA5 = 0; // SS
    ADCON1 = 0x0F; //Turn off ADcon
    CMCON = 0x07; //Turn off Comparator
    PORTB = 0x00; //Initial PORTB
    TRISB = 0x00; //Define PORTB as output
    // TRISBbits.RB0 = 0; // SDI
    PORTC = 0x00; //Initial PORTC
    TRISC = 0x00; //Define PORTC as output
    // OE
    PORTBbits.RB3 = 1;
    
    while(1) {
        counter = (counter+1) % 255;
        shift_out(counter);
        
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        __delay_ms(20);
        
    }
    
    return;
}
