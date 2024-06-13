cmake_minimum_required(VERSION 3.12)

# pico-sdk: branch master; sha: f396d05f8252d4670d4ea05c8b7ac938ef0cd381

include(pico_sdk_import.cmake)

project(gpd340_reverse C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()


set (LIBS
        pico_stdlib
        pico_multicore
        cmsis_core
        hardware_pwm
        hardware_pio
        hardware_dma
)

add_executable(demo_bitbang demo_bitbang.c)

target_include_directories(demo_bitbang PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(demo_bitbang ${LIBS})
pico_add_extra_outputs(demo_bitbang)


add_executable(demo_pio demo_pio.c)

pico_generate_pio_header(demo_pio ${CMAKE_CURRENT_LIST_DIR}/el.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(demo_pio PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(demo_pio ${LIBS})
pico_add_extra_outputs(demo_pio)


add_executable(demo_pio_dma demo_pio_dma.c)

pico_generate_pio_header(demo_pio_dma ${CMAKE_CURRENT_LIST_DIR}/el.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(demo_pio_dma PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(demo_pio_dma ${LIBS})
pico_add_extra_outputs(demo_pio_dma)

