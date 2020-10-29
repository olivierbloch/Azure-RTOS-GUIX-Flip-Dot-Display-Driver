#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.10)
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project (AzureSRFlipdot C ASM)

SET(CMAKE_ASM_FLAGS "-mcpu=cortex-m4")

azsphere_configure_tools(TOOLS_REVISION "20.07")
azsphere_configure_api(TARGET_API_SET "6")

ADD_COMPILE_DEFINITIONS(OSAI_BARE_METAL)
ADD_LINK_OPTIONS(-specs=nano.specs -specs=nosys.specs)

# Create executable
add_executable (${PROJECT_NAME}
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_adc.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_dma.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_eint.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_gpio.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_gpioif.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_gpt.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_i2c.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_i2s.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_pwm.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_spim.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_uart.c
    mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/src/os_hal_wdt.c
    threadx_app.c
    main.c
    tx_initialize_low_level.S
    )

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC mt3620_m4_software/MT3620_M4_Sample_Code/OS_HAL/inc)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
                           mt3620_m4_software/MT3620_M4_BSP/CMSIS/include
                           mt3620_m4_software/MT3620_M4_BSP/mt3620/inc
                           mt3620_m4_software/MT3620_M4_BSP/printf
                           mt3620_m4_software/MT3620_M4_Driver/MHAL/inc
                           mt3620_m4_software/MT3620_M4_Driver/HDL/inc)

ADD_SUBDIRECTORY(./mt3620_m4_software/MT3620_M4_Driver ./lib/MT3620_M4_Driver)

set(THREADX_ARCH "cortex_m4")
set(THREADX_TOOLCHAIN "gnu")
add_subdirectory(threadx)

target_link_libraries (${PROJECT_NAME} MT3620_M4_Driver)
target_link_libraries(${PROJECT_NAME} azrtos::threadx)

set_target_properties (${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/linker.ld)

azsphere_target_add_image_package(${PROJECT_NAME})
