cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(waterfill)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Set CMake build type (Release, Debug, etc.)
set(CMAKE_BUILD_TYPE Debug)

# Add source files to the project
# set(SOURCES src/main.cpp src/SparkFun_Qwiic_Relay.cpp)

# Add header files to the project
# set(HEADERS include/SparkFun_Qwiic_Relay.h)

add_executable(${PROJECT_NAME} src/program.cpp)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_gpio pico_cyw43_arch_none)

# Enable extra outputs for the Pico SDK libraries
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)       

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# Generate the UF2 file
pico_add_extra_outputs(${PROJECT_NAME})

# Custom build target
add_custom_target(build
    COMMAND cmake --build ${CMAKE_BINARY_DIR}
    COMMENT "Build ${PROJECT_NAME}"
)

# Custom flash target
add_custom_target(flash
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/flash.sh ${PROJECT_NAME}
    DEPENDS build
    COMMENT "Flashing ${PROJECT_NAME}.uf2 to Raspberry Pi Pico"
)