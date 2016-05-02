#include "time.h"

unsigned long get_time() {
    return __time;
}

void wait(unsigned long t) {
    unsigned long o = __time;
    while (o+t > __time) {
        NOP();
    }
}
