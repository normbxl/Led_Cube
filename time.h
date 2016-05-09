/* 
 * File:   time.h
 * Author: norman
 *
 * Created on 3. Mai 2016, 00:25
 */
#include <xc.h>

#ifndef TIME_H
#define	TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef unsigned long time_t;
extern volatile time_t __time;

time_t get_time();

void wait(time_t t);


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

