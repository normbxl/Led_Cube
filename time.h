/* 
 * File:   time.h
 * Author: norman
 *
 * Created on 3. Mai 2016, 00:25
 */

#ifndef TIME_H
#define	TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef unsigned long time_t;
    // extern time_t __time;
    
#define TIME time__
time_t time();

void wait(time_t t);


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

