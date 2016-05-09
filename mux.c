#include <xc.h>
#include "mux.h"
#include "time.h"
// cube[][][] is initialized in main.c
extern color_t cube[3][3][3];



void mux_init() {
    mux_t reg=0;
    P_OE=0;
    
    __mux_shift_out(reg);
    P_OE=1;
    P_OE_Y = 1;
    // fill the MUX-Reg-Translation LUT for one layer
    mux_lut[0][0].p.reg_x=1;
    mux_lut[0][0].p.reg_y=1;
    
    mux_lut[0][1].p.reg_x=1;
    mux_lut[0][1].p.reg_y=1<<1;
    
    mux_lut[0][2].p.reg_x=1<<1;
    mux_lut[0][2].p.reg_y=1;
    
    mux_lut[1][0].p.reg_x=1<<3;
    mux_lut[1][0].p.reg_y=1<<1;
    
    mux_lut[1][1].p.reg_x=1;
    mux_lut[1][1].p.reg_y=1<<6;
    
    mux_lut[1][2].p.reg_x=1<<1;
    mux_lut[1][2].p.reg_y=1<<1;
    
    mux_lut[2][0].p.reg_x=1<<3;
    mux_lut[2][0].p.reg_y=1;
    
    mux_lut[2][1].p.reg_x=1<<2;
    mux_lut[2][1].p.reg_y=1<<1;
    
    mux_lut[2][2].p.reg_x=1<<2;
    mux_lut[2][2].p.reg_y=1<<0;
    
}

void mux_show_layer(byte z) {
    // TODO: incoporate current player indicator
    
    mux_register_t reg_struct = mux_get_by_layer(z);
   
    mux_t reg = (mux_t)~reg_struct.p.reg_x + (mux_t)(reg_struct.p.reg_y << 8);
    P_OE=0;
    
    __mux_shift_out(reg);
    P_OE=1;
    
}

void mux_set_y_for_input(byte reg_y) {
    mux_t reg = (mux_t)(reg_y & 0xFF);
    P_OE=0;
    __mux_shift_out(reg);
    P_OE=1;
}

 void mux_test_output(byte cycles) {
    byte c,z,x,y;
    for (c=0; c<cycles; c++) {
        for(z=0; z<3; z++) {
            for (y=0; y<3; y++) {
                for(x=0; x<3; x++) {
                    cube[z][y][x]= (c%2==0) ? RED : GREEN;
                    mux_show_layer(z);
                    wait(200);
                    cube[z][y][x]= BLANK;
                }
            }
        }
    }
 }

mux_register_t mux_get_by_layer(byte z) {
    mux_register_t out;
    mux_register_t reg;
    out.value=0;
    byte z_shift = z*2;
    for (byte y=0; y<3; y++) {
        for (byte x=0; x<3; x++) {
            if (cube[z][y][x] != BLANK) {
                reg.value = 0;
                if (x==1 && y==1 && z>0) {
                    reg.p.reg_x = 1 << (cube[z][y][x]==RED ? 1 : 5);
                    reg.p.reg_y = 1 << (z==1 ? 6 : 7);
                }
                else {
                    switch (cube[z][y][x]) {
                        case GREEN:
                            reg.p.reg_x = mux_lut[y][x].p.reg_x;
                            break;
                        case RED:
                            reg.p.reg_x = mux_lut[y][x].p.reg_x << 4;
                            break;
                        case YELLOW:
                            reg.p.reg_x = mux_lut[y][x].p.reg_x + (mux_lut[y][x].p.reg_x << 4);
                            break;
                    }
                    reg.p.reg_y = mux_lut[y][x].p.reg_y << z_shift;
                }
                out.value += reg.value;
            }
        }
    }
    return out;
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