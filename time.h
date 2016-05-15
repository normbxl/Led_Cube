/* 
 * File:   time.h
 * Author: norman
 *
 * Created on 3. Mai 2016, 00:25
 */

#ifndef TIME_H
#define	TIME_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef unsigned long time_t;
    // extern time_t __time;
    
#define TIME time__
#define BLINK_PERIODE 300
time_t time();

void wait(time_t t);
bool blink();

#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

