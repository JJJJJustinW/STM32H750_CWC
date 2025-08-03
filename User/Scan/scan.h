// scan.h
#ifndef __SCAN_H
#define __SCAN_H

#include "main.h"

// Maximum size of ADC DMA buffer
#define MAX_ADC_BUF_SIZE     200

// ADC sampling frequency (samples per second)
#define ADC_SAMPLING_FREQ    1000000.0f  // 1Msps

#define MAX_SWEEP_POINTS 10000  // ?????????,?????????


// Mode4 State Machine
typedef enum {
    ADDA_STATE_IDLE=0,
    ADDA_STATE_INIT,
    ADDA_STATE_RUNNING,
    ADDA_STATE_HALTED
} ADC_DMA_STATE;

extern ADC_DMA_STATE AD_State;

// Sweep state machine
typedef enum {
    SWEEP_STATE_IDLE = 0,      // Not running
    SWEEP_STATE_WAIT,          // Waiting for delay before sampling
    SWEEP_STATE_SAMPLING,      // ADC sampling in progress
    SWEEP_STATE_DONE           // Sweep finished
} SweepState;

// Parameters for one sweep segment
typedef struct {
    uint32_t start_freq;       // Start frequency in Hz
    uint32_t end_freq;         // End frequency in Hz
    uint32_t step;             // Frequency step in Hz
    float delay_ratio;         // Time before sampling (as fraction of T)
    float sample_ratio;        // Sampling duration (as fraction of T)
} SweepSegment;


extern float sweep_result_buffer[MAX_SWEEP_POINTS];
extern uint32_t sweep_freq_buffer[MAX_SWEEP_POINTS];
extern float sweep_result_buffer_adc2[MAX_SWEEP_POINTS];
extern uint32_t sweep_freq_buffer_adc2[MAX_SWEEP_POINTS];


// === Sweep control API ===

// Start the entire frequency sweep
void Sweep_Start(void);

// Called every 1us by timer ISR to manage sweep timing
void Sweep_TIM_IRQHandler(void);

// Called when ADC DMA transfer completes
void Sweep_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

// Optional: user-defined callback to process result per frequency
void Process_Sweep_Result(uint32_t freq_hz, float avg_val);



#endif  // __SCAN_H
