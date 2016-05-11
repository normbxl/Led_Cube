#include "fsm.h"
#include "intelligence.h"

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
extern color_t cube[3][3][3];
color_t winner_color;

void fsm_init() {
    current_state = START_FSM;
    state_to_recover = START_FSM;

    led_covered = false;
    three_in_a_row = false;
    LED_set = false;
    winner_determined = false;
    winner_color = BLANK;
    
    current_player = GREEN;
}

bool fsm_is_end_of_game() {
    bool result=true;
    
    for (byte i=0; i<27; i++) {
        if ((color_t)(*((color_t*)cube + i)) == BLANK) {
            result = false;
            break;
        }
    }
    return result;
}

/********************************************************************
 * Function:        LED_Cube(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will not work.          
 ********************************************************************/
void fsm_loop(void) {

    switch (current_state) {
        case START_FSM:
            // *** outputs ***

            // *** transitions ***
            if (P_RESET == PUSHED) {
                current_state = BTN_DWN;
            }
            break;

        case BTN_DWN:
            // *** outputs ***

            // *** transitions ***
            if (P_RESET != PUSHED) {
                current_state = WAIT_FOR_LED_EVENT_FSM;
            }
            break;

        case WAIT_FOR_LED_EVENT_FSM:
            // *** outputs ***

            // *** transitions ***
            if (P_RESET == PUSHED) {
                current_state = RESET;
            }

            if (led_covered == true) {
                current_state = SET_CHOSEN_LED_FSM;
            }
            break;

        case SET_CHOSEN_LED_FSM:
            // *** outputs ***
            cube[sensed_pixel.z][sensed_pixel.y][sensed_pixel.x] = current_player;
            
            // *** transitions ***

            // RESET
            if (P_RESET == PUSHED) {
                current_state = RESET;
            }

            if (LED_set == 1) {
                current_state = CHECK_3_IN_A_ROW_FSM;
            }
            break;

        case CHECK_3_IN_A_ROW_FSM:
            // *** outputs ***
            
            winner_color = three_in_a_row_check();
            
            // *** transitions ***
            if (winner_color == BLANK) {
                current_player = current_player == GREEN ? RED : GREEN;
                current_state = WAIT_FOR_LED_EVENT_FSM;
            }
            else {
                current_state = DETERMINE_WINNER_FSM;
            }
            break;


        case DETERMINE_WINNER_FSM:
            // *** outputs ***


            // *** transitions ***
            if (winner_determined == 1) {
                current_state = FLASH_WIN_FSM;
            }
            else if (fsm_is_end_of_game()==true) {
                // TODO: show draw animation
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
            if (P_RESET != PUSHED) {
                current_state = START_FSM;
            }
            break;

        default:
            current_state = START_FSM;
            break;
    }


}
//EOF-------------------------------------------------------------------------
