/* 
 * File:   detector.h
 * Author: norman
 *
 * Created on 26. April 2016, 15:32
 */

#include "config.h"

#ifndef DETECTOR_H
#define	DETECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    bool detector_check(byte layer, pixel3_t *pos);
    

#ifdef	__cplusplus
}
#endif

#endif	/* DETECTOR_H */

