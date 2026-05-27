# QEI (Quadrature Encoder Interface)

This document describes the QEI peripheral usage within H7Lib1.0. The library uses timer-based encoder handling with a dedicated peripheral handler.

## Features

- Supports timer-based quadrature encoder counting
- Uses callback-driven capture handling for direction-aware counting
- Provides a simple handler structure for state and count tracking
- Supports multiple timer inputs for QEI channels

## Overview

The Quadrature Encoder Interface (QEI) is implemented using Timer peripherals in counting mode to measure rotational motion from quadrature encoders. The STM32H7 provides multiple timers that can be configured as QEI interfaces.

## Physical Ports

- Timer1 (QEI1): PA8 (CH1), PA9 (CH2)
- Timer8 (QEI2): PC6 (CH1), PC7 (CH2)

## Files

- qei.c
- qei.h

[!NOTE] H7_QEI library is located at Drivers/.

## Peripheral Handle Structure

```c
typedef struct{
	TIM_HandleTypeDef *htimer;
	
	vs32 count;

	H7_state_e state;
} H7_QEIHandler_s;
```

- Use the handler for all QEI operations to ensure consistent functionality.
- `count`: Current encoder count value (signed 32-bit).
- `state`: Current state of the QEI port.
- [!WARNING] These variables should be used for debugging purposes only and should not be modified directly.

## How to Use QEI

### Initialization

```c
H7_QEI_init(&QEI1, &htim1);
```

The QEI initialization should be called during the adapter module initialization phase (see `adapter.c`).

### QEI Position Tracking

#### Get Current Ticks
```c
uint32_t current_count = QEI1.count;
```

#### Reset Position
```c
H7_QEI_reset(&QEI1);
```

## Interrupt Handling

- The library uses timer input capture callbacks to increment or decrement the `count` variable inside `H7_QEIHandler_s`.

```c
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) {
	  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1)){
		  QEI1.count--;
	  }
	  else {
		  QEI1.count++;
	  }
    }

    if (htim->Instance == TIM8) {
	  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim8)){
		  QEI2.count--;
	  }
	  else {
		  QEI2.count++;
	  }
    }

}
```


## Common QEI Configurations

| Parameter | Value | Notes |
|-----------|-------|-------|
| Counting Mode | TIM_COUNTERMODE_UP | Use for incremental encoders |
| Polarity | Single edge | Capture only the rising edge |
| Prescaler | 0 | No prescaling for encoders |
| Auto-reload | 0xFFFF | Full 16-bit range |

## Notes and Warnings

- [!NOTE] Ensure proper grounding of the encoder cable shield to reduce noise.
- [!NOTE] Place the encoder close to the microcontroller or use twisted pair cables for longer distances.
- [!WARNING] Do not configure the QEI timer for any other purpose.


## Common Issues and Solutions

### Counter Not Incrementing
- Verify the QEI port has been initialized.
- Check encoder wiring and connections.

