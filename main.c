/*
 * File:   main.c
 * Author: norman
 *
 */

// required by __delay - functions
#define _XTAL_FREQ 48000000
#define UART_DEBUG

#include <xc.h>
#include <pic18f2550.h>
#include "config.h"
#include "mux.h"
#include "fsm.h"
#include "detector.h"
#include "time.h"

#ifdef UART_DEBUG
    // #include "serial/system.h"
    #include "serial/serial.h"
    #include <stdlib.h>
    #include <math.h>
    #include <string.h>
#endif
/**
 Z-Y-X array holding the LED state 
 */
color_t cube[3][3][3]= {  
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

volatile time_t time__ = 0;
volatile timer_signal_t tmr_signal;

void init_ports() {
    
    PORTA = 0x00; //Initial PORTA
    TRISA = 0xFF; //Define PORTA as input
    PORTB = 0x00; //Initial PORTB
    TRISB = 0xF0; //Define PORTB0:3 as output and PORTB4:7 as input
    // TRISBbits.RB0 = 0; // SDI
    // turining off weak pull-up
    INTCON2bits.RBPU=1;
    PORTC = 0x00; //Initial PORTC
    TRISC = 0b11000011;// RC0:1 and RC6:7 Inputs
    
    // ADC config
    ADCON1bits.VCFG0 = 0;   // Vref = Vdd
    ADCON1bits.VCFG1 = 0;   // Vref- = Vss
    ADCON1bits.PCFG = 0x0B;   // AN0-AN3 = analog in

    //ADCON0bits.CHS = 0x7;   // select AN0-AN3
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0x05; // 20 Tads to charge C
    ADCON2bits.ADCS = 0b110; // Fosc/64
    ADCON0bits.ADON = 1;    // turn on ADC
    
    P_OE=1;
}

void init_timer() {
    T0CONbits.T08BIT = 1;   // 8bit
    T0CONbits.T0CS=0;       // instruction clock (12 MHz)
    T0CONbits.PSA=0;        // use prescaler
    T0CONbits.T0PS=0x07;    // prescaler 256 => P=5.4ms f=183Hz
    INTCONbits.TMR0IE = 1;  // enable timer0 interrupt
    T0CONbits.TMR0ON = 1;   // Timer0 On
    
    T2CONbits.T2CKPS = 0x2;  // Timer2 PS 16x
    T2CONbits.TOUTPS = 0x4;   // 4x post scaler
    PR2 = 187;               // compare value for Timer2 Interrupt
    T2CONbits.TMR2ON = 1;   // Timer 2 On
    PIE1bits.TMR2IE = 1;    // Timer 2 interrupt enable
    
    INTCONbits.PEIE = 1;    // enable peripherical interrupts
    INTCONbits.GIE = 1;     // enable interrupts
    
}

void interrupt ISR() {
    if (INTCONbits.TMR0IF == 1) {
        /* after a round of displaying all 3 layers
         * a sensing round is inserted for one layer
         * d0 d1 d2 s0 d0 d1 d2 s1 d0 d1 d2 s2, ...
         */
        if (tmr_signal.busy==0) {
            tmr_signal.display_layer = (tmr_signal.display_layer+1) % LAYERS;
//            if (tmr_signal.display_layer==0) {
//                tmr_signal.sense_layer = (tmr_signal.sense_layer+1) % LAYERS;
//                tmr_signal.refresh = SENSE;
//            }
//            else {
                tmr_signal.refresh = DISPLAY;
            //}
            
        }
        INTCONbits.TMR0IF = 0;  // clear TMR0 interrupt flag
    }
    if (PIR1bits.TMR2IF == 1) {
        time__++;
        PIR1bits.TMR2IF = 0;
    }
#ifdef UART_DEBUG
    if (RCIF) {
        Serial_ReadISR();
    }
#endif
}

void set_test_pattern() {
    byte rr = 0x23;
    byte rc;
    for (byte i=0; i<27; i++) {
        // pseudo random
        for (byte c=0; c<3; c++) {
            rr = (rr << 1) | ((rr & 0x80) >> 7) ^ ((rr & 0x40)>>6);
        }
        rc = rr & 0x1;
        *((color_t*)cube + i) = rc > 1 ? RED : GREEN;
    }
    
}

void main(void) {
    init_ports();
    fsm_init();
    init_timer();
    mux_init();

    //mux_test_output(2);
    //test_main();
    //set_test_pattern();
    
    while(1)
    {
        fsm_loop();
        switch (tmr_signal.refresh) {
            case SENSE:
                tmr_signal.busy = 1;
                led_covered = detector_check(tmr_signal.sense_layer, &sensed_pixel);
            case DISPLAY:
                tmr_signal.busy = 1;
                mux_show_layer(tmr_signal.display_layer);
                tmr_signal.refresh=NONE;
                tmr_signal.busy=0;
                break;
        }
    }
}
