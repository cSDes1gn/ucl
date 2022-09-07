cmake_minimum_required(VERSION 3.16)

# AVR Fuses
set(E_FUSE 0xfd)
set(H_FUSE 0xd8)
set(L_FUSE 0xff)
set(LOCK_BIT 0xff)

set(CMAKE_C_STANDARD 99)
# set target system
set(CMAKE_SYSTEM_NAME Generic)

## AVR Chip Configuration
set(F_CPU 16000000UL)
set(MCU atmega2560)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUDRATE 115200)
# The programmer to use, read avrdude manual for list
set(AVRDUDE_PROG_TYPE "wiring")

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

find_program(AVR_CC NAMES avr-gcc PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVR_CXX NAMES avr-g++ PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVR_OBJCOPY NAMES avr-objcopy PATHS ${CGT_ROOT} NO_DEFAULT_PATH)
find_program(AVRDUDE NAMES avr-objcopy PATHS ${CGT_ROOT} NO_DEFAULT_PATH)

set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})


project(
  avr
  LANGUAGES CXX C
  VERSION "1.0.0"
  DESCRIPTION "Avr-gcc firmware toolchain demo"
)

# cmake build guard
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

message(STATUS "Building avr-gcc demo")
add_executable(avr src/main.c src/usart.c)
include_directories(include)

# Burn fuses
add_custom_target(fuses ${AVRDUDE} -c ${AVRDUDE_PROG_TYPE} -p ${MCU}  -U lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m )
