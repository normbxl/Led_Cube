/* 
 * File:   config.h
 * Author: norman
 *
 * Created on 20. April 2016, 12:37
 */
#include <stdbool.h>

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define _XTAL_FREQ 20000000
#define LAYERS 3

// Port shortcuts
#define P_CLK   PORTBbits.RB1   // clock
#define P_STR   PORTBbits.RB0   // strobe
#define P_DAT   PORTCbits.RC7   // data
    
#define P_OE  PORTBbits.RB2     // output-enable X-lines (G-R)
#define P_OE_Y  PORTBbits.RB3   // output-enable Y-lines (GND)
#define P_RESET PORTCbits.RC1   // RESET

    
typedef unsigned char   uint8_t;
typedef uint8_t         byte;
typedef unsigned int    mux_t;

typedef enum { BLANK=0, GREEN, RED, YELLOW } color_t ;

// 3-D pixel, 2 bits per component is sufficent in a 3x3x3 world
typedef struct {
    byte x  :2;
    byte y  :2;
    byte z  :2;
} pixel3_t;

typedef struct {
    byte x  :2;
    byte y  :2;
} pixel2_t;

typedef struct {
    uint8_t reg_x;
    uint8_t reg_y;
} mux_parts_t;

typedef union {
    mux_t value;
    mux_parts_t p;
} mux_register_t;

volatile struct {
    byte display_layer      : 2;
    byte display_refresh    : 1;
    byte sense_layer        : 2;
    byte sense_refresh      : 1;
    byte busy               : 1;
} tmr_signal;
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

