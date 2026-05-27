# Priority Configuration

This document describes the interrupt priority settings defined in `Platform/priorities.h`.

## Purpose

The H7 library centralizes NVIC interrupt priorities in one header file. This makes it easier to adjust interrupt behavior for FDCAN, I2C, SPI, UART, timers, and DMA.

## Priority Rules

- Priority values range from `0` to `15`.
- `0` is the highest priority.
- Higher numeric values are lower priority.
- Use lower priority (higher number) for peripherals that generate frequent or repeated requests to avoid blocking higher-priority interrupts.

## File Location

- `Platform/priorities.h`

## FDCAN Priorities

The FDCAN interrupts are grouped by IT0 and IT1 priorities.

| Macro | Default | Description |
|---|---|---|
| `H7_FDCAN1_IT0_IRQ_PRIO` | `8` | FDCAN1 high-priority interrupt line 0 |
| `H7_FDCAN2_IT0_IRQ_PRIO` | `8` | FDCAN2 high-priority interrupt line 0 |
| `H7_FDCAN3_IT0_IRQ_PRIO` | `8` | FDCAN3 high-priority interrupt line 0 |
| `H7_FDCAN1_IT1_IRQ_PRIO` | `13` | FDCAN1 low-priority interrupt line 1 |
| `H7_FDCAN2_IT1_IRQ_PRIO` | `13` | FDCAN2 low-priority interrupt line 1 |
| `H7_FDCAN3_IT1_IRQ_PRIO` | `13` | FDCAN3 low-priority interrupt line 1 |
| `H7_FDCAN_CAL_IRQ_PRIO` | `13` | Clock calibration interrupt priority |

## I2C Priorities

| Macro | Default | Description |
|---|---|---|
| `H7_I2C1_EVENT_IRQ_PRIO` | `10` | I2C1 event interrupt priority |
| `H7_I2C1_ERROR_IRQ_PRIO` | `8` | I2C1 error interrupt priority |
| `H7_I2C5_EVENT_IRQ_PRIO` | `10` | I2C5 event interrupt priority |
| `H7_I2C5_ERROR_IRQ_PRIO` | `8` | I2C5 error interrupt priority |

## SPI Priorities

| Macro | Default | Description |
|---|---|---|
| `H7_SPI2_IRQ_PRIO` | `10` | SPI2 interrupt priority |
| `H7_SPI2_TX_DMA_PRIO` | `8` | SPI2 DMA transmit priority |
| `H7_SPI2_RX_DMA_PRIO` | `8` | SPI2 DMA receive priority |

## UART Priorities

| Macro | Default | Description |
|---|---|---|
| `H7_UART4_IRQ_PRIO` | `12` | UART4 interrupt priority |
| `H7_UART5_IRQ_PRIO` | `12` | UART5 interrupt priority |
| `H7_UART7_IRQ_PRIO` | `12` | UART7 interrupt priority |
| `H7_UART9_IRQ_PRIO` | `12` | UART9 interrupt priority |
| `H7_UART4_TX_DMA_IRQ_PRIO` | `5` | UART4 DMA transmit priority |
| `H7_UART4_RX_DMA_IRQ_PRIO` | `5` | UART4 DMA receive priority |
| `H7_UART5_TX_DMA_IRQ_PRIO` | `5` | UART5 DMA transmit priority |
| `H7_UART5_RX_DMA_IRQ_PRIO` | `5` | UART5 DMA receive priority |
| `H7_UART7_TX_DMA_IRQ_PRIO` | `5` | UART7 DMA transmit priority |
| `H7_UART7_RX_DMA_IRQ_PRIO` | `5` | UART7 DMA receive priority |
| `H7_UART9_TX_DMA_IRQ_PRIO` | `5` | UART9 DMA transmit priority |
| `H7_UART9_RX_DMA_IRQ_PRIO` | `5` | UART9 DMA receive priority |

## Timer and QEI Priorities

| Macro | Default | Description |
|---|---|---|
| `H7_TIM1_UPDATE_IRQ_PRIO` | `7` | TIM1 update interrupt priority (QEI1) |
| `H7_TIM1_TRG_COM_IRQ_PRIO` | `10` | TIM1 trigger/commutation interrupt priority |
| `H7_TIM1_COMPARE_CAPTURE_IRQ_PRIO` | `7` | TIM1 capture/compare interrupt priority |
| `H7_TIM8_TIM12_BREAK_IRQ_PRIO` | `5` | TIM8/TIM12 break interrupt priority |
| `H7_TIM8_TIM13_UPDATE_IRQ_PRIO` | `7` | TIM8/TIM13 update interrupt priority |
| `H7_TIM8_TIM14_TRG_COM_IRQ_PRIO` | `10` | TIM8/TIM14 trigger/commutation interrupt priority |
| `H7_TIM8_COMPARE_CAPTURE_IRQ_PRIO` | `7` | TIM8 capture/compare interrupt priority |
| `H7_TIM2_IRQ_PRIO` | `5` | TIM2 interrupt priority |
| `H7_TIM3_IRQ_PRIO` | `5` | TIM3 interrupt priority |
| `H7_TIM4_IRQ_PRIO` | `5` | TIM4 interrupt priority |
| `H7_TIM5_IRQ_PRIO` | `5` | TIM5 interrupt priority |
| `H7_TIM6_DAC_IRQ_PRIO` | `2` | TIM6/DAC interrupt priority |
| `H7_TIM7_IRQ_PRIO` | `5` | TIM7 interrupt priority |
| `H7_TIM12_IRQ_PRIO` | `5` | TIM12 interrupt priority |
| `H7_TIM15_IRQ_PRIO` | `5` | TIM15 interrupt priority |
| `H7_TIM16_IRQ_PRIO` | `5` | TIM16 interrupt priority |
| `H7_TIM17_IRQ_PRIO` | `5` | TIM17 interrupt priority |
| `H7_TIM23_IRQ_PRIO` | `5` | TIM23 interrupt priority |
| `H7_TIM24_IRQ_PRIO` | `5` | TIM24 interrupt priority |

## Usage Notes

- Interrupt priorities should be chosen carefully. A high-frequency peripheral should not use the highest system priority unless it must preempt all other activity.
- Use lower priority values for truly critical interrupts.
- Use higher priority values for frequent or non-critical interrupts to keep the system responsive.

