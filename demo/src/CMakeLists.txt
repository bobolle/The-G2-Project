cmake_minimum_required(VERSION 3.13)

add_library(demo-lib)

target_sources(demo-lib
    PRIVATE
        photoresistor.c
)

target_include_directories(demo-lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/lwip
)

target_link_libraries(demo-lib
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
    hardware_gpio
    hardware_adc
)

add_executable(demo main.c)

target_link_libraries(demo
    demo-lib
)

pico_enable_stdio_usb(demo 1)
pico_enable_stdio_uart(demo 1)
pico_add_extra_outputs(demo)
