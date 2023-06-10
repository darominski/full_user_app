cmake_minimum_required(VERSION 3.17)

set(TARGET "" CACHE STRING "Target -- DIOT for a local install")

set(USE_LIBLOG "1" CACHE STRING "Use liblog -- 1 or 0")

set(LIB v_loop)

set(CMAKE_SYSTEM_NAME Generic)

if (CMAKE_SIZE STREQUAL "")
  message(FATAL_ERROR "You must provide CMAKE_SIZE option")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_compile_options(-O2 -Wall -mtune=cortex-a53 -ffunction-sections -fdata-sections)

add_link_options(
  -Wall
    -ffreestanding
    -specs=nosys.specs
    -Wl,--gc-sections
    )
  

project(v_loop C CXX ASM)

### BSP

include(../bsp/bsp.cmake)
  
MakeBsp(1)

### END BSP

### bumbleboot

set(BUILD_SLAVE_PAYLOAD 1)   # TODO: This is *bad*. It should probably be a separate CMakeLists instead.
add_subdirectory(../bmboot ${CMAKE_CURRENT_BINARY_DIR}/bmboot)

### END bumbleboot

### fgc4_hal

set(FGC4_HAL_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../hal/inc)

# ### END fgc4_hal

set(BMBOOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../bmboot)

include(${BMBOOT_DIR}/cmake/Bmboot.cmake)

add_executable(${LIB}.elf
  main.cpp
  src/Communication.cpp
  src/retarget_bmboot.cpp
  src/retarget.h
  
  ../hal/inc/converter_interface.h
  ../hal/inc/peripheral_board.h
  ../hal/inc/peripherals/adc_serial.h
  ../hal/inc/peripherals/dac_serial.h
  ../hal/inc/peripherals/in_out.h
  )

target_include_directories(${LIB}.elf PRIVATE
  inc
  public_include
  ${FGC4_HAL_INCLUDE}
  )

find_package(PropertyLib REQUIRED)

target_link_libraries(v_loop.elf
  PUBLIC
  PropertyLib
  PRIVATE
  bmboot_payload
  fgc4_bsp_el1
  )

Bmboot_PayloadPostBuild(v_loop.elf)

# post-build, automatically run:
# - objcopy to create .bin file
# - objdump to create disassembly
# - (not needed at the moment: mkimage to creat U-Boot image)
set(IMG_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.img)
if (OVERRIDE_BIN_FILE)
  set(BIN_FILE ${OVERRIDE_BIN_FILE})
else()
  set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/build_flags.txt "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}")    # needed for benchmark framework
