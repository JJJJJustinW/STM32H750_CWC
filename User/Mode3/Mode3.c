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
    uint8_t tgt_mag = input[5]-10;
    double tangent=2.6;
    double t1=1.2;
    switch (m3_freq) {
        case 100:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.4)*0.0014)*tgt_mag / 10.0 * 46.36364)+tangent+t1;
            break;
        case 200:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.6)*0.00138)*tgt_mag / 10.0 * 45.98696)+tangent-0.5;
            break;
        case 300:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-17.9)*0.00137)*tgt_mag / 10.0 * 48.0)+tangent;
            break;
        case 400:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-17.9)*0.00137)*tgt_mag / 10.0 * 50.2)+tangent;
            break;
        case 500:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-18.3)*0.00131)*tgt_mag / 10.0 * 54.16667)+tangent;
            break;
        case 600:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.6)*0.0013)*tgt_mag / 10.0 * 59.166667)+tangent;
            break;
        case 700:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.72)*0.0013)*tgt_mag / 10.0 * 66.95652)+tangent;
            break;
        case 800:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.74)*0.0013)*tgt_mag / 10.0 * 71.79487)+tangent;
            break;
        case 900:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.77)*0.0013)*tgt_mag / 10.0 * 76.33333)+tangent;
            break;
        case 1000:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.8)*0.0013)*tgt_mag / 10.0 * 82.59831)+tangent;
            break;
        case 1100:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.73)*0.0013)*tgt_mag / 10.0 * 89.16667)+tangent;
            break;
        case 1200:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.86)*0.0013)*tgt_mag / 10.0 * 97.45763)+tangent;
            break;
        case 1300:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.88)*0.0013)*tgt_mag / 10.0 * 105.3565)+tangent;
            break;
        case 1400:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.9)*0.0013)*tgt_mag / 10.0 * 111.8644)+tangent;
            break;
        case 1500:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.2)*0.00145)*tgt_mag / 10.0 * 117.5)+tangent;
            break;
        case 1600:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 125.8333)+tangent;
            break;
        case 1700:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 133.3333)+tangent;
            break;
        case 1800:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 141.5571)+tangent;
            break;
        case 1900:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 150.0724)+tangent;
            break;
        case 2000:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-15.5)*0.0014)*tgt_mag / 10.0 * 157.5042)+tangent;
            break;
        case 2100:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 165.7611)+tangent;
            break;
        case 2200:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 174.5)+tangent;
            break;
        case 2300:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 181.6667)+tangent;
            break;
        case 2400:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.0)*0.0014)*tgt_mag / 10.0 * 190.0413)+tangent;
            break;
        case 2500:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 199.2787)+tangent;
            break;
        case 2600:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.0)*0.0014)*tgt_mag / 10.0 * 208.0)+tangent;
            break;
        case 2700:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 217.9512)+tangent;
            break;
        case 2800:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 226.8387)+tangent;
            break;
        case 2900:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 235.3016)+tangent;
            break;
        case 3000:
            m3_DDS_Out = (uint16_t) round((1+(tgt_mag-16.5)*0.0014)*tgt_mag / 10.0 * 244.6129)+tangent;
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
