# QEI (Quadrature Encoder Interface)

Generally in H7Lib1.0, most peripherals (if not all), have their own ==Peripheral Handle Structure==.

## Overview

The Quadrature Encoder Interface (QEI) is implemented using Timer peripherals in counting mode to measure rotational motion from quadrature encoders. The STM32H7 provides multiple timers that can be configured as QEI interfaces.

## Physical Ports

- Timer1(QEI1): PA8 (CH1), PA9 (CH2) 
- Timer8(QEI2): PC6 (CH1), PC7 (CH2) 

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

- The user is recommended to use the handler for any use of the QEI peripheral, to ensure smooth functionality.
- *count*: Current encoder count value (signed 32-bit)
- *state*: Holds the current state of qei port
- [!warning] These variables should be used for debugging purposes only and should not be modified directly.

## How to Use QEI

#### Initialization

```c
H7_QEI_init(&QEI1, &htim1);
```

The UART initialization should be called during the adapter module initialization phase (see `adapter.c`).

### QEI Position Tracking

#### Get Current ticks
```c
uint32_t current_count = QEI1.count;
```

#### Reset Position
```c
void H7_QEI_reset(&QEI1);
```


## Interrupt Handling

- The library uses IC interrupt to increment or decrement `count` variable inside `H7_QEIHandler_s`

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
| Counting Mode | TIM_COUNTERMODE_UP | Use for incremmental encoders |
| Polarity | Signle edge | Capture capture only rising |
| Prescaler | 0 | No prescaling for encoders |
| Auto-reload | 0xFFFF | Full 16-bit range |

## Notes and Warnings

- [!note] Ensure proper grounding of encoder cable shield to reduce noise
- [!note] Place encoder close to microcontroller or use twisted pair cables for longer distances
- [!warning] Do not configure the QEI timer for any other purpose


## Common Issues and Solutions

### Counter Not Incrementing
- Verify you are initialzing the port
- Check encoder wiring and connections

