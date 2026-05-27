# QEI Functions Reference

This document describes the main functions available for Quadrature Encoder Interface (QEI) in H7Lib1.0.

## Initialization Functions

### `void H7_QEI_Init(H7_QEIHandler_s *handler, TIM_HandleTypeDef *htim)`
Initializes the QEI handler structure with the provided timer handle.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `htim`: Pointer to HAL Timer handle (configured for encoder mode)

**Returns:** None

---

### `void H7_QEI_DeInit(H7_QEIHandler_s *handler)`
De-initializes the QEI peripheral and stops the timer.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

### `void H7_QEI_Start(H7_QEIHandler_s *handler)`
Starts the QEI timer counting.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

### `void H7_QEI_Stop(H7_QEIHandler_s *handler)`
Stops the QEI timer counting.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

## Position Functions

### `uint32_t H7_QEI_Get_Count(H7_QEIHandler_s *handler)`
Gets the current encoder count value (0 to ARR).

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** Current counter value

---

### `int32_t H7_QEI_Get_Position(H7_QEIHandler_s *handler)`
Gets the absolute position including overflow counts.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** Absolute position value

---

### `void H7_QEI_Set_Position(H7_QEIHandler_s *handler, int32_t position)`
Sets the absolute position counter.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `position`: Position value to set

**Returns:** None

---

### `void H7_QEI_Reset_Position(H7_QEIHandler_s *handler)`
Resets position counter to zero.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

## Direction Functions

### `uint8_t H7_QEI_Get_Direction(H7_QEIHandler_s *handler)`
Gets the current rotation direction.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** 0 for forward, 1 for reverse

---

## Velocity Functions

### `void H7_QEI_Update_Velocity(H7_QEIHandler_s *handler, float delta_time_ms)`
Calculates velocity based on position change.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `delta_time_ms`: Time elapsed in milliseconds since last update

**Returns:** None

---

### `float H7_QEI_Get_Velocity(H7_QEIHandler_s *handler)`
Gets the calculated velocity (counts per millisecond).

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** Velocity value

---

### `float H7_QEI_Get_RPM(H7_QEIHandler_s *handler, uint16_t pulses_per_rev)`
Calculates and returns RPM based on velocity.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `pulses_per_rev`: Encoder pulses per revolution

**Returns:** RPM value

---

### `float H7_QEI_Get_Linear_Velocity(H7_QEIHandler_s *handler, float wheel_diameter_mm, uint16_t pulses_per_rev)`
Calculates linear velocity from rotation.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `wheel_diameter_mm`: Wheel diameter in millimeters
- `pulses_per_rev`: Encoder pulses per revolution

**Returns:** Linear velocity in mm/ms (or mm/s if scaled)

---

## Overflow Handling Functions

### `int32_t H7_QEI_Get_Overflow_Count(H7_QEIHandler_s *handler)`
Gets the number of complete revolutions (overflows).

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** Number of overflows

---

### `void H7_QEI_Reset_Overflow(H7_QEIHandler_s *handler)`
Resets the overflow counter.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

## Configuration Functions

### `void H7_QEI_Set_Max_Count(H7_QEIHandler_s *handler, uint32_t max_count)`
Sets the maximum count value (auto-reload register).

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `max_count`: Maximum count value (typically 0xFFFF for 16-bit counter)

**Returns:** None

---

### `uint32_t H7_QEI_Get_Max_Count(H7_QEIHandler_s *handler)`
Gets the maximum count value (auto-reload register).

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** Maximum count value

---

## Distance/Position Calculation Functions

### `int32_t H7_QEI_Get_Distance_Counts(H7_QEIHandler_s *handler, int32_t reference_position)`
Gets distance traveled since reference position in counts.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `reference_position`: Reference position to measure from

**Returns:** Distance in encoder counts

---

### `float H7_QEI_Get_Distance_mm(H7_QEIHandler_s *handler, int32_t reference_position, float wheel_diameter_mm, uint16_t pulses_per_rev)`
Calculates distance traveled in millimeters.

**Parameters:**
- `handler`: Pointer to QEI handler structure
- `reference_position`: Reference position to measure from
- `wheel_diameter_mm`: Wheel diameter in millimeters
- `pulses_per_rev`: Encoder pulses per revolution

**Returns:** Distance in millimeters

---

## Status Functions

### `H7_state_e H7_QEI_Get_Status(H7_QEIHandler_s *handler)`
Gets current status of QEI peripheral.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** H7_OK, H7_BUSY, or H7_ERROR

---

## Interrupt Functions

### `void H7_QEI_Enable_Overflow_IT(H7_QEIHandler_s *handler)`
Enables overflow interrupt for revolution counting.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

### `void H7_QEI_Disable_Overflow_IT(H7_QEIHandler_s *handler)`
Disables overflow interrupt.

**Parameters:**
- `handler`: Pointer to QEI handler structure

**Returns:** None

---

## Callback Functions

### `void H7_QEI_Overflow_Callback(H7_QEIHandler_s *handler)`
Called when timer overflow occurs. Implement in callbacks module.

---

### `void H7_QEI_Error_Callback(H7_QEIHandler_s *handler)`
Called on QEI error. Implement in callbacks module.

---

## Example Usage

```c
// Initialization
H7_QEIHandler_s qei_handler;
H7_QEI_Init(&qei_handler, &htim2);
H7_QEI_Enable_Overflow_IT(&qei_handler);
H7_QEI_Start(&qei_handler);

// Get position
int32_t position = H7_QEI_Get_Position(&qei_handler);

// Reset position
H7_QEI_Reset_Position(&qei_handler);

// Update velocity every 10ms
H7_QEI_Update_Velocity(&qei_handler, 10.0);

// Get velocity metrics
float velocity = H7_QEI_Get_Velocity(&qei_handler);
float rpm = H7_QEI_Get_RPM(&qei_handler, 100);  // 100 PPR encoder
float linear_vel = H7_QEI_Get_Linear_Velocity(&qei_handler, 100.0, 100);  // 100mm wheel dia

// Calculate distance traveled
int32_t reference_pos = H7_QEI_Get_Position(&qei_handler);
// ... move the robot ...
float distance = H7_QEI_Get_Distance_mm(&qei_handler, reference_pos, 100.0, 100);

// Check direction
if (H7_QEI_Get_Direction(&qei_handler) == 0) {
	// Moving forward
} else {
	// Moving backward
}
```

