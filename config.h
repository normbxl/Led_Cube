/* 
 * File:   config.h
 * Author: norman
 *
 * Created on 20. April 2016, 12:37
 */
#include <stdbool.h>
#include <xc.h>

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 48000000
#endif
#define LAYERS 3

// Port shortcuts
#define P_CLK   PORTBbits.RB1   // clock
#define P_STR   PORTBbits.RB0   // strobe
#define P_DAT   PORTBbits.RB2   // data
    
#define P_OE    PORTBbits.RB3     // output-enable
    
#define P_LEFT  PORTBbits.RB4
#define P_DOWN  PORTBbits.RB5
#define P_UP    PORTBbits.RB6
#define P_RIGHT PORTBbits.RB7
#define P_FIRE  PORTAbits.RA5
    
#define P_RESET PORTCbits.RC0   // RESET
#define P_LED   PORTCbits.RC1
    
typedef unsigned char   uint8_t;
typedef uint8_t         byte;
typedef unsigned int    mux_t;

typedef enum { BLANK=0, GREEN, RED, CURSOR_GREEN_B, CURSOR_RED_B, CURSOR_GREEN_G, CURSOR_RED_G, CURSOR_GREEN_R, CURSOR_RED_R } color_t ;

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

#define NONE 0
#define FSM 3
#define SENSE 2
#define DISPLAY 1

typedef struct {
    byte display_layer     : 4;
    byte refresh           : 3;
    byte busy              : 1;
} timer_signal_t;
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

