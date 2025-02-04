cmake_minimum_required(VERSION 3.13)

# Project name
project(tcp_server C)

set(CMAKE_C_STANDARD 11)

# Executeable
add_executable(${CMAKE_PROJECT_NAME} server.c)

# Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE m)
