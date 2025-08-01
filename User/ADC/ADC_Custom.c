//
// Created by Penta on 2025/7/25.
//
#include <stdio.h>

#include "string.h"

#include "ADC_Custom.h"

#include "Serial.h"

uint32_t g_adc1_dma_data1[ADC_DATA_LENGTH];//Store the data from adc1
uint8_t g_adc1_dma_complete_flag = 0;//dma cplt flag, controlled in dma_conv_cplt_callback




void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==ADC1)
    {
        //Serial_printf("convcplt\r\n");
        g_adc1_dma_complete_flag = 2;
    }
}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==ADC1)
    {
        //Serial_printf("halfconv\r\n");
        g_adc1_dma_complete_flag = 1;
    }
}



