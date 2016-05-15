/* 
 * File:   button.h
 * Author: norman
 *
 * Created on 15. Mai 2016, 22:53
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <stdbool.h>
#include "config.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {UP, ON_DOWN, DOWN, ON_RELEASE} button_state;
    
    typedef struct {
        button_state state;
        bool initialized;
    } button_t;
    
    button_state button_check(byte val, button_t *b);
    

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTON_H */

