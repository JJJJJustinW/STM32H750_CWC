//
// Created by guestsh1 on 2025/7/31.
//

#ifndef STM32H750_CWC_CUSTOM_LOGGER_H
#define STM32H750_CWC_CUSTOM_LOGGER_H

#include <stdio.h>

/*
 * Begin configuration
 * */

 #define ENABLE_CUSTOM_LOG // Comment this line to disable all logs (no runtime overhead)

#define LOG_VERBOSITY_CUTOFF 4 // Suppress all log with verbosity greater than this value (with runtime overhead)

#define V_ERROR 0
#define V_WARN 1
#define V_INFO 3
#define V_DEBUG 7

#define FILE_NAME_CUTOFF 16 // Max length of the printed file name

/*
 * End configuration
 * */

#ifdef ENABLE_CUSTOM_LOG
#define CUSTOM_LOG_V(verbosity, ...) \
do{if(verbosity<=LOG_VERBOSITY_CUTOFF){print_log_header(__FILE__, __LINE__, verbosity);Serial_printf(__VA_ARGS__);}}while(0)

#define CUSTOM_LOG(...) \
do{print_log_header(__FILE__, __LINE__, V_INFO);Serial_printf(__VA_ARGS__);}while(0)
#else /*ENABLE_CUSTOM_LOG*/
#define CUSTOM_LOG_V(verbosity, ...) do{}while(0);
#define CUSTOM_LOG(...) do{}while(0);
#endif /*ENABLE_CUSTOM_LOG*/

void print_log_header(const char *file_name, int line_number, int verbosity);

#endif //STM32H750_CWC_CUSTOM_LOGGER_H
