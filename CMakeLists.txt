# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define a variável do diretório onde os cabeçalhos estão localizados
include_directories(${CMAKE_SOURCE_DIR}/servo_motor)  # Adiciona o diretório 'servo_motor'
include_directories(${CMAKE_SOURCE_DIR}/stepper_motor)  # Adiciona o diretório 'stepper_motor'
include_directories(${CMAKE_SOURCE_DIR}/joystick)  # Adiciona o diretório 'stepper_motor'
include_directories(${CMAKE_SOURCE_DIR}/working_led)  # Adiciona o diretório 'stepper_motor'
include_directories(${CMAKE_SOURCE_DIR}/audio)  # Adiciona o diretório 'stepper_motor'
include_directories(${CMAKE_SOURCE_DIR}/display_oled)  # Adiciona o diretório 'stepper_motor'

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(main
    main.c
    servo_motor/servo_motor.c
    stepper_motor/stepper_motor.c
    joystick/joystick.c
    working_led/working_led.c
    audio/play_audio.c
    display_oled/ssd1306_i2c.c
    )

pico_set_program_name(main "BitDogLab")
pico_set_program_version(main "0.1")

# Link libraries
target_link_libraries(main
    pico_stdlib
    hardware_pwm
    hardware_adc
    hardware_i2c
    )

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(main pico_cyw43_arch_none)
endif()

target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)

# add url via pico_set_program_url

# Adiciona suporte às bibliotecas necessárias
# A função pico_enable_stdio_* habilita saída pela UART ou USB
pico_enable_stdio_uart(main 0)  # Habilita saída pela UART (1 = ativo)
pico_enable_stdio_usb(main 1)   # Desativa saída pela USB (0 = inativo)

