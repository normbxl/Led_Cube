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

    #define DETECT_THRESHOLD 30.0

    float old_q[3][3][3];
    float old_v[3][3][3];
    
    const float ALPHA = 0.05;
    const float BETA = 0.5;
  
    pixel2_t rev_lut[9] = {
        {0, 0},{0, 1},{0, 2},
        {1, 2},{2, 2},{2, 1},
        {2, 0},{1, 0},{1, 1}
    };
    
    bool detector_check(byte layer, pixel3_t *pos);
    

#ifdef	__cplusplus
}
#endif

#endif	/* DETECTOR_H */

