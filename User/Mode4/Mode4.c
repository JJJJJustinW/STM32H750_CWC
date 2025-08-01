//
// Created by Penta on 2025/8/1.
//
#include "Mode4.h"
#include <string.h>

#include "delay.h"
#include "custom_logger.h"

ADC_DMA_HandleTypeDef hadda;

uint16_t ADC_DATA_LENGTH_HLF=ADC_DATA_LENGTH/2;

void ADC_DMA_Init(
    ADC_DMA_HandleTypeDef* hadda,
    float vref,
    float bias,
    uint16_t fullCode,
    uint16_t biasCode,
    TIM_HandleTypeDef *htim,
    uint32_t fTimClk,
    uint16_t timPsc,
    ADC_HandleTypeDef *hadc1,
    DAC_HandleTypeDef *hdac1,
    uint16_t           hdac1Ch1,
    uint32_t fsMax,
    uint16_t bufSize,
    void (*realTimeFunc)(void),
    void (*halfTxfrFunc)(void),
    void (*fullTxfrFunc)(void)
) {
    hadda->vref = vref;
    hadda->bias = bias;
    hadda->fullCode = fullCode;
    hadda->biasCode = biasCode;
    hadda->htim = htim;
    hadda->fTimClk = fTimClk;
    hadda->timPsc = timPsc;
    hadda->timArr = 0;
    hadda->hadc1 = hadc1;
    hadda->hdac1 = hdac1;
    hadda->hdac1Ch1 = hdac1Ch1;
    hadda->fsMax = fsMax;
    hadda->fs = fsMax;
    hadda->irqState = ADDA_IRQ_Idle;
    hadda->bufSize = bufSize;
    memset(hadda->adc1RawBuf, 0, sizeof(hadda->adc1RawBuf));
    memset(hadda->dac1Ch1RawBuf, 0, sizeof(hadda->dac1Ch1RawBuf));
    hadda->adc1Data = 0;
    hadda->dac1Ch1Data = 0;
    hadda->realTimeFunc = realTimeFunc;
    hadda->halfTxfrFunc = halfTxfrFunc;
    hadda->fullTxfrFunc = fullTxfrFunc;
}

void ADC_DMA_Start(ADC_DMA_HandleTypeDef *hadda) {
    uint16_t size = 2*(hadda->bufSize);
    delay_ms(100);
    HAL_ADC_Start(hadda->hadc1);
    // HAL_ADCEx_MultiModeStart_DMA(hadda->hadc1,       (uint32_t *)(hadda->adcDualRawBuf), size);//Original for differential-ended output
    HAL_ADC_Start_DMA(hadda->hadc1,(uint32_t *)(hadda->adc1RawBuf),size);
    HAL_DAC_Start_DMA(hadda->hdac1, hadda->hdac1Ch1,  (uint32_t *)(hadda->dac1Ch1RawBuf), size, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(hadda->htim);
}

void ADC_DMA_Stop(ADC_DMA_HandleTypeDef *hadda) {
    HAL_TIM_Base_Stop(hadda->htim);
    HAL_ADCEx_MultiModeStop_DMA(hadda->hadc1);
    HAL_ADC_Stop_DMA(hadda->hadc1);
    HAL_DAC_Stop_DMA(hadda->hdac1, hadda->hdac1Ch1);
}


void ADC_DMA_SetSamplingFreq(ADC_DMA_HandleTypeDef *hadda, uint32_t freq) {
    uint16_t dataTemp;

    if (freq < 1U) {
        freq = 1U;
    } else if (freq > (hadda->fsMax)) {
        freq = hadda->fsMax;
    }

    dataTemp = (hadda->fTimClk) / (hadda->timPsc) / freq;
    hadda->htim->Instance->ARR = dataTemp - 1;
    hadda->fs = freq;

}



void ADC_DMA_Main(ADC_DMA_HandleTypeDef *hadda) {
    uint16_t i;
    uint16_t size = hadda->bufSize;
    uint16_t uintTemp1;
    int32_t intTemp1;

    switch (hadda->irqState) {
        case ADDA_IRQ_Idle:
            hadda->irqState = ADDA_IRQ_Idle;
            __NOP();
            break;
        case ADDA_IRQ_Half:
            hadda->irqState = ADDA_IRQ_Idle;
            for (i = 0; i < size; i++) {
                uintTemp1 = (uint16_t)(0x0000FFFF &  (hadda->adc1RawBuf[i]));
                uintTemp1 = uintTemp1 << 4;
                hadda->adc1Data = ((int32_t)uintTemp1 - 32768);
                hadda->realTimeFunc();
                intTemp1 = hadda->dac1Ch1Data;
                hadda->dac1Ch1RawBuf[i] = (uint16_t)((intTemp1 + 32768)/16);
            }
            hadda->halfTxfrFunc();
            break;
        case ADDA_IRQ_Full:
            hadda->irqState = ADDA_IRQ_Idle;
            for (i = size; i < 2*size; i++) {
                uintTemp1 = (uint16_t)(0x0000FFFF &  (hadda->adc1RawBuf[i]));
                uintTemp1 = uintTemp1 << 4;
                hadda->adc1Data = ((int32_t)uintTemp1 - 32768);
                hadda->realTimeFunc();
                intTemp1 = hadda->dac1Ch1Data;
                hadda->dac1Ch1RawBuf[i] = (uint16_t)((intTemp1 + 32768)/16);
            }
            hadda->fullTxfrFunc();
            break;
        default:
            hadda->irqState = ADDA_IRQ_Idle;
            __NOP();
            break;
    }

}

/*-*----Real-Time Function(Run each item in the DMA array)----*-*/
void ADC_DMA_RTFunc(void) {
    __NOP();
}

/*-*----Full Transfer Function(Run at the end of Full IRQ)----*-*/
void ADC_DMA_FullTxFunc(void) {
    CUSTOM_LOG_V(V_INFO,"DMA FULL TX\r\n");
}

/*-*----Half Transfer Function(Run at the end of Half IRQ)----*-*/
void ADC_DMA_HalfTxFunc(void) {
    CUSTOM_LOG_V(V_INFO,"DMA HALF TX\r\n");
}


// void ADC_DMA_Output(ADC_DMA_HandleTypeDef *hadda)
// {
//     //Serial_printf("ADCDMA\r\n");
//     if(g_adc1_dma_complete_flag == 1)//part1 of the data
//     {
//         //Serial_printf("dma_f1\r\n");
//
//
//         for(uint32_t itor_dma=0;itor_dma<ADC_DATA_LENGTH_HLF;itor_dma++)
//         {
//             Serial_printf_t(huart_debug,"%d ",g_adc1_dma_data1[itor_dma]&0x0000ffff);//
//         }
//
//         Serial_printf_t(huart_screen,"\r\n\r\n");
//
//         g_adc1_dma_complete_flag = 0;
//         memset(&g_adc1_dma_data1[0],0,ADC_DATA_LENGTH/2);//Clear the stored data
//     }
//     if(g_adc1_dma_complete_flag == 2)//part2
//     {
//         //Serial_printf("dma_f2\r\n");
//
//
//         for(uint32_t itor_dma=ADC_DATA_LENGTH_HLF;itor_dma<ADC_DATA_LENGTH;itor_dma++)
//         {
//             Serial_printf_t(huart_debug,"%d ",g_adc1_dma_data1[itor_dma]&0x0000ffff);//
//         }
//
//         Serial_printf_t(huart_screen,"\r\n\r\n");
//
//         g_adc1_dma_complete_flag = 0;
//         memset(&g_adc1_dma_data1[ADC_DATA_LENGTH_HLF],0,ADC_DATA_LENGTH/2);//Clear the stored data
//     }
// }


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==hadda.hadc1->Instance)
    {
        //Serial_printf("convcplt\r\n");
        hadda.irqState=ADDA_IRQ_Full;
    }
}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==hadda.hadc1->Instance)
    {
        //Serial_printf("halfconv\r\n");
        hadda.irqState=ADDA_IRQ_Half;
    }
}

