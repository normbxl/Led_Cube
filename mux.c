#include <xc.h>
#include "mux.h"

extern pixel_t cube[][][];

void mux_init() {
    mux_t reg=0;
    P_OE_X=0;
    P_OE_Y=0;
    
    __mux_shift_out(reg);
    P_OE_X=1;
    P_OE_Y=1;
    
}

void mux_set_output_xy(byte reg_x, byte reg_y) {
    mux_t reg = (mux_t)~reg_x + (mux_t)(reg_y << 8);
    P_OE_X=0;
    P_OE_Y=0;
    __mux_shift_out(reg);
    P_OE_X=1;
    P_OE_Y=2;
}

void mux_set_input(byte reg_y) {
    mux_t reg = (mux_t)(reg_y << 8);
    P_OE_X=0;
    P_OE_Y=0;
    __mux_shift_out(reg);
    P_OE_Y=2;   
}

mux_t mux_cube_layer_to_mux(byte z) {
    
}

void __mux_shift_out(mux_t reg) {
    //clock
    P_CLK = 0;
    // strobe
    P_STR = 0;
    for (uint8_t i = 0; i<16; i++) {
        // data
        P_DAT = (reg >> i) & 0x01;
        if (i==15) {
            // strobe
            P_STR = 1;
        }
        //clock impulse
        P_CLK = 1;
        __nop();
        P_CLK = 0;
    }
    // strobe
    P_STR = 0;
}