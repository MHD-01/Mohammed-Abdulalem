# I2C Functions Reference

This document describes the main functions available for I2C communication in H7Lib1.0.

## Initialization Functions

### `H7_state_e H7_i2cx_init_struct(H7_i2cHandler_s *i2c, I2C_HandleTypeDef *hi2c)`
Initialize the `H7_i2cHandler_s` structure and link it to the HAL handle.

**Parameters:**
- `i2c`: Pointer to `H7_i2cHandler_s`
- `hi2c`: Pointer to HAL `I2C_HandleTypeDef`

**Returns:** `H7_state_e` (current I2C state)

---

### `H7_state_e H7_i2cx_init(H7_i2cHandler_s *i2c, u32 ownAdress, u32 addressingMode, H7_i2c_speed clockSpeed)`
Initialize the I2C peripheral with the specified address, addressing mode, and clock speed.

**Parameters:**
- `i2c`: Pointer to `H7_i2cHandler_s`
- `ownAddress`: The device address, either 7-bit or 10-bit
- `addressingMode`: 7-bit or 10-bit addressing mode
- `clockSpeed`: Communication speed (I2C clock)

**Returns:** `H7_state_e` (current I2C state)

---

## DMA initialization

### `H7_state_e H7_i2cx_DMA_RX_init(H7_i2cHandler_s *i2c, u32 DMA_mode, u32 ownAdress, u32 addressingMode, H7_i2c_speed clockSpeed)`
Initialize the I2C peripheral for DMA-based reception.

**Parameters:**
- `i2c`: Pointer to `H7_i2cHandler_s`
- `DMA_mode`: DMA transfer mode (e.g., `DMA_CIRCULAR` for continuous reception)
- `ownAddress`: The device address, either 7-bit or 10-bit
- `addressingMode`: 7-bit or 10-bit addressing mode
- `clockSpeed`: Communication speed (I2C clock)

**Returns:** `H7_state_e` (current I2C state)

[!NOTE] Call `H7_i2cx_init_struct` before using this function.

---

### `H7_state_e H7_i2cx_DMA_TX_init(H7_i2cHandler_s *i2c, u32 DMA_mode, u32 ownAdress, u32 addressingMode, H7_i2c_speed clockSpeed)`
Initialize the I2C peripheral for DMA-based transmission.

**Parameters:**
- `i2c`: Pointer to `H7_i2cHandler_s`
- `DMA_mode`: DMA transfer mode (e.g., `DMA_CIRCULAR` for continuous transmission)
- `ownAddress`: The device address, either 7-bit or 10-bit
- `addressingMode`: 7-bit or 10-bit addressing mode
- `clockSpeed`: Communication speed (I2C clock)

**Returns:** `H7_state_e` (current I2C state)

[!NOTE] Call `H7_i2cx_init_struct` before using this function.

---

## Callback Functions

### `void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)`
Called when a master transmit operation completes. Implement this callback in the callbacks module.

---

### `void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)`
Called when a master receive operation completes. Implement this callback in the callbacks module.

---

### `void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)`
Called when a slave transmit operation completes. Implement this callback in the callbacks module.

---

### `void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)`
Called when a memory write operation completes. Implement this callback in the callbacks module.

---

### `void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)`
Called when a memory read operation completes. Implement this callback in the callbacks module.

---

### `void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)`
Called when an I2C error occurs (timeout, arbitration loss, bus error, etc.). Implement this callback in the callbacks module.

---

