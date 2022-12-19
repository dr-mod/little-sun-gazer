cmake_minimum_required(VERSION 3.16.3)

# Pull in SDK
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

# ePaper stuff
include_directories(../Pico_ePaper_Code/c/lib/Config)
include_directories(../Pico_ePaper_Code/c/lib/GUI)

project(little-sun-gazer)

pico_sdk_init()

add_subdirectory(ds3231)

# Reference parallel directory
add_subdirectory(../Pico_ePaper_Code/c/lib/Fonts Fonts)
add_subdirectory(../Pico_ePaper_Code/c/lib/Config Config)
add_subdirectory(../Pico_ePaper_Code/c/lib/GUI GUI)

include_directories(./utils)
include_directories(./ds3231)
include_directories(./Config)

add_executable(little-sun-gazer main.cpp)

pico_add_extra_outputs(little-sun-gazer)

target_link_libraries(little-sun-gazer
    ds3231
    pico_stdlib
    Fonts
    hardware_rtc
    hardware_sleep
    hardware_i2c
    hardware_adc
    hardware_spi
)