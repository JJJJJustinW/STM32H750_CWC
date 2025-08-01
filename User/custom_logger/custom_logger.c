//
// Created by guestsh1 on 2025/7/31.
//
#include "custom_logger.h"
#include "Serial.h"
#include <string.h>
#include <time.h>

static clock_t t0 = 0;

static void print_time() {
    if (t0 == 0) {
        t0 = clock();
    }
    clock_t t1 = clock() - t0;
    int t2 = (int) (t1 / CLOCKS_PER_SEC);
    int t3 = (int) (t1 % CLOCKS_PER_SEC);
    Serial_printf("[% 5d.%02d] ", t2, t3);
}

static void print_log_verbosity_name(int verbosity) {
    if (verbosity <= V_ERROR) {
        Serial_SendStr("[ERROR] ");
        return;
    }
    if (verbosity <= V_WARN) {
        Serial_SendStr("[WARN ] ");
        return;
    }
    if (verbosity <= V_INFO) {
        Serial_SendStr("[INFO ] ");
        return;
    }
    if (verbosity <= V_DEBUG) {
        Serial_SendStr("[DEBUG] ");
        return;
    }
    Serial_SendStr("[TRACE] ");
}

static void print_file_name_and_line_number(const char *file_name, int line_number) {
    // Print file name
    Serial_SendByte('[');
    size_t file_name_len = strlen(file_name);
    if (file_name_len <= FILE_NAME_CUTOFF) {
        // Case 1: file name shorter than limit --> prepend spaces
        size_t prepend_spaces = FILE_NAME_CUTOFF - file_name_len;
        for (size_t i = 0; i < prepend_spaces; i++) {
            Serial_SendByte(' ');
        }
        Serial_SendStr(file_name);
    } else {
        // Case 2: file name longer than limit --> cutoff
        const char *file_name_begin = &file_name[file_name_len - FILE_NAME_CUTOFF + 3];
        Serial_SendStr("...");
        Serial_SendStr(file_name_begin);
    }
    Serial_SendByte(':');

    // Print line number
    Serial_printf("% 4d", line_number);
    Serial_SendStr("] ");
}

void print_log_header(const char *file_name, int line_number, int verbosity) {
//    print_time();
    print_file_name_and_line_number(file_name, line_number);
    print_log_verbosity_name(verbosity);
}