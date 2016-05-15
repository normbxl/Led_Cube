
#include <pic18f2550.h>
#include <xc.h>
#include "config.h"
#include "detector.h"
#include "mux.h"
#include "time.h"

#define DETECT_THRESHOLD 20.0

float old_q[3][3][3];
float old_v[3][3][3];

float ALPHA = 0.1;
float BETA = 0.5;
byte initializing = 3;

extern color_t cube[3][3][3];

pixel2_t rev_lut[2][9] = {
    {
        {0, 0},{1, 0},{2, 0},
        {2, 1},{2, 2},{1, 2},
        {0, 2},{0, 1},{1, 1}
    },
    {
        {0, 0},{0, 1},{2, 0},
        {1, 0},{2, 2},{2, 1},
        {0, 2},{1, 2},{1, 1}
    }
};

bool detector_check(byte layer, pixel3_t *pos) {
    byte y, x, k, z;
    // TODO: check for center LEDs
    // mux_register_t regs = mux_get_by_layer(layer);
    // activate only unused ones
    byte gnd_reg = (0xC0 >> (layer*2)) | 0x3;  // ~regs.p.reg_y;
    // 
    byte gnd_mask = 0b10101000;
    float q, v;
    bool result=false;
    bool init=false;
    
    if (initializing) {
        initializing--;
        init=true;
    }
    
    // we have to shift the gnd-mask twice to multiplex the 4 analog input that
    // are connected to the green anode, the last shift is for the center pole
    // which is connected to A0
    for (byte r=0; r<3; r++) {
        // discharge all lines
        mux_set_y_for_input(0xFF);
        // wait for 50µs to discharge LED anodes
        // __delay_us(50);
        z = r==0 ? 0 : 1;
        for (byte i=0; i < (r==2 ? 1 : 4); i++) {
            ADCON2bits.ADCS = 0b110; // Fosc/64
            ADCON2bits.ACQT = 0x02; // 20 Tads to charge C
            
            ADCON0bits.CHS = i;
            // logic AND to mask out even or odd gnd connectors
            mux_set_y_for_input(gnd_reg & gnd_mask);
            ADCON0bits.GO = 1;
            do {
                // do something useful while waiting for the ADC 
                k = (i<<1) + (r == 2 ? 8 : r);
            }
            
            while (ADCON0bits.DONE == 0);
            mux_set_y_for_input(0xFF);
            
            x = rev_lut[z][k].x;
            y = rev_lut[z][k].y;
            int adc_value = (ADRESH << 8) + ADRESL;
            
            if (init) {
                 old_q[layer][y][x] = (float)adc_value;
                 old_v[layer][y][x] = (float)adc_value;
            }
            
            // slow adapting
            q = ALPHA *(float)adc_value + old_q[layer][y][x] * (1-ALPHA);
            // fast adapting
            v = BETA *(float)adc_value + old_v[layer][y][x] * (1-BETA);
            if (q-v > DETECT_THRESHOLD && !result && cube[layer][y][x] == BLANK) {
                pos->x = x;
                pos->y = y;
                pos->z = layer;
                result = true;
            }
        }
        gnd_mask = gnd_mask >> 1;
    }
    return result;
}
float inc_alpha(float f) {
    ALPHA = (ALPHA+f > 0.0 && ALPHA+f < 1.0 )? ALPHA+f : ALPHA;
    return ALPHA;
}
float inc_beta(float f) {
    BETA = (BETA+f > 0.0 && BETA+f < 1.0 )? BETA+f : BETA;
    return BETA;
}