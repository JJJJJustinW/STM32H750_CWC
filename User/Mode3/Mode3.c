//
// Created by Penta on 2025/7/31.
//
#include "Serial.h"
#include "Mode3.h"

uint32_t m3_freq=0;
uint8_t m3_mag=0;

void Mode3_FreqMagSel(const uint8_t *input) {
    m3_freq = (input[1] << 0) + (input[2]<<8) + (input[3]<<16) + (input[4]<<24);
    m3_mag=input[5]<<0;
}