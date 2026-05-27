# QEI Functions Reference

This document describes the main functions available for Quadrature Encoder Interface (QEI) in H7Lib1.0.

## Initialization Functions

### `voiH7_state_e H7_QEI_init(H7_QEIHandler_s *h7qei, TIM_HandleTypeDef *htim)`
Initializes the QEI handler structure and the peripheral.

**Parameters:**
- `h7qei`: Pointer to H7_QEI handler structure
- `htim`: Pointer to HAL Timer handle

**Returns:** `H7_state_e` (current QEI state)

---

### `vs32 H7_QEI_read(H7_QEIHandler_s *h7qei)`
Read the counter values.

**Parameters:**
- `h7qei`: Pointer to H7_QEI handler structure

**Returns:** `vs32` counter values

---

### `void H7_QEI_reset(H7_QEIHandler_s *h7qei)`
Resets the counter to 0.

**Parameters:**
- `h7qei`: Pointer to H7_QEI handler structure

**Returns:** None

