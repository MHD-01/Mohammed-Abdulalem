# QEI (Quadrature Encoder Interface)

Generally in H7Lib1.0, most peripherals (if not all), have their own ==Peripheral Handle Structure==.

## Overview

The Quadrature Encoder Interface (QEI) is implemented using Timer peripherals in counting mode to measure rotational motion from quadrature encoders. The STM32H7 provides multiple timers that can be configured as QEI interfaces.

## Physical Ports

![QEI Physical Ports](../attachment/qei_ports.jpg)

- Timer1: PA8 (CH1), PA9 (CH2) / PE9 (CH1), PE11 (CH2)
- Timer2: PA0 (CH1), PA1 (CH2) / PB10 (CH1), PB11 (CH2)
- Timer3: PA6 (CH1), PA7 (CH2) / PB4 (CH1), PB5 (CH2)
- Timer4: PB6 (CH1), PB7 (CH2)
- Timer5: PA0 (CH1), PA1 (CH2) / PH10 (CH1), PH12 (CH2)
- Timer8: PC6 (CH1), PC7 (CH2) / PI5 (CH1), PI7 (CH2)

## Files

![QEI Files Diagram](../attachment/qei.png)

## Peripheral Handle Structure

```c
typedef struct{
	TIM_HandleTypeDef *htim;
	uint32_t count;
	int32_t overflow_count;
	uint32_t max_count;
	int32_t position;
	float velocity;
	uint32_t last_count;
	H7_state_e status;
} H7_QEIHandler_s;
```

- The user is recommended to use the handler for any use of the QEI peripheral, to ensure smooth functionality.
- *count*: Current encoder count value (0 to ARR)
- *overflow_count*: Number of times the counter has overflowed
- *max_count*: Auto-reload register value (determines counter range)
- *position*: Total position calculated from count and overflow_count
- *velocity*: Calculated velocity from position change
- *last_count*: Previous count value for velocity calculation
- [!warning] These variables should be used for debugging purposes only and should not be modified directly.

## How to Use QEI

### Enable the QEI Timer

- First thing is to define the timer to be used for QEI.
- This can be done in QEI.h, by uncommenting the macro.

```c
#define QEI_TIMER 2  // or 1, 3, 4, 5, 8
```

### QEI Initialization in adapter.c

- The initialization function should be called during the adapter module initialization phase.
- Configure the timer in encoder interface mode (counting both edges of both channels)
- Ensure that GPIO pins are configured as inputs with proper speed settings

```c
// Timer should be configured as:
// - Counter Mode: Encoder Interface Mode (TIM_COUNTERMODE_UP)
// - Clock Polarity: Both edges
// - Channel 1: Input mode (CH1 input from A-phase)
// - Channel 2: Input mode (CH2 input from B-phase)
// - Auto-reload value: 0xFFFF (for 16-bit counting)
```

### QEI Position Tracking

#### Get Current Position
```c
uint32_t current_count = __HAL_TIM_GET_COUNTER(&htim);
int32_t total_position = (overflow_count << 16) | current_count;
```

#### Reset Position
```c
__HAL_TIM_SET_COUNTER(&htim, 0);
overflow_count = 0;
position = 0;
```

#### Calculate Velocity
```c
// Velocity calculation (counts per sample time)
float velocity = (position - last_position) / delta_time;
last_position = position;
```

## Encoder Types and Configuration

| Encoder Type | Pulses per Revolution | Configuration |
|--------------|----------------------|---------------|
| Standard Quadrature | 100 - 2000 PPR | Normal mode |
| High-resolution | 2000+ PPR | May require overflow handling |
| Single Channel | Any | Configure as pulse counter |

## Interrupt Handling

- Enable overflow interrupt to count complete revolutions:

```c
// Enable timer overflow interrupt
__HAL_TIM_ENABLE_IT(&htim, TIM_IT_UPDATE);

// In callback:
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)  // QEI Timer
	{
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim))
			overflow_count--;
		else
			overflow_count++;
	}
}
```

## Direction Detection

- The QEI peripheral automatically detects rotation direction:

```c
// Direction is reflected in the counter direction
// Increment = Forward rotation
// Decrement = Reverse rotation

// Get direction (simplified):
// Check if counter is incrementing or decrementing
uint8_t direction = (__HAL_TIM_GET_COUNTER(&htim) > last_count) ? 1 : 0;
```

## Common QEI Configurations

| Parameter | Value | Notes |
|-----------|-------|-------|
| Counting Mode | Encoder Interface | Use for quadrature encoders |
| Polarity | Both Edges | Capture both rising and falling edges |
| Channel 1 | Input from Phase A | First quadrature signal |
| Channel 2 | Input from Phase B | Second quadrature signal (90° offset) |
| Prescaler | 0 | No prescaling for encoders |
| Auto-reload | 0xFFFF | Full 16-bit range |

## Notes and Warnings

- [!note] Ensure proper grounding of encoder cable shield to reduce noise
- [!note] Add pull-up or pull-down resistors based on encoder output type (open collector or push-pull)
- [!note] Place encoder close to microcontroller or use twisted pair cables for longer distances
- [!warning] Do not configure the QEI timer for any other purpose
- [!warning] Always handle timer overflow events for accurate position tracking over multiple revolutions
- [!warning] Be aware of counter rollover when comparing position values

## Position Calculation Over Multiple Revolutions

```c
// Complete position calculation:
int32_t get_absolute_position(void)
{
	uint32_t current_count = __HAL_TIM_GET_COUNTER(&htim);
	int32_t total_position = (overflow_count * 65536) + current_count;
	return total_position;
}

// Distance in mm (example):
// pulses_per_revolution = 100 (from encoder spec)
// wheel_circumference = 314 mm (for 100mm diameter wheel)
float distance_mm = (get_absolute_position() / pulses_per_revolution) * wheel_circumference;
```

## Common Issues and Solutions

### Counter Not Incrementing
- Verify GPIO pins are properly configured as inputs
- Check encoder wiring and connections
- Verify timer is started with `__HAL_TIM_ENABLE(&htim)`

### Erratic Counting
- Add RC filter or capacitors to encoder signal lines
- Check for proper pull-up/pull-down resistor configuration
- Reduce noise with twisted pair cables and shielding

### Missed Counts
- Increase MCU clock speed if possible
- Ensure no other high-priority interrupts are blocking
- Reduce encoder signal debounce time

