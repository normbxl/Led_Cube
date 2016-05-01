
#include <pic18f2550.h>

#include "config.h"
#include "detector.h"
#include "mux.h"


void detector_init(){
    
}

bool detector_check(byte layer, pixel3_t *pos) {
    byte y, x;
    // TODO: check for center LEDs
    mux_register_t regs = mux_get_by_layer(layer);
    byte gnd_reg = ~regs.p.reg_y;
    
    byte gnd_mask = 0b01010101;
    float q, v;
    
    for (byte r=0; r<2; r++) {
        // logic and to mask out even or odd gnd connectors
        mux_set_y_for_input(gnd_reg & gnd_mask);

        for (byte i=0; i<4; i++) {
            if (gnd_reg)
            ADCON0bits.CHS = i;
            ADCON0bits.GO = 1;

            while (ADCON0bits.DONE == 0) {
                // nop
            }
            int adc_value = (ADRESH << 2) + (ADRESL >> 6);
            q = ALPHA *(float)adc_value + old_q[layer][y][x] * (1-ALPHA);
            v = BETA *(float)adc_value + old_v[layer][y][x] * (1-BETA);
        }
    }
}