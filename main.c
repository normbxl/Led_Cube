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
            if (tmr_signal.display_layer==0) {
                tmr_signal.sense_layer = (tmr_signal.sense_layer+1) % LAYERS;
                tmr_signal.refresh = SENSE;
            }
            else {
                tmr_signal.refresh = DISPLAY;
            }
            
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

void test_main() {
    bool pDown = false;
    time_t pDown_ts;
    byte c=9;
    
    color_t col = GREEN;

    //mux_register_t reg_struct;
    while (1) {
        //PORTCbits.RC2 = P_RESET;
        if (P_RESET == 0 && !pDown) {
            pDown=true;
            pDown_ts=TIME;
        }
        // on release
        else if ( P_RESET == 1 && pDown && pDown_ts + 100 < TIME ) {
           
            
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

void set_uc_gnd(byte reg) {
    
}

void adc_test_main_2() {
    char *b;
    int status;
    Serial_begin(9600);

    char b1[10];
    byte layer=0;
    while (1) {
        byte y, x, k, z;
        // TODO: check for center LEDs
        // mux_register_t regs = mux_get_by_layer(layer);
        // activate only unused ones
        byte gnd_reg = (0xC0 >> (layer*2)) | 0x3;  // ~regs.p.reg_y;
        // 
        
        byte gnd_mask = 0b10101000;

        int adc_value;
        mux_register_t rt;
        
        // discharge
        mux_set_y_for_measurment(0xFF);
        
        for (byte r=0; r<3; r++) {
            
            z = r==0 ? 0 : 1;
            for (byte i=0; i < (r==2 ? 1 : 4); i++) {
                ADCON2bits.ADCS = 0b110; // Fosc/64
                ADCON2bits.ACQT = 0x02; // 20 Tads to charge C

                ADCON0bits.CHS = i;
                //mux_set_y_for_input(gnd_reg & gnd_mask);
                
                
                ADCON0bits.GO = 1;
                do {
                    NOP();
                }
                while (ADCON0bits.DONE == 0);
                
                adc_value = (ADRESH << 8) + ADRESL;
                itoa(b1, adc_value, 10);
                Serial_write((byte)(i*2+r+48));
                Serial_print("=");
                Serial_print(b1);
                Serial_print("\t");
            }
            gnd_mask = gnd_mask >> 1;
        }
        
        Serial_print("\r\n");
        wait(200);
    }

}

void adc_test_main() {
    char *b;
    int status;
    //Serial_begin(9600);
    //mux_test_output(1);
    
    while (1) {
        switch (tmr_signal.refresh) {
           case SENSE:
               tmr_signal.busy = 1;
               led_covered = detector_check(tmr_signal.sense_layer, &sensed_pixel);
               if (led_covered) {
                   cube[sensed_pixel.z][sensed_pixel.y][sensed_pixel.x] = GREEN;
//                   Serial_print("LED COVERED: ");
//                   Serial_write(sensed_pixel.x+48); 
//                   Serial_print(", ");
//                   Serial_write(sensed_pixel.y+48); 
//                   Serial_print(", ");
//                   Serial_write(sensed_pixel.z+48); 
//                   Serial_print("\r\n");
               }
           case DISPLAY:
               tmr_signal.busy = 1;
               mux_show_layer(tmr_signal.display_layer);
               tmr_signal.refresh=NONE;
               tmr_signal.busy=0;
               break;
        }
        /*
        if (Serial_available()) {
            while (Serial_available()>0) {
                byte b=Serial_read();
                switch(b) {
                    case 'a':
                        inc_alpha(-0.02);
                        break;
                    case 'A':
                        inc_alpha(0.02);
                        break;
                    case 'b':
                        inc_beta(-0.02);
                        break;
                    case 'B':
                        inc_beta(0.02);
                        break;
                    case 'c':
                        for (byte a=0; a<27; a++) {
                            *(((color_t*)cube)+a) = BLANK;
                        }
                        break;
                    default:
                        break;
                }
            }
            
            Serial_print("ALPHA=");
            b = ftoa(inc_alpha(0.0), &status);
            Serial_print(b);
            Serial_print(" BETA=");
            b = ftoa(inc_beta(0.0), &status);
            Serial_println(b);
        }
        */ 
         
    }
    
    
}

void main(void) {
    init_ports();
    fsm_init();
    init_timer();
    mux_init();

    adc_test_main();

    mux_test_output(1);
    //test_main();
    set_test_pattern();
    
    while(1)
    {
        //fsm_loop();
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
