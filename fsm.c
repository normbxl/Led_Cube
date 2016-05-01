#include "fsm.h"

/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          MATHIAS YEAh!!!!!
 * Updated:         17/02/2016
 ********************************************************************/

void fsm_init(void) 
{
    current_state = START_FSM;
    state_to_recover = START_FSM;

    LED_covered = 0;
    three_in_a_row = 0;
    LED_set = 0;
    winner_determined = 0;

}

/********************************************************************
 * Function:        LED_Cube(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will not work.          
 ********************************************************************/
void fsm_loop(void) {

    switch (current_state) 
    {
        case START_FSM:
            // *** outputs ***

            // *** transitions ***
            if (PORTCbits.RC1 == PUSHED) 
            {
                current_state = BTN_DWN;
            }
            break;

        case BTN_DWN:
            // *** outputs ***

            // *** transitions ***
            if (PORTCbits.RC1 != PUSHED) 
            {
                current_state = WAIT_FOR_LED_EVENT_FSM;
            }
            break;

        case WAIT_FOR_LED_EVENT_FSM:
            // *** outputs ***


            // *** transitions ***
            if (PORTCbits.RC1 == PUSHED) 
            {
                current_state = RESET;
            }

            if (LED_covered == 1) 
            {
                current_state = SET_CHOSEN_LED_FSM;
            }
            break;

        case SET_CHOSEN_LED_FSM:
            // *** outputs ***


            // *** transitions ***
            if (PORTCbits.RC1 == PUSHED) 
            {
                current_state = RESET;
            }

            if (LED_set == 1) 
            {
                current_state = CHECK_3_IN_A_ROW_FSM;
            }
            break;

        case CHECK_3_IN_A_ROW_FSM:
            // *** outputs ***


            // *** transitions ***
            if (three_in_a_row == 0) 
            {
                current_state = WAIT_FOR_LED_EVENT_FSM;
            }

            if (three_in_a_row == 1) 
            {
                current_state = DETERMINE_WINNER_FSM;
            }
            break;


        case DETERMINE_WINNER_FSM:
            // *** outputs ***


            // *** transitions ***
            if (winner_determined == 1) 
            {
                current_state = FLASH_WIN_FSM;
            }
            break;

        case FLASH_WIN_FSM:
            // *** outputs ***


            // *** transitions ***
            ;
            break;

        case RESET:
            // *** outputs ***

            // *** transitions ***
            if (PORTCbits.RC1 != PUSHED) 
            {
                current_state = START_FSM;
            }
            break;

        default:
            current_state = START_FSM;
            break;
    }

}
//EOF-------------------------------------------------------------------------
