#include "button.h"

button_state button_check(byte val, button_t *b) {
    if (!b->initialized) {
        b->state = val==HIGH ? DOWN : UP;
        b->initialized=true;
    }
    else {
        if (val==HIGH) {
            switch(b->state) {
                case ON_DOWN:
                    b->state=DOWN;
                    break;
                case UP:
                    b->state=ON_DOWN;
                    break;
                default:
                    b->state=DOWN;
                    break;
            }
        }
        else {
            switch(b->state) {
                case ON_DOWN:
                    b->state=ON_RELEASE;
                    break;
                case DOWN:
                    b->state=ON_RELEASE;
                    break;
                default:
                    b->state=UP;
                    break;
            }
        }
    }
    return b->state;
}
