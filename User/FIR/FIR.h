//
// Created by Penta on 2025/8/1.
//
#ifndef FIR_H
#define FIR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

typedef struct {
    int order;
    double *coeffs;
    double *buffer;
    int buf_index;
} FIR_HandleTypeDef;

typedef struct {
    double real;
    double imag;
} Complex;

typedef struct {
    int sample_pts;
    double *frequency;
    double *magnitude;
    double *phase;
} SweepTestCase;






#endif //FIR_H
