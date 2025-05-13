add_library(pico_ultrasonic_lib uss.h uss.c)
target_link_libraries(pico_ultrasonic_lib pico_stdlib hardware_gpio hardware_timer)
target_include_directories(pico_ultrasonic_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
