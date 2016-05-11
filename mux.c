#include <xc.h>
#include "mux.h"
#include "time.h"
// cube[][][] is initialized in main.c
extern color_t cube[3][3][3];



void mux_init() {
    mux_t reg=0;
    P_OE=0;
    P_OE_Y = 0;
    __mux_shift_out(reg);
    P_OE=1;
    P_OE_Y = 1;
    // fill the MUX-Reg-Translation LUT for one layer
    // 1
    mux_lut[0][0][0].p.reg_x=1<<3;
    mux_lut[0][0][0].p.reg_y=1<<7;
    // 2
    mux_lut[0][0][1].p.reg_x=1<<3;
    mux_lut[0][0][1].p.reg_y=1<<6;
    // 3
    mux_lut[0][0][2].p.reg_x=1<<2;
    mux_lut[0][0][2].p.reg_y=1<<7;
    // 4
    mux_lut[0][1][2].p.reg_x=1<<2;
    mux_lut[0][1][2].p.reg_y=1<<6;
    // 5
    mux_lut[0][2][2].p.reg_x=1<<1;
    mux_lut[0][2][2].p.reg_y=1<<7;
    // 6
    mux_lut[0][2][1].p.reg_x=1<<1;
    mux_lut[0][2][1].p.reg_y=1<<6;
    // 7
    mux_lut[0][2][0].p.reg_x=1;
    mux_lut[0][2][0].p.reg_y=1<<7;
    // 8
    mux_lut[0][1][0].p.reg_x=1;
    mux_lut[0][1][0].p.reg_y=1<<6;
    // 25
    mux_lut[0][1][1].p.reg_x=1<<3;
    mux_lut[0][1][1].p.reg_y=1<<1;
    
    // Layer 2
    
    // 9
    mux_lut[1][0][0].p.reg_x=1<<3;
    mux_lut[1][0][0].p.reg_y=1<<5;
    // 10
    mux_lut[1][0][1].p.reg_x=1<<1;
    mux_lut[1][0][1].p.reg_y=1<<4;
    // 11
    mux_lut[1][0][2].p.reg_x=1<<2;
    mux_lut[1][0][2].p.reg_y=1<<5;
    // 12
    mux_lut[1][1][2].p.reg_x=1;
    mux_lut[1][1][2].p.reg_y=1<<5;
    // 13
    mux_lut[1][2][2].p.reg_x=1;
    mux_lut[1][2][2].p.reg_y=1<<4;
    // 14
    mux_lut[1][2][1].p.reg_x=1<<2;
    mux_lut[1][2][1].p.reg_y=1<<4;
    // 15
    mux_lut[1][2][0].p.reg_x=1<<1;
    mux_lut[1][2][0].p.reg_y=1<<4;
    // 16
    mux_lut[1][1][0].p.reg_x=1<<3;
    mux_lut[1][1][0].p.reg_y=1<<4;
    // 26
    mux_lut[1][1][1].p.reg_x=1<<2;
    mux_lut[1][1][1].p.reg_y=1<<1;
    
    // Layer 2
    
    // 17
    mux_lut[2][0][0].p.reg_x=1<<3;
    mux_lut[2][0][0].p.reg_y=1<<3;
    // 18
    mux_lut[2][0][1].p.reg_x=1<<1;
    mux_lut[2][0][1].p.reg_y=1<<2;
    // 19
    mux_lut[2][0][2].p.reg_x=1<<2;
    mux_lut[2][0][2].p.reg_y=1<<3;
    // 20
    mux_lut[2][1][2].p.reg_x=1;
    mux_lut[2][1][2].p.reg_y=1<<2;
    // 21
    mux_lut[2][2][2].p.reg_x=1;
    mux_lut[2][2][2].p.reg_y=1<<2;
    // 22
    mux_lut[2][2][1].p.reg_x=1<<2;
    mux_lut[2][2][1].p.reg_y=1<<2;
    // 23
    mux_lut[2][2][0].p.reg_x=1<<1;
    mux_lut[2][2][0].p.reg_y=1<<2;
    // 24
    mux_lut[2][1][0].p.reg_x=1<<3;
    mux_lut[2][1][0].p.reg_y=1<<2;
    // 27
    mux_lut[2][1][1].p.reg_x=1<<2;
    mux_lut[2][1][1].p.reg_y=1;
    
}

void mux_show_layer(byte z) {
    // TODO: incoporate current player indicator
    
    mux_register_t reg_struct = mux_get_by_layer(z);
    reg_struct.p.reg_x = ~reg_struct.p.reg_x;
    
    P_OE=0;
    P_OE_Y=0;
    __mux_shift_out(reg_struct.value);
    P_OE=1;
    P_OE_Y=1;
    
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
                    wait(5000);
                    //cube[z][y][x]= BLANK;
                }
            }
        }
        wait(50000);
    }
 }

mux_register_t mux_get_by_layer(byte z) {
    mux_register_t out;
    mux_register_t w_reg;
    out.value=0;
    for (byte y=0; y<3; y++) {
        for (byte x=0; x<3; x++) {
            if (cube[z][y][x] != BLANK) {
                w_reg.value = 0;
                /*if (x==1 && y==1 && z>0) {
                    w_reg.p.reg_x = 1 << (cube[z][y][x]==RED ? 2 : 6);
                    w_reg.p.reg_y = 1 << (z==1 ? 1 : 0);
                 */ 
                
                switch (cube[z][y][x]) {
                    case GREEN:
                        w_reg.p.reg_x = mux_lut[z][y][x].p.reg_x << 4;
                        break;
                    case RED:
                        w_reg.p.reg_x = mux_lut[z][y][x].p.reg_x;
                        break;
                    case YELLOW:
                        w_reg.p.reg_x = mux_lut[z][y][x].p.reg_x | (mux_lut[z][y][x].p.reg_x << 4);
                        break;
                }
                w_reg.p.reg_y = mux_lut[z][y][x].p.reg_y;

                out.value |= w_reg.value;
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
    P_DAT = 0;
    for (byte i = 0; i<16; i++) {
        // data
        P_DAT = (reg >> i) & 0x01;
        if (i==15) {
            // strobe
            P_STR = 1;
        }
        //clock impulse
        P_CLK = 1;
        __delay_us(10);
        P_CLK = 0;
        __delay_us(10);
        
        P_DAT = 0;
    }
    // strobe
    P_STR = 0;
}