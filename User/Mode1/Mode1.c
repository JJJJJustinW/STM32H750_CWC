//
// Created by Penta on 2025/7/31.
//

#include "Serial.h"
#include "Mode1.h"

uint32_t m1_freq=0;

void Mode1_FreqSel(const uint8_t *input) {
    m1_freq = (input[1] << 0) + (input[2]<<8) + (input[3]<<16) + (input[4]<<24);
}