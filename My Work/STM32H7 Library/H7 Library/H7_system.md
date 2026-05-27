## About

The `H7_system` module contains the STM32H7 system core initialization and shared system utilities for H7Lib1.0. It configures the CPU, clocks, caches, HAL, and runtime helpers.

## Features

- System clock configuration
- Memory Protection Unit (MPU) setup
- Cache enablement
- HAL initialization
- DWT / timing support
- Short type aliases for consistent integer usage
- Central system state enum shared across modules
- Global `H7SYSTEM` structure for status and utility access

## Files

![H7 System Files](attachment/h7syste.png)

The `H7_system` module uses `Platform/H7_system.c` and `Platform/H7_system.h`.

## Overview

`H7_system` initializes low-level platform settings and provides a shared foundation for the rest of the library.

## MPU Initialization

The `H7_system.c` file includes the platform MPU setup used by H7Lib1.0. This initialization configures the memory protection regions and access attributes required for DMA buffers, code execution, and peripheral memory access.

- The MPU is configured before the main application starts.
- It ensures the Cortex-M7 can safely access instruction memory, data memory, and the DMA memory region.
- Proper MPU configuration is critical for stable DMA transfers and cache coherence on STM32H7.

The MPU setup is part of the system initialization sequence and helps protect the memory map while enabling high-performance access for the library.

## Special Types

The module defines compact signed and unsigned aliases to simplify code and improve consistency.

```c
///// Signed types /////
typedef __int64_t s64;
typedef __int32_t s32;
typedef __int16_t s16;
typedef __int8_t s8;

// Constant signed types
typedef const __int64_t sc64;
typedef const __int32_t sc32;
typedef const __int16_t sc16;
typedef const __int8_t sc8;

// Volatile signed types
typedef volatile __int64_t vs64;
typedef volatile __int32_t vs32;
typedef volatile __int16_t vs16;
typedef volatile __int8_t vs8;

///// Unsigned types ////
typedef __uint64_t u64;
typedef __uint32_t u32;
typedef __uint16_t u16;
typedef __uint8_t u8;

// Unsigned constant types
typedef const __uint64_t uc64;
typedef const __uint32_t uc32;
typedef const __uint16_t uc16;
typedef const __uint8_t uc8;

// Volatile unsigned types
typedef volatile __uint64_t vu64;
typedef volatile __uint32_t vu32;
typedef volatile __uint16_t vu16;
typedef volatile __uint8_t vu8;
```

These type aliases make code shorter and more consistent across the library.

## System State Enum

The `H7_state_e` enum defines shared status and error values used by multiple modules.

```c
typedef enum{
/**************** System states ****************/
H7_STATE_NULL,
H7_OK,
H7_PERIPH_OK,
/**************** GPIO states ****************/
H7_GPIO_PORT_INV,
/**************** UART states ****************/
H7_UART_NULL_PTR,
H7_UART_PORT_INV,
H7_UART_INIT_ERR,
H7_UART_DMA_INIT_ERR,
/**************** Timer states ****************/
H7_PWM_CHANNEL_OK,
H7_TIM_NULL_PTR,
H7_TIM_INVALID_TIMER,
H7_TIM_INIT_ERR,
H7_TIM_PWM_INIT_ERR,
H7_TIM_TIMEOUT_ERR,
// Configuration Errors
H7_TIM_MASTER_CONF_ERR,
H7_TIM_CLKSOURCE_CONF_ERR,
H7_PWM_CONFIG_CHANNEL_ERR,
H7_TIM_START_Failed,
H7_TIM_START_IT_Failed,
H7_PWM_START_FAILED,
/**************** FDCAN states ****************/
H7_FDCAN_FILTER_OK,
H7_FDCAN_NULL_PTR,
H7_FDCAN_INVALID_PORT,
H7_FDCAN_INIT_ERR,
H7_FDCAN_START_ERR,
H7_FDCAN_FIFO_ACTICATE_ERR,
H7_FDCAN_TIMEOUT_ERR,
H7_FDCAN_FILTER_INIT_ERR,
H7_FDCAN_MEM_RAM_OVERSIZE,
H7_FDCAN_MSGSEND_ERR,
H7_FDCAN_TX_FIFO_FULL,
/**************** I2C states ****************/
H7_I2C_NULL_PTR,
H7_I2C_INVALID_PORT,
H7_I2C_INVALID_SPEED,
H7_I2C_INIT_ERR,
H7_I2C_DMA_INIT_ERR,
H7_I2C_TIMEOUT_ERR,
// Configuration Errors
H7_I2C_AN_FILTER_CONFIG_ERR,
H7_I2C_DG_FILTER_CONFIG_ERR,
H7_I2C_START_IT_Failed,
/**************** QEI states ****************/
H7_QEI_INVALID_PORT,
H7_QEI_INIT_ERR,
/**************** EXTI states ****************/
H7_EXTI_NULL_PTR,
H7_EXTI_INVALID_PIN
} H7_state_e;
```

This enum is reused across the library for consistent error handling.

## Register Access Types

`H7_system` also defines structures for direct register bit access.

```c
typedef struct{
    union{
        u32 reg_32;
        struct{
            unsigned bit0 : 1;
            unsigned bit1 : 1;
            unsigned bit2 : 1;
            unsigned bit3 : 1;
            unsigned bit4 : 1;
            unsigned bit5 : 1;
            unsigned bit6 : 1;
            unsigned bit7 : 1;
            unsigned bit8 : 1;
            unsigned bit9 : 1;
            unsigned bit10 : 1;
            unsigned bit11 : 1;
            unsigned bit12 : 1;
            unsigned bit13 : 1;
            unsigned bit14 : 1;
            unsigned bit15 : 1;
            unsigned bit16 : 1;
            unsigned bit17 : 1;
            unsigned bit18 : 1;
            unsigned bit19 : 1;
            unsigned bit20 : 1;
            unsigned bit21 : 1;
            unsigned bit22 : 1;
            unsigned bit23 : 1;
            unsigned bit24 : 1;
            unsigned bit25 : 1;
            unsigned bit26 : 1;
            unsigned bit27 : 1;
            unsigned bit28 : 1;
            unsigned bit29 : 1;
            unsigned bit30 : 1;
            unsigned bit31 : 1;
        };
    };
} H7_word_s;

typedef struct{
    union{
        u16 reg_16;
        struct{
            unsigned bit0 : 1;
            unsigned bit1 : 1;
            unsigned bit2 : 1;
            unsigned bit3 : 1;
            unsigned bit4 : 1;
            unsigned bit5 : 1;
            unsigned bit6 : 1;
            unsigned bit7 : 1;
            unsigned bit8 : 1;
            unsigned bit9 : 1;
            unsigned bit10 : 1;
            unsigned bit11 : 1;
            unsigned bit12 : 1;
            unsigned bit13 : 1;
            unsigned bit14 : 1;
            unsigned bit15 : 1;
        };
    };
} H7_half_word_s;
```

These structures allow bit-level access to 32-bit and 16-bit registers.

## System Structure

The `H7_System_s` structure provides centralized access to system state and helper functions.

```c
typedef struct {
    struct {
        H7_state_e system;
        H7_state_e last_error;
        u16 error_count;
        float cpu_load;
        u32 free_heap;
    } status;

    struct {
        vu32 (*get_tick_us)(void);
        u32 (*get_tick_ms)(void);
        u32 (*get_tick_s)(void);
        void (*delay_us)(u32);
        osStatus_t (*delay_ms)(u32);
    } time;

    void (*reset)(void);
} H7_System_s;

extern H7_System_s H7SYSTEM;
```

This structure makes system diagnostics and utility functions easy to access from any module.

## Error Handling

`H7_system` uses `H7_state_e` values for status and error reporting. When a fatal or configuration error occurs, `Error_Handler()` is called and execution stops. The macro `H7_ERR_HANDLE()` saves the most recent error to `H7SYSTEM.last_error`.

### `H7_system_init()`

This function performs core system configuration and initializes the `H7SYSTEM` object, including timing functions and reset callbacks.
