#include "fsm.h"
#include "intelligence.h"
#include "button.h"

#define UART_DEBUG

#ifdef UART_DEBUG
    // #include "serial/system.h"
    #include "serial/serial.h"
#endif

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

bool three_in_a_row;
bool LED_set;
bool winner_determined;



enum 
{
    START_FSM =             0,
    BTN_DWN   =             1,
    WAIT_FOR_LED_EVENT_FSM= 2,
    SET_CHOSEN_LED_FSM  =   3,
    CHECK_3_IN_A_ROW_FSM =  4, 
    GAME_TIE             =  5,
    FLASH_WIN_FSM   =       6,
    RESET     =             7
} current_state, old_state;


pixel3_t cursor;
pixel3_t cursor_old;

button_t btn_up;
button_t btn_down;
button_t btn_left;
button_t btn_right;
button_t btn_fire;



void fsm_init() {
    current_state = START_FSM;
    old_state = START_FSM;
    led_covered = false;
    three_in_a_row = false;
    LED_set = false;
    winner_determined = false;
    winner_color = BLANK;
    
    current_player = GREEN;
#ifdef UART_DEBUG
    Serial_begin(9600);
    Serial_println("-- LED CUBE --");
#endif
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
    pixel3_t prev_pos;
    prev_pos.x = cursor.x;
    prev_pos.y = cursor.y;
    prev_pos.z = cursor.z;
    
#ifdef UART_DEBUG
    if (up==ON_DOWN)
        Serial_println("UP");
    
    if (down==ON_DOWN)
        Serial_println("DOWN");
    
    if (left==ON_DOWN)
        Serial_println("LEFT");
    
    if (right==ON_DOWN)
        Serial_println("RIGHT");
    
    if (fire==ON_DOWN)
        Serial_println("FIRE");
#endif
    
    if (fire == ON_DOWN) {
        cursor_old.x = cursor.x;
        cursor_old.y = cursor.y;
        cursor_old.z = cursor.z;
    }
    
    if (fire==ON_RELEASE && cursor_old.z==cursor.z && cursor_old.y == cursor_old.y && cursor_old.x==cursor.x) {
        color_t col = cube[cursor.z][cursor.y][cursor.x];
        if (col == CURSOR_GREEN_B || col == CURSOR_RED_B) {
            sensed_pixel.x = cursor.x;
            sensed_pixel.y = cursor.y;
            sensed_pixel.z = cursor.z;
#ifdef UART_DEBUG
            Serial_print("pixel set at ");
            Serial_write(cursor.x+48); Serial_print(", ");
            Serial_write(cursor.y+48); Serial_print(", ");
            Serial_write(cursor.z+48); Serial_print("\r\n");
#endif
        }
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
    if (cube[cursor.z][cursor.y][cursor.x] <= RED) {
        cube[cursor.z][cursor.y][cursor.x] = current_player + 2 + cube[cursor.z][cursor.y][cursor.x]*2;
    }
    
    
    if (cursor.x != prev_pos.x || cursor.y != prev_pos.y || cursor.z != prev_pos.z) {
        cube[prev_pos.z][prev_pos.y][prev_pos.x] = (cube[prev_pos.z][prev_pos.y][prev_pos.x]-3) >> 1;
#ifdef UART_DEBUG
        Serial_print("cursor [");
        Serial_write(cube[cursor.z][cursor.y][cursor.x] + 47); Serial_print("] ");
        Serial_write(cursor.x+48); Serial_print(", ");
        Serial_write(cursor.y+48); Serial_print(", ");
        Serial_write(cursor.z+48); Serial_print("\r\n");
#endif
    }
    return false;
}

void set_state(byte state) {
    old_state = current_state;
    current_state = state;
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
            if (P_RESET == PUSHED || button_check(P_FIRE, &btn_fire)==ON_DOWN) {
                set_state(BTN_DWN);
            }
            break;

        case BTN_DWN:
            // *** outputs ***

            // *** transitions ***
            if (P_RESET != PUSHED || button_check(P_FIRE, &btn_fire)==ON_RELEASE) {
                cursor.x= cursor_old.x=1;
                cursor.y= cursor_old.y=1;
                cursor.z= cursor_old.z=0;
                
                set_state(WAIT_FOR_LED_EVENT_FSM);
            }
            break;

        case WAIT_FOR_LED_EVENT_FSM:
            // *** outputs ***

            // *** transitions ***
            if (P_RESET == PUSHED) {
                set_state(RESET);
            }

            if (fsm_joystick_ctrl() == true) {
                set_state(SET_CHOSEN_LED_FSM);
            }
            break;

        case SET_CHOSEN_LED_FSM:
            // *** outputs ***
            cube[sensed_pixel.z][sensed_pixel.y][sensed_pixel.x] = current_player;
            
            // *** transitions ***

            // RESET
            if (P_RESET == PUSHED) {
                set_state(RESET);
            }

            
            set_state(CHECK_3_IN_A_ROW_FSM);
            
            break;

        case CHECK_3_IN_A_ROW_FSM:
            // *** outputs ***
            
            winner_color = three_in_a_row_check();
            
            // *** transitions ***
            if (winner_color == BLANK) {
                current_player = current_player == GREEN ? RED : GREEN;
                if (fsm_is_end_of_game()==true) {
                    set_state(GAME_TIE);
#ifdef UART_DEBUG
                    Serial_println("TIE");
#endif
                }
                else {
                    set_state(WAIT_FOR_LED_EVENT_FSM);
                }
            }
            else {
                set_state(FLASH_WIN_FSM);
                color_t* col;
                for (int c=0; c<27; c++) {
                    col = ((color_t*)(cube) + c);
                    *col = (*col == current_player) ?  current_player + 2 : *col;
                }
#ifdef UART_DEBUG
                Serial_print(current_player==1 ? "GREEN" :"RED" );
                Serial_println(" WINS");
#endif
            }
            break;


        case GAME_TIE:
            // *** outputs ***
            // RESET
            if (P_RESET == PUSHED) {
                set_state(RESET);
            }
            if (button_check(P_FIRE, &btn_fire)==ON_RELEASE) {
                set_state(RESET);
            }
            break;

        case FLASH_WIN_FSM:
            // *** outputs ***
            

            if (P_RESET == PUSHED) {
                set_state(RESET);
            }
            if (button_check(P_FIRE, &btn_fire)==ON_RELEASE) {
                set_state(RESET);
            }
            break;

        case RESET:
            // *** outputs ***
            // erase cube
            for (int c=0; c<27; c++) {
                *((color_t*)(cube) + c) = BLANK;
            }
            cursor.x=1;
            cursor.y=1;
            cursor.z=0;
            // *** transitions ***
            if (P_RESET != PUSHED) {
                set_state(START_FSM);
            }
            break;

        default:
            set_state(START_FSM);
            break;
    }
    if (current_state != old_state) {
        //Serial_print("state changed ");
        Serial_write(old_state+48);
        Serial_print("->");
        Serial_write(current_state+48);
        Serial_print("\r\n");
        old_state=current_state;
    }

}
//EOF-------------------------------------------------------------------------
