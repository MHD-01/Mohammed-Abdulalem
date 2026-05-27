# I2C Functions Reference

This document describes the main functions available for I2C communication in H7Lib1.0.

## Initialization Functions

### `void H7_I2C_Init(H7_I2CHandler_s *handler, I2C_HandleTypeDef *hi2c)`
Initializes the I2C handler structure with the provided HAL handle.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `hi2c`: Pointer to HAL I2C handle

**Returns:** None

---

### `void H7_I2C_DeInit(H7_I2CHandler_s *handler)`
De-initializes the I2C peripheral.

**Parameters:**
- `handler`: Pointer to I2C handler structure

**Returns:** None

---

## Data Transmission Functions

### `H7_state_e H7_I2C_Master_Transmit(H7_I2CHandler_s *handler, uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout)`
Transmits data in polling mode.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address (7-bit or 10-bit)
- `data`: Pointer to data buffer
- `size`: Number of bytes to transmit
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_I2C_Master_Transmit_IT(H7_I2CHandler_s *handler, uint8_t address, uint8_t *data, uint16_t size)`
Transmits data using interrupts.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address
- `data`: Pointer to data buffer
- `size`: Number of bytes to transmit

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Data Reception Functions

### `H7_state_e H7_I2C_Master_Receive(H7_I2CHandler_s *handler, uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout)`
Receives data in polling mode.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address
- `data`: Pointer to receive buffer
- `size`: Number of bytes to receive
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_I2C_Master_Receive_IT(H7_I2CHandler_s *handler, uint8_t address, uint8_t *data, uint16_t size)`
Receives data using interrupts.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address
- `data`: Pointer to receive buffer
- `size`: Number of bytes to receive

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Combined Transfer Functions

### `H7_state_e H7_I2C_Mem_Write(H7_I2CHandler_s *handler, uint8_t address, uint8_t mem_addr, uint8_t *data, uint16_t size, uint32_t timeout)`
Writes to memory location in I2C slave device.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address
- `mem_addr`: Memory address in slave device
- `data`: Pointer to data buffer
- `size`: Number of bytes to write
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_I2C_Mem_Read(H7_I2CHandler_s *handler, uint8_t address, uint8_t mem_addr, uint8_t *data, uint16_t size, uint32_t timeout)`
Reads from memory location in I2C slave device.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address
- `mem_addr`: Memory address in slave device
- `data`: Pointer to receive buffer
- `size`: Number of bytes to read
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Status and Configuration Functions

### `H7_state_e H7_I2C_Get_Status(H7_I2CHandler_s *handler)`
Gets current status of I2C peripheral.

**Parameters:**
- `handler`: Pointer to I2C handler structure

**Returns:** H7_OK, H7_BUSY, or H7_ERROR

---

### `void H7_I2C_Set_Address(H7_I2CHandler_s *handler, uint8_t address)`
Sets the I2C slave address for communication.

**Parameters:**
- `handler`: Pointer to I2C handler structure
- `address`: I2C slave address

**Returns:** None

---

## Callback Functions

### `void H7_I2C_TxCplt_Callback(H7_I2CHandler_s *handler)`
Called when I2C transmission is complete. Implement in callbacks module.

---

### `void H7_I2C_RxCplt_Callback(H7_I2CHandler_s *handler)`
Called when I2C reception is complete. Implement in callbacks module.

---

### `void H7_I2C_Error_Callback(H7_I2CHandler_s *handler)`
Called on I2C error. Implement in callbacks module.

---

## Example Usage

```c
// Initialization
H7_I2CHandler_s i2c_handler;
H7_I2C_Init(&i2c_handler, &hi2c1);

// Set slave address
H7_I2C_Set_Address(&i2c_handler, 0x68);  // Example: MPU6050

// Transmit data
uint8_t tx_buffer[2] = {0x6B, 0x00};  // Register, Value
H7_I2C_Master_Transmit(&i2c_handler, 0x68, tx_buffer, 2, 1000);

// Receive data
uint8_t rx_buffer[6] = {0};
H7_I2C_Master_Receive(&i2c_handler, 0x68, rx_buffer, 6, 1000);
```

