cmake_minimum_required(VERSION 3.13)
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Pull in SDK 
set(PICO_BOARD pico_w)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(app C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(blink main.c)

target_include_directories(blink PRIVATE 
${CMAKE_CURRENT_LIST_DIR}
)
 
# pull in common dependencies
target_link_libraries(blink 
    pico_stdlib 
    pico_cyw43_arch_none
    FreeRTOS-Kernel 
    FreeRTOS-Kernel-Heap4
    )

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(blink)
