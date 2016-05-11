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
#include "detector.h"
#include "time.h"

/**
 Z-Y-X array holding the LED state 
 */
color_t cube[3][3][3]= {  
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

volatile time_t time__ = 0;

void init_ports() {
    ADCON1bits.VCFG0 = 0;   // Vref = Vdd
    ADCON1bits.VCFG1 = 0;   // Vref- = Vss
    ADCON1bits.PCFG = 0b1010;   // AN0-AN3 = analog in
    
    //ADCON0bits.CHS = 0x7;   // select AN0-AN3
    ADCON2bits.ACQT = 0x07; // 20 Tads to charge C
    ADCON2bits.ADCS = 0b110; // Fosc/64
    
    ADCON0bits.ADON = 1;    // turn on ADC
    
    PORTA = 0x00; //Initial PORTA
    TRISA = 0xFF; //Define PORTA as input
    PORTB = 0x00; //Initial PORTB
    TRISB = 0x00; //Define PORTB as output
    // TRISBbits.RB0 = 0; // SDI
    PORTC = 0x00; //Initial PORTC
    TRISC = 0x3 ; //Define PORTC as output except RC0 + RC1
    // OE
    PORTBbits.RB3 = 1;
}

void init_timer() {
    T0CONbits.T08BIT = 1;   // 8bit
    T0CONbits.T0CS=0;       // instruction clock (12 MHz)
    T0CONbits.PSA=0;        // use prescaler
    T0CONbits.T0PS=0x07;    // prescaler 256 => P=5.4ms f=183Hz
    INTCONbits.TMR0IE = 1;  // enable timer0 interrupt
    T0CONbits.TMR0ON = 1;   // Timer0 On
    
    T2CONbits.T2CKPS = 0x2;  // Timer2 PS 16x
    T2CONbits.TOUTPS = 0;   // No post scaler
    PR2 = 74;               // compare value for Timer2 Interrupt
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
            
            if (tmr_signal.display_layer==0) {
                tmr_signal.sense_layer = (tmr_signal.sense_layer+1) % LAYERS;
                tmr_signal.sense_refresh = 1;
                tmr_signal.display_refresh=1;
            }
        }
        INTCONbits.TMR0IF = 0;  // clear TMR0 interrupt flag
    }
    if (PIR1bits.TMR2IF == 1) {
        time__++;
        PIR1bits.TMR2IF = 0;
    }
}

void test_main() {
    bool pDown = false;
    time_t pDown_ts;
    byte c=0;
    
    color_t col = GREEN;

    //mux_register_t reg_struct;
    while (1) {
        PORTCbits.RC2 = P_RESET;
        if (P_RESET == 0 && !pDown) {
            pDown=true;
            pDown_ts=TIME;
        }
        // on release
        else if ( P_RESET == 1 && pDown && pDown_ts + 1000 > TIME ) {
           
            
            //mux_test_output(2);
            //for (byte i=0; i<27; i++) {
            //    *((color_t*)cube + i) = BLANK;
            //}
            
            if (c==27) {
                col= col==RED ? GREEN : RED;
                c=0;
            }
            
            if (c>0) {
                byte d=c-1;
                cube[d/9][(d/3)%3][d%3] = BLANK;
                //*((color_t*)cube + c-1) = BLANK;
            }
            //*((color_t*)cube + c) = col;
            cube[c/9][(c/3)%3][c%3] = col;
            mux_show_layer(c/9);    
            
            c++;
        }
        if (P_RESET==1) {
            pDown=false;
        }
    }
    
}



void main(void) {
    init_ports();
    fsm_init();
    init_timer();
    mux_init();
    test_main();
    
    while(1)
    {
        fsm_loop();
        
        if (tmr_signal.sense_refresh) {
            tmr_signal.busy = true;
            led_covered = detector_check(tmr_signal.sense_layer, &sensed_pixel);
            tmr_signal.sense_refresh = false;
        }
        if (tmr_signal.display_refresh) {
            tmr_signal.busy = true;
            mux_show_layer(tmr_signal.display_layer);
            tmr_signal.display_refresh=0;
            tmr_signal.busy=false;
        }
    }
    
}
