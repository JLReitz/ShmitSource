set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION Cortex-M4-STM32F407)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(ARM_OPTIONS -mcpu=cortex-m4)

add_compile_options(
    ${ARM_OPTIONS}
        -fmessage-length=0
        -funsigned-char
        -ffunction-sections
        -fdata-sections
        -MMD
        -MP
)

add_compile_definitions(
    STM32F407xx
    USE_FULL_ASSERT
    OS_USE_TRACE_SEMIHOSTING_STDOUT
    OS_USE_SEMIHOSTING
)

add_link_options(
    ${ARM_OPTIONS}
    --specs=rdimon.specs
    -u_printf_float
    -u_scanf_float
    -nostartfiles
    LINKER:--gc-sections
    LINKER:--build-id
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)