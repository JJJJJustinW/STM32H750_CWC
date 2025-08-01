//
// Created by Penta on 2025/8/1.
//

#ifndef MODE4_H
#define MODE4_H

#include "Serial.h"
#include "tim.h"
#include "adc.h"

/*-*The SET size of buffer array for each half of DMA*-*/
#define ADC_DMA_BUFSIZE 1000

/*-*The MAXIMUM size of buffer array for each half of DMA*-*/
#define ADC_DMA_BUFSIZE_HALF_MAX       (2000)
#define ADC_DMA_BUFSIZE_FULL_MAX       (2*ADC_DMA_BUFSIZE_HALF_MAX)


typedef enum {
    ADDA_IRQ_Idle = 0,
    ADDA_IRQ_Half,
    ADDA_IRQ_Full
} ADC_DMA_IrqState;

typedef enum {
    ADDA_STATE_IDLE=0,
    ADDA_STATE_INIT,
    ADDA_STATE_RUNNING,
    ADDA_STATE_HALTED
} ADC_DMA_STATE;


#define ADC_DATA_LENGTH  800   //Sample Cnt 1000(2000/2)

typedef struct {
    float vref;
    float bias;
    uint16_t fullCode;
    uint16_t biasCode;

    TIM_HandleTypeDef *htim;
    uint32_t fTimClk;
    uint16_t timPsc;
    uint16_t timArr;

    ADC_HandleTypeDef *hadc1;
    DAC_HandleTypeDef *hdac1;
    uint16_t           hdac1Ch1;

    uint32_t fsMax;
    uint32_t fs;

    ADC_DMA_IrqState irqState;

    uint16_t bufSize;
    uint32_t adc1RawBuf[ADC_DMA_BUFSIZE_FULL_MAX];
    uint16_t dac1Ch1RawBuf[ADC_DMA_BUFSIZE_FULL_MAX];

    int32_t adc1Data;
    int32_t dac1Ch1Data;

    void (*realTimeFunc)(void);
    void (*halfTxfrFunc)(void);
    void (*fullTxfrFunc)(void);

} ADC_DMA_HandleTypeDef;




extern uint32_t g_adc1_dma_data1[ADC_DATA_LENGTH];



extern ADC_DMA_HandleTypeDef hadda;

// void ADC_DMA_Output(void);

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
);


void ADC_DMA_SetSamplingFreq(ADC_DMA_HandleTypeDef *hadda, uint32_t freq);
void ADC_DMA_Start(ADC_DMA_HandleTypeDef *hadda);
void ADC_DMA_Stop(ADC_DMA_HandleTypeDef *hadda);
void ADC_DMA_Main(ADC_DMA_HandleTypeDef *hadda);


void ADC_DMA_RTFunc(void);
void ADC_DMA_FullTxFunc(void);
void ADC_DMA_HalfTxFunc(void);




#endif //MODE4_H
