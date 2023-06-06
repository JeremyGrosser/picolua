cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(picolua)
pico_sdk_init()

add_subdirectory(lua-5.4.6/src)

add_executable(picolua
    picolua.c
)

pico_enable_stdio_usb(picolua 1)
pico_enable_stdio_uart(picolua 1)
pico_add_extra_outputs(picolua)

target_link_libraries(picolua pico_stdlib lua)
