cmake_minimum_required(VERSION 3.12)

include(../pimoroni-pico/libraries/pico_explorer/pico_explorer.cmake)
include(pico_sdk_import.cmake)

pico_sdk_init()

add_executable(snake
    main.cpp
)

target_link_libraries(snake 
    pico_stdlib 
    pico_multicore
    pico_explorer
)

pico_add_extra_outputs(snake)