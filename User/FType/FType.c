//
// Created by Penta on 2025/8/2.
//
#include "FType.h"

FilterType FilterAnalysis(float *Amplitude, float *Phase, uint32_t N) {
    if (N < 80) return FILTER_UNKNOWN;

    //Split the Frequency Ranges
    // uint32_t low_start = 0;
    // uint32_t low_end   = N / 10;
    // uint32_t mid_start = N / 3;
    // uint32_t mid_end   = mid_start + N / 5;
    // uint32_t high_start = N - N / 10;
    // uint32_t high_end   = N;
    float max_amp=0;
    // uint32_t max_amp_index=0;

    //locate the maximum value
    for (int i=0;i<N;i++) {
        if (Amplitude[i] > max_amp) {
            max_amp = Amplitude[i];
            // max_amp_index = i;
        }
    }
    if ((Amplitude[1]+Amplitude[2])<max_amp*1.5) {
        if ((Amplitude[N-2]+Amplitude[N-1])<max_amp*1.5) {
            return FILTER_BAND_PASS;
        }
        return FILTER_HIGH_PASS;
    }
    if((Amplitude[N-2]+Amplitude[N-1])<max_amp*1.5){return FILTER_LOW_PASS;}
    else return FILTER_BAND_STOP;


    //Average Amp for each range
//     float avg_amp_low = 0, avg_amp_mid = 0, avg_amp_high = 0;
//     for (uint32_t i = low_start; i < low_end; i++) avg_amp_low += Amplitude[i];
//     avg_amp_low /= (low_end - low_start);
//     for (uint32_t i = mid_start; i < mid_end; i++) avg_amp_mid += Amplitude[i];
//     avg_amp_mid /= (mid_end - mid_start);
//     for (uint32_t i = high_start; i < high_end; i++) avg_amp_high += Amplitude[i];
//     avg_amp_high /= (high_end - high_start);
//
//     //Average Phase
//     float avg_phase_low = 0, avg_phase_mid = 0, avg_phase_high = 0;
//     for (uint32_t i = low_start; i < low_end; i++) avg_phase_low += Phase[i];
//     avg_phase_low /= (low_end - low_start);
//     for (uint32_t i = mid_start; i < mid_end; i++) avg_phase_mid += Phase[i];
//     avg_phase_mid /= (mid_end - mid_start);
//     for (uint32_t i = high_start; i < high_end; i++) avg_phase_high += Phase[i];
//     avg_phase_high /= (high_end - high_start);
//
//     //Normalize Amp(Based on the maximum amp region)
//     float max_amp = fmaxf(fmaxf(avg_amp_low, avg_amp_mid), avg_amp_high);
//     float ratio_low = avg_amp_low / max_amp;
//     float ratio_mid = avg_amp_mid / max_amp;
//     float ratio_high = avg_amp_high / max_amp;
//
//     //phase trend(Up or Down for first half and second half)
//     float dphi_low  = avg_phase_mid - avg_phase_low;
//     float dphi_high = avg_phase_high - avg_phase_mid;
//     float avg_phase_lm=(avg_phase_mid+avg_phase_low)/2;
//     float avg_phase_mh=(avg_phase_high+avg_phase_mid)/2;
//     float dphi_mid =avg_phase_mh-avg_phase_mid;
//
//     /* *============= Distinguish =============* */
//     if (ratio_low > 0.7f && ratio_high < 0.3f && dphi_high < -0.5f) {
//         return FILTER_LOW_PASS;
//     } else if (ratio_low < 0.3f && ratio_high > 0.7f && dphi_low < -0.5f) {
//         return FILTER_HIGH_PASS;
//     } else if (ratio_mid > 0.7f && ratio_low < 0.3f && ratio_high < 0.3f &&
//                fabsf(dphi_low) > 0.5f && fabsf(dphi_high) > 0.5f) {
//         return FILTER_BAND_PASS;
//     } else if (ratio_mid < 0.3f && ratio_low > 0.7f && ratio_high > 0.7f &&
//                fabsf(dphi_mid) > 1.0f) {
//         return FILTER_BAND_STOP;
//     } else {
//         return FILTER_UNKNOWN;
//     }
}
