
#include <pic18f2550.h>
#include <xc.h>
#include "config.h"
#include "detector.h"
#include "mux.h"

bool detector_check(byte layer, pixel3_t *pos) {
    byte y, x, k;
    // TODO: check for center LEDs
    mux_register_t regs = mux_get_by_layer(layer);
    // activate only unused ones
    byte gnd_reg = ~regs.p.reg_y;
    // 
    byte gnd_mask = 0b00010101;
    float q, v;
    bool result=false;
    
    // we have to shift the gnd-mask twice to multiplex the 4 analog input that
    // are connected to the green anode, the last shift is for the center pole
    // which is connected to A0
    for (byte r=0; r<3; r++) {
        // logic AND to mask out even or odd gnd connectors
        mux_set_y_for_input(gnd_reg & gnd_mask);
        
        for (byte i=0; i < (r==2 ? 1 : 4); i++) {
            
            ADCON0bits.CHS = i;
            ADCON0bits.GO = 1;

            while (ADCON0bits.DONE == 0) {
                // do something useful while waiting for the ADC 
                k = (i<<1) + (r == 2 ? 8 : r);
            }
            x = rev_lut[k].x;
            y = rev_lut[k].y;
            int adc_value = (ADRESH << 2) + (ADRESL >> 6);
            q = ALPHA *(float)adc_value + old_q[layer][y][x] * (1-ALPHA);
            v = BETA *(float)adc_value + old_v[layer][y][x] * (1-BETA);
            if (v-q > DETECT_THRESHOLD && result) {
                pos->x = x;
                pos->y = y;
                pos->z = layer;
                result = true;
            }
        }
        gnd_mask = gnd_mask << 1;
    }
    return result;
}