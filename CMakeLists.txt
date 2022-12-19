cmake_minimum_required(VERSION 3.16.3)
# Pull in SDK (must be before project)
include(../pico-sdk/pico_sdk_init.cmake)
include(../pico-extras/pico_sdk_import.cmake)
project(little-sun-gazer)
pico_sdk_init()
add_subdirectory(ds3231)
add_subdirectory(../Pico_ePaper_Code/c/lib/Fonts Fonts)
add_subdirectory(../Pico_ePaper_Code/c/lib/Config Config)
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
#    hardware_sleep
    hardware_i2c
    hardware_adc
    hardware_spi
)