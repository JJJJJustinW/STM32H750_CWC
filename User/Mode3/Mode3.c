//
// Created by Penta on 2025/7/31.
//
#include "Serial.h"
#include "Mode3.h"
#include "math.h"


uint32_t m3_freq = 0;
double m3_mag = 0;
double m3_amp = 10.2;
uint16_t m3_DDS_Out = 0;
uint16_t m3_dig_to_analog = 1900;


void Mode3_FreqMagSel(const uint8_t *input) {
    m3_freq = (input[1] << 0) + (input[2] << 8) + (input[3] << 16) + (input[4] << 24);
    uint8_t tgt_mag = input[5];
    switch (m3_freq) {
        case 100:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 46.36364);
            break;
        case 200:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.6)*0.00138)*tgt_mag / 10.0 * 46.08696);
            break;
        case 300:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.6)*0.00135)*tgt_mag / 10.0 * 50.90909);
            break;
        case 400:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.6)*0.00132)*tgt_mag / 10.0 * 50.0);
            break;
        case 500:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.62)*0.00131)*tgt_mag / 10.0 * 54.16667);
            break;
        case 600:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.7)*0.0013)*tgt_mag / 10.0 * 59.166667);
            break;
        case 700:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.72)*0.0013)*tgt_mag / 10.0 * 66.95652);
            break;
        case 800:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.74)*0.0013)*tgt_mag / 10.0 * 71.79487);
            break;
        case 900:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.77)*0.0013)*tgt_mag / 10.0 * 75.83333);
            break;
        case 1000:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.8)*0.0013)*tgt_mag / 10.0 * 83.89831);
            break;
        case 1100:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.83)*0.0014)*tgt_mag / 10.0 * 89.16667);
            break;
        case 1200:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.86)*0.0014)*tgt_mag / 10.0 * 97.45763);
            break;
        case 1300:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.88)*0.0014)*tgt_mag / 10.0 * 106.9565);
            break;
        case 1400:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.9)*0.0014)*tgt_mag / 10.0 * 111.8644);
            break;
        case 1500:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 117.5);
            break;
        case 1600:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 125.8333);
            break;
        case 1700:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 133.3333);
            break;
        case 1800:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 142.8571);
            break;
        case 1900:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 155.1724);
            break;
        case 2000:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 160.5042);
            break;
        case 2100:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 178.7611);
            break;
        case 2200:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 177.5);
            break;
        case 2300:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 186.6667);
            break;
        case 2400:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 195.0413);
            break;
        case 2500:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 203.2787);
            break;
        case 2600:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 208.0);
            break;
        case 2700:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 221.9512);
            break;
        case 2800:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 229.8387);
            break;
        case 2900:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 237.3016);
            break;
        case 3000:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 251.6129);
            break;
        default:
            m3_mag = ((10 - (2 - tgt_mag / 10.0) * (1.02 - 0.05 * pow((1.5 - m3_freq / 1000.0), 2))) * 0.1) * tgt_mag /
                     10.0 /
                     m3_amp / 5 * (sqrt(
                         (pow(10, -16)) * (pow(m3_freq * 2 * 3.141592653589793238, 4)) + 7 * (pow(10, -8)) * (pow(
                             m3_freq * 3.14159653589793238 * 2, 2)) + 1));
            m3_DDS_Out = (uint16_t) round(m3_mag * m3_dig_to_analog);
    }
    // m3_mag = ((10 - (2 - tgt_mag / 10.0) * (1.06 - 0.05 * pow((1.5 - m3_freq / 1000.0), 2))) * 0.1) * tgt_mag / 10.0 /
    //          m3_amp / 5 * (sqrt(
    //              (pow(10, -16)) * (pow(m3_freq * 2 * 3.141592653589793238, 4)) + 7 * (pow(10, -8)) * (pow(
    //                  m3_freq * 3.14159653589793238 * 2, 2)) + 1));
}
