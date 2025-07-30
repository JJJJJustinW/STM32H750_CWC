//
// Created by Penta on 2025/7/25.
//

#ifndef ADC_CUSTOM_H
#define ADC_CUSTOM_H

#include "adc.h"

#define ADC_DATA_LENGTH  800   //Sample Cnt 1000(2000/2)


extern uint32_t g_adc1_dma_data1[ADC_DATA_LENGTH];
extern uint8_t g_adc1_dma_complete_flag;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc);


void ADC_DMA_Output(void);

#endif //ADC_CUSTOM_H
