//
// Created by Penta on 2025/8/2.
//

#ifndef FTYPE_H
#define FTYPE_H

#include "math.h"
#include <stdio.h>
#include <stdint.h>

typedef enum {
    FILTER_LOW_PASS = 0,
    FILTER_HIGH_PASS = 1,
    FILTER_BAND_PASS = 2,
    FILTER_BAND_STOP = 3,
    FILTER_UNKNOWN=4
} FilterType;

FilterType FilterAnalysis(float *Amplitude, float *Phase, uint32_t N);



#endif //FTYPE_H
