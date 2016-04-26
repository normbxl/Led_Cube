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
    
    mux_register_t mux_lut[3][3];
    
    void mux_init();
    /**
     * Set MUX for input-readings, X-Ports are switched off, only Y-coords are needed
     * @param reg_y
     */
    void mux_set_y_for_input(byte reg_y);
    
    /**
     * outputs layer z by looking up set pixels in cube[][][]
     * @param z 0..2 layer to show
     */
    void mux_show_layer(byte z);
    
    /**
     * private function to actually send stuff
     * @param reg
     */
    void __mux_shift_out(mux_t reg);
    
    mux_register_t mux_get_mux_by_layer(byte z);
    

#ifdef	__cplusplus
}
#endif

#endif	/* MUX_H */

