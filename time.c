#include "time.h"
#include <pic18.h>

extern volatile time_t time__;

time_t time() {
    return time__;
}

bool blink() {
    return (time__ % BLINK_PERIODE) > (BLINK_PERIODE >> 1);
}

void wait(time_t t) {
    time_t o = time__;
    while (o+t > time__) {
        __nop();
    }
}
