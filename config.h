/* 
 * File:   config.h
 * Author: norman
 *
 * Created on 20. April 2016, 12:37
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define _XTAL_FREQ 20000000
    
typedef unsigned char   uint8_t;
typedef uint8_t         byte;
typedef unsigned int    mux_t;
typedef enum {false=0, true} bool;


typedef enum { BLANK=0, GREEN, RED, YELLOW } pixel_t ;

// 3-D pixel
typedef struct {
    byte x;
    byte y;
    byte z;
} pixel3_t;

typedef struct {
    uint8_t reg_x;
    uint8_t reg_y;
} mux_parts_t;

typedef union {
    mux_t value;
    mux_parts_t p;
} mux_register_t;


// Port shortcuts
#define P_CLK   PORTBbits.RB1   // clock
#define P_STR   PORTBbits.RB0   // strobe
#define P_DAT   PORTCbits.RC7   // data
    
#define P_OE  PORTBbits.RB2   // output-enable X-lines (G-R)
#define P_OE_Y  PORTBbits.RB3   // output-enable Y-lines (GND)   


    
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

