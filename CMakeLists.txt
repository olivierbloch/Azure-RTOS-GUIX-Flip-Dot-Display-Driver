# Licensed under the MIT License.

cmake_minimum_required (VERSION 3.10)
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project (AzureSRFlipdot C ASM)

SET(CMAKE_ASM_FLAGS "-mcpu=cortex-m4")

azsphere_configure_tools(TOOLS_REVISION "20.07")

ADD_COMPILE_DEFINITIONS(OSAI_BARE_METAL)
ADD_LINK_OPTIONS(-specs=nano.specs -specs=nosys.specs)

include_directories(${CMAKE_SOURCE_DIR} 
	                ${CMAKE_SOURCE_DIR}/mt3620_lib
					${CMAKE_SOURCE_DIR}/mt3620_lib/mt3620)


# Create executable
add_executable (${PROJECT_NAME}
    mt3620-m4-samples/ThreadX_RTApp_MT3620_AzureRTOS/tx_initialize_low_level.S
    mt3620-m4-samples/ThreadX_RTApp_MT3620_AzureRTOS/VectorTable.c
    mt3620_lib/UART.c
    flipdot_graphics_driver.c
    flipdot_guix_resources.c
    flipdot_guix_specifications.c
    threadx_app.c
    main.c
    )

set(THREADX_ARCH "cortex_m4")
set(THREADX_TOOLCHAIN "gnu")
add_subdirectory(threadx)
add_subdirectory(guix)

target_link_libraries(${PROJECT_NAME} azrtos::threadx azrtos::guix)
set_target_properties (${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_HOME_DIRECTORY}/mt3620-m4-samples/ThreadX_RTApp_MT3620_AzureRTOS/linker.ld)

# target_link_libraries (${PROJECT_NAME} PUBLIC -L"${CMAKE_SOURCE_DIR}")

azsphere_target_add_image_package(${PROJECT_NAME})
