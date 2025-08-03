//
// Created by Penta on 2025/8/1.
//
#include "FIR.h"


void FIR_Init(
    FIR_HandleTypeDef *fir1,
    FreqNode *freq_domain,
    SweepTestCase *sweep1,
    int filter_order,
    double sample_rate
    ) {
    int length=filter_order+1;
    fir1=(FIR_HandleTypeDef*)malloc(sizeof(FIR_HandleTypeDef));
    fir1->order=filter_order;
    fir1->coeffs=(double*)calloc(length,sizeof(double));
    fir1->buffer=(double*)calloc(length,sizeof(double));
    fir1->buf_index=0;

}

void FIR_Complexize(Complex *FreqResp) {

    /*-*---F_Response in complex form---*-*/
    FreqResp=

    double phase=0;
    // FreqNode *f[];
    // for(int k = 0; k <= length/2; k++){
    //     phase = -freq_domain[k].pha;
    //     f[k][0] = freq_domain[k].amp * cos(phase);
    //     f[k][1] = freq_domain[k].amp * sin(phase);
    // }
}





