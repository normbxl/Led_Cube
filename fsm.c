#include "fsm.h"
#include "intelligence.h"
#include "button.h"

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

color_t current_player;

pixel3_t cursor;
pixel3_t cursor_old;

button_t btn_up;
button_t btn_down;
button_t btn_left;
button_t btn_right;
button_t btn_fire;



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

bool fsm_joystick_ctrl() {
    button_state fire = button_check(P_FIRE, &btn_fire);
    button_state up = button_check(P_UP, &btn_up);
    button_state down = button_check(P_DOWN, &btn_down);
    button_state left = button_check(P_LEFT, &btn_left);
    button_state right = button_check(P_RIGHT, &btn_right);
    pixel3_t prev_pos = cursor;
    if (fire == ON_DOWN) {
        cursor_old.x = cursor.x;
        cursor_old.y = cursor.y;
        cursor_old.z = cursor.z;
    }
    
    if (fire==ON_RELEASE && cursor_old.z==cursor.z && cursor_old.y == cursor_old.y && cursor_old.x==cursor.x) {
        sensed_pixel = cursor;
        return true;
    }
    
    if (fire == DOWN) {
        
        if (up==ON_DOWN) {
            cursor.z = (cursor.z+1) % 3;
        }
        if (down==ON_DOWN) {
            cursor.z = (cursor.z>0 ? cursor.z-1 : 2);
        }
    }
    else if (fire==UP) {
        if (up==ON_DOWN) {
            cursor.y = (cursor.y+1) % 3;
        }
        if (down==ON_DOWN) {
            cursor.y = (cursor.y>0 ? cursor.y-1 : 2);
        }
        if (left==ON_DOWN) {
            cursor.x = (cursor.x+1) % 3;
        }
        if (right==ON_DOWN) {
            cursor.x = (cursor.x>0 ? cursor.x-1 : 2);
        }
    }
    if (cube[cursor.z][cursor.y][cursor.x] < 3) {
        cube[cursor.z][cursor.y][cursor.x] = current_player + 2 + cube[cursor.z][cursor.y][cursor.x]*2;
    }
    
    if (cursor.x != prev_pos.x && cursor.y != prev_pos.y && cursor.z != prev_pos.z) {
        cube[prev_pos.z][prev_pos.y][prev_pos.x] = (cube[prev_pos.z][prev_pos.y][prev_pos.x]-3) >> 1;
    }
    return false;
}

/********************************************************************
 * Function:        LED_Cube(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will not yet work.          
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
                cursor.x= cursor_old.x=1;
                cursor.y= cursor_old.y=1;
                cursor.z= cursor_old.z=0;
                
                current_state = WAIT_FOR_LED_EVENT_FSM;
            }
            break;

        case WAIT_FOR_LED_EVENT_FSM:
            // *** outputs ***

            // *** transitions ***
            if (P_RESET == PUSHED) {
                current_state = RESET;
            }

            if (fsm_joystick_ctrl() == true) {
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
