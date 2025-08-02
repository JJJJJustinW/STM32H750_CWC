#include "scan.h"
#include <stdio.h>
#include "AD9959.h"
#include "adc.h"
#include "Serial.h"
#include "custom_logger.h"

//Mode4 State Machine initialize
ADC_DMA_STATE AD_State = ADDA_STATE_IDLE;

// === Sweep parameter table ===
const SweepSegment sweep_table[] = {
    {100,     1000,     100,  0.4f, 0.2f},
    {1000,    10000,    100,  0.4f, 0.2f},
    {10000,   100000,   100,  0.4f, 0.2f},
    {100000,  1000000,  100,  0.4f, 0.2f}
};
const int sweep_segment_count = sizeof(sweep_table) / sizeof(SweepSegment);

// === Global variables ===
SweepState sweep_state = SWEEP_STATE_IDLE;
uint32_t sweep_tick = 0;
uint32_t current_freq = 0;
uint32_t current_segment = 0;
uint16_t adc_buffer[MAX_ADC_BUF_SIZE];
uint16_t adc2_buffer[MAX_ADC_BUF_SIZE];
uint32_t adc_sample_count = 0;
volatile uint8_t adc1_done = 0;
volatile uint8_t adc2_done = 0;

float sweep_result_buffer[MAX_SWEEP_POINTS];   
uint32_t sweep_freq_buffer[MAX_SWEEP_POINTS];   
float sweep_result_buffer_adc2[MAX_SWEEP_POINTS];   
uint32_t sweep_freq_buffer_adc2[MAX_SWEEP_POINTS];   
uint32_t sweep_result_index = 0; 
// External ADC handle (defined in main file)
extern ADC_HandleTypeDef hadc1;

// === Start sweep ===
void Sweep_Start(void)
{
    current_segment = 0;
    current_freq = sweep_table[0].start_freq;
  AD9959_SetFrequency4Channel(current_freq,0,0,0);
    IO_Update();
	 
 //CUSTOM_LOG_V(V_INFO, "===========2==========\r\n");
    sweep_tick = 0;
    sweep_state = SWEEP_STATE_WAIT;
}

// === Timer interrupt callback (called every 1us) ===
void Sweep_TIM_IRQHandler(void)
{
    if (sweep_state != SWEEP_STATE_WAIT) return;

    sweep_tick++;

    float T = 1.0f / current_freq;
    float delay_time = sweep_table[current_segment].delay_ratio * T;
    uint32_t delay_us = (uint32_t)(delay_time * 1e6f);

    if (sweep_tick >= delay_us) {

        float sample_time = sweep_table[current_segment].sample_ratio * T;
        adc_sample_count = (uint32_t)(sample_time * ADC_SAMPLING_FREQ);
			if (adc_sample_count == 0) adc_sample_count = 10;
        if (adc_sample_count > MAX_ADC_BUF_SIZE) adc_sample_count = MAX_ADC_BUF_SIZE;
    //  CUSTOM_LOG_V(V_INFO, "===========startADC==========\r\n");
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, adc_sample_count);
			HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_buffer, adc_sample_count);
        sweep_state = SWEEP_STATE_SAMPLING;
    }
				// CUSTOM_LOG_V(V_INFO, "===========tim_callback_gone==========\r\n");
}



//void Sweep_PrintResults(void)
//{
//    Serial_printf("\r\n=== Sweep Results (%lu points) ===\r\n", sweep_result_index);
//    for (uint32_t i = 0; i < sweep_result_index; i++) {
//        Serial_printf("Freq: %lu Hz\tAvg ADC: %.2f\r\n", sweep_freq_buffer[i], sweep_result_buffer[i]);
//    }
//}
void Sweep_PrintResults(void)
{
    Serial_printf("\r\n=== Sweep Results (%lu points) ===\r\n", sweep_result_index);
    for (uint32_t i = 0; i < sweep_result_index; i++) {
        Serial_printf("Freq: %lu Hz\tADC1: %.3f V\tADC2: %.3f V\r\n",
            sweep_freq_buffer[i],
            sweep_result_buffer[i],
            sweep_result_buffer_adc2[i]);
    }
}

//void Sweep_ADC_ConvCpltCallback(void)
//{
//    if (sweep_state != SWEEP_STATE_SAMPLING) return;
//    uint64_t sum = 0;
//    for (uint32_t i = 0; i < adc_sample_count; i++) {
//        sum += adc_buffer[i];
//    }
//    float avg = (float)sum / adc_sample_count;
//    float avg_voltage = (avg / 65535.0f) * 3.3f; 
//    if (sweep_result_index < MAX_SWEEP_POINTS) {
//        sweep_result_buffer[sweep_result_index] = avg;
//        sweep_freq_buffer[sweep_result_index] = current_freq;
//        sweep_result_index++;
//    }

//    Serial_printf("Sweep ADC done: freq = %lu Hz\n", current_freq);

//    current_freq += sweep_table[current_segment].step;
//    if (current_freq > sweep_table[current_segment].end_freq) {
//        current_segment++;
//        if (current_segment >= sweep_segment_count) {
//            sweep_state = SWEEP_STATE_DONE;
//					Sweep_PrintResults();
//            return;
//        }
//        current_freq = sweep_table[current_segment].start_freq;
//    }

//    Write_frequence(0, current_freq);
//    IO_Update();

//    sweep_tick = 0;
//    sweep_state = SWEEP_STATE_WAIT;
//}
void Sweep_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (sweep_state != SWEEP_STATE_SAMPLING) return;

    if (hadc->Instance == ADC1) {
        // === ADC1  ===
        uint64_t sum = 0;
        for (uint32_t i = 0; i < adc_sample_count; i++) {
            sum += adc_buffer[i];
        }
        float avg_adc = (float)sum / adc_sample_count;
        float voltage = (avg_adc / 65535.0f) * 3.3f;
        if (sweep_result_index < MAX_SWEEP_POINTS) {
            sweep_result_buffer[sweep_result_index] = voltage;
            sweep_freq_buffer[sweep_result_index] = current_freq;
        }
        adc1_done = 1;
    }

    else if (hadc->Instance == ADC2) {
        // === ADC2 ===
        uint64_t sum2 = 0;
        for (uint32_t i = 0; i < adc_sample_count; i++) {
            sum2 += adc2_buffer[i];
        }
        float avg_adc2 = (float)sum2 / adc_sample_count;
        float voltage2 = (avg_adc2 / 65535.0f) * 3.3f;
        if (sweep_result_index < MAX_SWEEP_POINTS) {
            sweep_result_buffer_adc2[sweep_result_index] = voltage2;
            sweep_freq_buffer_adc2[sweep_result_index] = current_freq;
        }
        adc2_done = 1;
    }

    if (adc1_done && adc2_done) {
        adc1_done = 0;
        adc2_done = 0;

        sweep_result_index++;
//       Serial_printf("Sweep ADC done: freq = %lu Hz\n", current_freq);

        current_freq += sweep_table[current_segment].step;
        if (current_freq > sweep_table[current_segment].end_freq) {
            current_segment++;
            if (current_segment >= sweep_segment_count) {
                sweep_state = SWEEP_STATE_DONE;
                Sweep_PrintResults();
                return;
            }
            current_freq = sweep_table[current_segment].start_freq;
        }

         AD9959_SetFrequency4Channel(current_freq,0,0,0);
        IO_Update();
        sweep_tick = 0;
        sweep_state = SWEEP_STATE_WAIT;
    }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4) {
        // CUSTOM_LOG_V(V_INFO, "===========tim4==========\r\n");
        Sweep_TIM_IRQHandler();  // 1us ????????
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    Sweep_ADC_ConvCpltCallback(hadc);

}


