cmake_minimum_required(VERSION 3.13)
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Define board
if (NOT DEFINED PICO_BOARD)
    set(PICO_BOARD "pico_w")
    message(STATUS "PICO_BOARD was not set, defaulting to ${PICO_BOARD}")
endif()

# Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Project define
project(edgedevice C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Controll server and wifi-flags are set
if (NOT DEFINED WIFI_SSID)
    message(FATAL_ERROR "Wifi SSiD is not set! use -DWIFI_SSID=\"your_ssid\" when running cmake.")
endif()

if (NOT DEFINED WIFI_PASS)
    message(FATAL_ERROR "Wifi password is not set! use -DWIFI_PASS=\"your_password\" when running cmake.")
endif()

if (NOT DEFINED SERVER_IP)
    message(FATAL_ERROR "Server IP is not set! use -DSERVER_IP=\"your_server_ip\" when running cmake.")
endif()

if (NOT DEFINED SERVER_PORT)
    message(FATAL_ERROR "Server port is not set! use -DSERVER_PORT=\"your_server_port\" when running cmake.")
endif()

# Initialize Pico SDK
pico_sdk_init()

# Executables
add_executable(${CMAKE_PROJECT_NAME} main.c)

# USB and UART support
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)

# Link libs
target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
    hardware_adc
    FreeRTOS-Kernel 
    FreeRTOS-Kernel-Heap4
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lwip
)
# Flags to compilator
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    LWIP_MQTT=1
    WIFI_SSID="${WIFI_SSID}"
    WIFI_PASS="${WIFI_PASS}"
    MQTT_BROKER_IP="${SERVER_IP}"
    MQTT_BROKER_PORT=${SERVER_PORT}
    MQTT_USER="${MQTT_USER}"
    MQTT_PASSWORD="${MQTT_PASSWORD}"
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${PICO_SDK_PATH}/lib/lwip/src/apps/mqtt/mqtt.c)

# Outdata
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
