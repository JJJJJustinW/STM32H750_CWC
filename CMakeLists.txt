cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32H750_CWC)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMSISCORE "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/")
add_subdirectory(CMSIS-DSP-1.16.2/Source bin_dsp)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
#        User/ADC/ADC_Custom.c
#        User/ADC/ADC_Custom.h
        User/Mode1/Mode1.c
        User/Mode1/Mode1.h
        User/DDS/AD9959.c
        User/DDS/AD9959.h
        User/Mode3/Mode3.c
        User/Mode3/Mode3.h
        User/Mode4/Mode4.c
        User/Mode4/Mode4.h
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
        CMSIS-DSP-1.16.2
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
#        User/ADC/ADC_Custom.c
        User/Serial/Serial.c
        User/Delay/delay.c
        User/IIC/myiic.c
        User/DoubleKeys/Key2.c
        User/DDS/AD9959.c
        User/custom_logger/custom_logger.c
        User/Mode1/Mode1.c
        User/Mode3/Mode3.c
        User/Mode4/Mode4.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
#        User/ADC
        User/Serial
        User/Delay
        User/IIC
        User/DoubleKeys
        User/DDS
        User/custom_logger
        User/Mode1
        User/Mode3
        User/Mode4
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
