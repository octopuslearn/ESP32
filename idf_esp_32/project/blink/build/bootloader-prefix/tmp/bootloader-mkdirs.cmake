# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/SoftwareInstall/Espressif/frameworks/esp-idf-v5.1/components/bootloader/subproject"
  "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader"
  "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix"
  "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix/tmp"
  "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix/src/bootloader-stamp"
  "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix/src"
  "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/project/ESP32_learn/idf_esp_32/project/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
