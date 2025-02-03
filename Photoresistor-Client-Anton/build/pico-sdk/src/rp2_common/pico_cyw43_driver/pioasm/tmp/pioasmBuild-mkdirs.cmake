# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/anton/pico/pico-sdk/tools/pioasm"
  "/home/anton/ws/projectg2/photoresistor/build/pioasm"
  "/home/anton/ws/projectg2/photoresistor/build/pioasm-install"
  "/home/anton/ws/projectg2/photoresistor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/anton/ws/projectg2/photoresistor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/anton/ws/projectg2/photoresistor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/anton/ws/projectg2/photoresistor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/anton/ws/projectg2/photoresistor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/anton/ws/projectg2/photoresistor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
