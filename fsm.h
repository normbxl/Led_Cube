/* 
 * File:   fsm.h
 * Author: norman
 *
 * Created on 2. Mai 2016, 00:28
 */

#ifndef FSM_H
#define	FSM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include "config.h"
// #include <stdbool.h>

#define PUSHED 0


bool led_covered;
pixel3_t selected_pixel;


void fsm_init();
void fsm_loop(void);

void fsm_clear_cube();

#ifdef	__cplusplus
}
#endif

#endif	/* FSM_H */

