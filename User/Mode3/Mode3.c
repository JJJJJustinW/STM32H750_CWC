//
// Created by Penta on 2025/7/31.
//
#include "Serial.h"
#include "Mode3.h"


uint32_t m3_freq=0;
double m3_mag=0;
double m3_amp=10.4;
uint16_t m3_DDS_Out=0;
uint16_t m3_dig_to_analog=1;


void Mode3_FreqMagSel(const uint8_t *input) {
    m3_freq = (input[1] << 0) + (input[2]<<8) + (input[3]<<16) + (input[4]<<24);
    uint8_t tgt_mag=input[5];
    m3_mag= tgt_mag/10.0/m3_amp/5*(sqrt((pow(10,-16))*(pow(m3_freq*2*3.14159265359,4))+7*(pow(10,-8))*(pow(m3_freq*3.1415965359*2,2))+1));
    m3_DDS_Out=m3_mag*m3_dig_to_analog;
}