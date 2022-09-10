cmake_minimum_required(VERSION 3.16)

# AVR Fuses
set(E_FUSE 0xfd)
set(H_FUSE 0xd8)
set(L_FUSE 0xff)
set(LOCK_BIT 0xcf)

set(CMAKE_C_STANDARD 99)
# set target system
set(CMAKE_SYSTEM_NAME Generic)
# remove compiler tests against local libs
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

## AVR Chip Configuration
set(F_CPU 16000000UL)
set(MCU atmega2560)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUDRATE 115200)
# The programmer to use, read avrdude manual for list
set(AVRDUDE_PROG_TYPE "wiring")

# set serial usb device for flashing
set(SERIAL_PORT "/dev/tty.usbserial-A10JTC54")
# setup toolchain paths
set(CGT_ROOT "/usr/local/bin")

# mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

# Pass defines to compiler
add_definitions(
    -DF_CPU=${F_CPU}
    -DBAUD=${BAUD}
)
add_compile_options(
    -mmcu=${MCU} # MCU
    -std=gnu99 # C99 standard
    -Os # optimize
    -Wall # enable warnings
    -Wno-main
    -Wundef
    -pedantic
    -Wstrict-prototypes
    -Werror
    -Wfatal-errors
    -Wl,--relax,--gc-sections
    -g
    -gdwarf-2
    -funsigned-char # a few optimizations
    -funsigned-bitfields
    -fpack-struct
    -fshort-enums
    -ffunction-sections
    -fdata-sections
    -fno-split-wide-types
    -fno-tree-scev-cprop
)

# find programs
find_program(AVR_CC NAMES avr-gcc PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVR_CXX NAMES avr-g++ PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVR_OBJCOPY NAMES avr-objcopy PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVR_STRIP NAMES avr-strip PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVRDUDE NAMES avrdude PATHS ${CGT_ROOT} NO_DEFAULT_PATH)

set(CMAKE_ASM_COMPILER ${AVR_CC})
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})
# global project name
set(PROJECT_NAME "ucl")

project(
  ${PROJECT_NAME}
  LANGUAGES CXX C
  VERSION "1.0.0"
  DESCRIPTION "Under-cabinet lighting firmware."
)

# cmake build guard
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

message(STATUS "Configuring ucl source")
add_executable(
  ${PROJECT_NAME}
  src/main.c
  src/usart.c
  src/logger.c
  src/eeprom.c
)
include_directories(include)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

message(STATUS "Adding custom targets")
# Strip binary for upload
add_custom_target(strip ALL ${AVR_STRIP} ${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})
# Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(hex ALL ${AVR_OBJCOPY} -R .eeprom -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS strip)
# Upload the firmware with avrdude
add_custom_target(flash ${AVRDUDE} -c ${AVRDUDE_PROG_TYPE} -p${MCU} -P${SERIAL_PORT} -b${BAUDRATE} -D -Uflash:w:${PROJECT_NAME}.hex:i DEPENDS hex)
# Burn fuses
add_custom_target(fuses ${AVRDUDE} -c${AVRDUDE_PROG_TYPE} -p${MCU} -P${SERIAL_PORT} -b${BAUDRATE} -Ulfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m)

message(STATUS "Flagging additional files for cleaning")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")