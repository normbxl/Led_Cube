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
pixel3_t sensed_pixel;
bool three_in_a_row;
bool LED_set;
bool winner_determined;



enum 
{
    START_FSM,
    BTN_DWN,
    WAIT_FOR_LED_EVENT_FSM,
    SET_CHOSEN_LED_FSM,
    CHECK_3_IN_A_ROW_FSM, DETERMINE_WINNER_FSM, FLASH_WIN_FSM, RESET
} current_state, state_to_recover;

void fsm_init();
void fsm_loop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FSM_H */

