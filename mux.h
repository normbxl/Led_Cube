/* 
 * File:   mux.h
 * Author: norman
 *
 * Created on 20. April 2016, 12:53
 */

#ifndef MUX_H
#define	MUX_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"
    
    void mux_init();
    /**
     * Sets output of X and Y port accordingly.
     * X-Port is automatically inverted to controll the PNP transistors 
     * @param reg_x non-inverted bitmask, bits 0:3 -> green 1:4, bits 4:7 -> red 1:4
     * @param reg_y bitmask, bits 0:7 -> GND 0:7
     */
    void mux_set_output_xy(byte reg_x, byte reg_y);
    /**
     * Set MUX for input-readings, X-Ports are switched off, only Y-coords are needed
     * @param reg_y
     */
    void mux_set_input(byte reg_y);
    
    /**
     * private function to actually send stuff
     * @param reg
     */
    void __mux_shift_out(mux_t reg);
    
    mux_t mux_cube_layer_to_mux(byte z);
    

#ifdef	__cplusplus
}
#endif

#endif	/* MUX_H */

