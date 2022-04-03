aux_source_directory(. DIR_ds3231_SRCS)

add_library(ds3231 ${DIR_ds3231_SRCS})
target_link_libraries(ds3231 PUBLIC pico_stdlib hardware_i2c)
