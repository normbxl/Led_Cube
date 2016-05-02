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

volatile unsigned long __time = 0;

unsigned long get_time();

void wait(unsigned long t);


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

