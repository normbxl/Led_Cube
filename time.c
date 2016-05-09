#include "time.h"

time_t get_time() {
    return __time;
}

void wait(time_t t) {
    time_t o = __time;
    while (o+t > __time) {
        NOP();
    }
}
