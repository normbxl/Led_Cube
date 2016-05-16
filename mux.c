#include <xc.h>
#include "mux.h"
#include "time.h"
// cube[][][] is initialized in main.c
extern color_t cube[3][3][3];

extern color_t current_player;

mux_register_t layer_reg;

const pixel2_t mux_rev_lut[2][8] = {
    {
        {0, 0},{1, 0},{2, 0},
        {2, 1},{2, 2},{1, 2},
        {0, 2},{0, 1}
    },
    {
        {0, 0},{0, 1},{2, 0},
        {1, 0},{2, 2},{2, 1},
        {0, 2},{1, 2}
    }
};

void mux_init() {
    mux_t reg=0;
    
    __mux_shift_out(reg);
    P_OE=1;
    
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
    mux_lut[1][0][1].p.reg_x=1<<2; 
    mux_lut[1][0][1].p.reg_y=1<<4;
    // 11
    mux_lut[1][0][2].p.reg_x=1<<2;
    mux_lut[1][0][2].p.reg_y=1<<5;
    // 12
    mux_lut[1][1][2].p.reg_x=1<<1;
    mux_lut[1][1][2].p.reg_y=1<<4;
    // 13
    mux_lut[1][2][2].p.reg_x=1<<1;
    mux_lut[1][2][2].p.reg_y=1<<5;
    // 14
    mux_lut[1][2][1].p.reg_x=1<<0;
    mux_lut[1][2][1].p.reg_y=1<<4;
    // 15
    mux_lut[1][2][0].p.reg_x=1<<0;
    mux_lut[1][2][0].p.reg_y=1<<5;
    // 16
    mux_lut[1][1][0].p.reg_x=1<<3;
    mux_lut[1][1][0].p.reg_y=1<<4;
    // 26
    mux_lut[1][1][1].p.reg_x=1<<2;
    mux_lut[1][1][1].p.reg_y=1<<1;
    
    // Layer 3
    
    // 17
    mux_lut[2][0][0].p.reg_x=1<<3;
    mux_lut[2][0][0].p.reg_y=1<<3;
    // 18
    mux_lut[2][0][1].p.reg_x=1<<2; 
    mux_lut[2][0][1].p.reg_y=1<<2;
    // 19
    mux_lut[2][0][2].p.reg_x=1<<2;
    mux_lut[2][0][2].p.reg_y=1<<3;
    // 20
    mux_lut[2][1][2].p.reg_x=1<<1;
    mux_lut[2][1][2].p.reg_y=1<<2;
    // 21
    mux_lut[2][2][2].p.reg_x=1<<1;
    mux_lut[2][2][2].p.reg_y=1<<3;
    // 22
    mux_lut[2][2][1].p.reg_x=1<<0;
    mux_lut[2][2][1].p.reg_y=1<<2;
    // 23
    mux_lut[2][2][0].p.reg_x=1<<0;
    mux_lut[2][2][0].p.reg_y=1<<3;
    // 24
    mux_lut[2][1][0].p.reg_x=1<<3;
    mux_lut[2][1][0].p.reg_y=1<<2;
    // 25
    mux_lut[2][1][1].p.reg_x=1<<2;
    mux_lut[2][1][1].p.reg_y=1<<0;
    
}

void mux_show_layer(byte z) {
    
    mux_register_t reg = mux_get_by_layer(z >> 1, z % 2);;
    
    // add current player indication
    if (z==6) {
        reg.p.reg_x |= (current_player == RED ? 0x20 : 0x2);
        reg.p.reg_y |= 0x2;
    }
    
    
    reg.p.reg_x = ~reg.p.reg_x;
    
    __mux_shift_out(reg.value);
    
}

void mux_set_y_for_measurment(byte reg_y) {
    
    mux_register_t reg_struct;
    reg_struct.p.reg_y = reg_y;
    // x-drivers must be set high (pnp-transistors: in blocking mode)
    reg_struct.p.reg_x = 0xFF;
    __mux_shift_out(reg_struct.value);
}

 void mux_test_output(byte cycles) {
    byte c,z,x,y;
    for (c=0; c<cycles; c++) {
        for(z=0; z<3; z++) {
            for (y=0; y<3; y++) {
                for(x=0; x<3; x++) {
                    cube[z][y][x]= (c%2==0) ? RED : GREEN;
                    mux_show_layer(z);
                    wait(300);
                    cube[z][y][x]= BLANK;
                }
            }
        }
    }
 }

 /**
  * 
  * @param z layer number
  * @param g ground number (0 or 1)
  * @return 
  */
mux_register_t mux_get_by_layer(byte z, byte g) {
    mux_register_t out;
    mux_register_t w_reg;
    color_t color;
    byte x, y;
    out.value=0;
    byte r = r==0 ? 0 : 1;
    byte _z = z;
    
    
    for (byte k=g; k<8; k+=2) {
        if (_z==3) {
            x=1;
            y=1;
            z= g==0 ? (k==0 ? 0 : 1) : 2;
            // end loop
            if (z >= 1) {
                k=9;
            }
        }
        else {
            x=mux_rev_lut[r][k].x;
            y=mux_rev_lut[r][k].y;
        }
        color = cube[z][y][x];
        if (color > RED) {
            color = color <= CURSOR_RED_B ? color - 2 : ((color-3) >> 1); 
            color = blink() ? color : BLANK;
        }
        //color = color < 3 ? color : (color-3) >> 1; 
        if (color != BLANK) {
            w_reg.value = 0;
            switch (color) {
                case GREEN:
                    w_reg.p.reg_x = mux_lut[z][y][x].p.reg_x << 4;
                    break;
                case RED:
                    w_reg.p.reg_x = mux_lut[z][y][x].p.reg_x;
                    break;
            }
            w_reg.p.reg_y = mux_lut[z][y][x].p.reg_y;

            out.value |= w_reg.value;
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
        __delay_us(2);
        P_CLK = 0;
    }
    // strobe
    P_STR = 0;
    P_DAT = 0;
}