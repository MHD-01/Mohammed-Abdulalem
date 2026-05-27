# SPI Functions Reference

This document describes the main functions available for SPI communication in H7Lib1.0.

## Initialization Functions

### `void H7_SPI_Init(H7_SPIHandler_s *handler, SPI_HandleTypeDef *hspi)`
Initializes the SPI handler structure with the provided HAL handle.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `hspi`: Pointer to HAL SPI handle

**Returns:** None

---

### `void H7_SPI_DeInit(H7_SPIHandler_s *handler)`
De-initializes the SPI peripheral.

**Parameters:**
- `handler`: Pointer to SPI handler structure

**Returns:** None

---

## Data Transmission Functions

### `H7_state_e H7_SPI_Transmit(H7_SPIHandler_s *handler, uint8_t *data, uint16_t size, uint32_t timeout)`
Transmits data in polling mode.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `data`: Pointer to data buffer
- `size`: Number of bytes to transmit
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_SPI_Transmit_IT(H7_SPIHandler_s *handler, uint8_t *data, uint16_t size)`
Transmits data using interrupts.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `data`: Pointer to data buffer
- `size`: Number of bytes to transmit

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_SPI_Transmit_DMA(H7_SPIHandler_s *handler, uint8_t *data, uint16_t size)`
Transmits data using DMA.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `data`: Pointer to data buffer (must be in DMA-accessible memory)
- `size`: Number of bytes to transmit

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Data Reception Functions

### `H7_state_e H7_SPI_Receive(H7_SPIHandler_s *handler, uint8_t *data, uint16_t size, uint32_t timeout)`
Receives data in polling mode.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `data`: Pointer to receive buffer
- `size`: Number of bytes to receive
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_SPI_Receive_IT(H7_SPIHandler_s *handler, uint8_t *data, uint16_t size)`
Receives data using interrupts.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `data`: Pointer to receive buffer
- `size`: Number of bytes to receive

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_SPI_Receive_DMA(H7_SPIHandler_s *handler, uint8_t *data, uint16_t size)`
Receives data using DMA.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `data`: Pointer to receive buffer (must be in DMA-accessible memory)
- `size`: Number of bytes to receive

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Full Duplex Transfer Functions

### `H7_state_e H7_SPI_TransmitReceive(H7_SPIHandler_s *handler, uint8_t *tx_data, uint8_t *rx_data, uint16_t size, uint32_t timeout)`
Transmits and receives data simultaneously in polling mode.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `tx_data`: Pointer to transmit buffer
- `rx_data`: Pointer to receive buffer
- `size`: Number of bytes to transfer
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_SPI_TransmitReceive_IT(H7_SPIHandler_s *handler, uint8_t *tx_data, uint8_t *rx_data, uint16_t size)`
Transmits and receives data simultaneously using interrupts.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `tx_data`: Pointer to transmit buffer
- `rx_data`: Pointer to receive buffer
- `size`: Number of bytes to transfer

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_SPI_TransmitReceive_DMA(H7_SPIHandler_s *handler, uint8_t *tx_data, uint8_t *rx_data, uint16_t size)`
Transmits and receives data simultaneously using DMA.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `tx_data`: Pointer to transmit buffer (must be in DMA-accessible memory)
- `rx_data`: Pointer to receive buffer (must be in DMA-accessible memory)
- `size`: Number of bytes to transfer

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Chip Select Functions

### `void H7_SPI_CS_Low(H7_SPIHandler_s *handler)`
Pulls the Chip Select line low.

**Parameters:**
- `handler`: Pointer to SPI handler structure

**Returns:** None

---

### `void H7_SPI_CS_High(H7_SPIHandler_s *handler)`
Pulls the Chip Select line high.

**Parameters:**
- `handler`: Pointer to SPI handler structure

**Returns:** None

---

### `void H7_SPI_Set_CS_Pin(H7_SPIHandler_s *handler, GPIO_TypeDef *cs_port, uint16_t cs_pin)`
Configures the Chip Select pin and port.

**Parameters:**
- `handler`: Pointer to SPI handler structure
- `cs_port`: GPIO port of CS pin
- `cs_pin`: GPIO pin number

**Returns:** None

---

## Status and Configuration Functions

### `H7_state_e H7_SPI_Get_Status(H7_SPIHandler_s *handler)`
Gets current status of SPI peripheral.

**Parameters:**
- `handler`: Pointer to SPI handler structure

**Returns:** H7_OK, H7_BUSY, or H7_ERROR

---

## Callback Functions

### `void H7_SPI_TxCplt_Callback(H7_SPIHandler_s *handler)`
Called when SPI transmission is complete. Implement in callbacks module.

---

### `void H7_SPI_RxCplt_Callback(H7_SPIHandler_s *handler)`
Called when SPI reception is complete. Implement in callbacks module.

---

### `void H7_SPI_TxRxCplt_Callback(H7_SPIHandler_s *handler)`
Called when SPI full duplex transfer is complete. Implement in callbacks module.

---

### `void H7_SPI_Error_Callback(H7_SPIHandler_s *handler)`
Called on SPI error. Implement in callbacks module.

---

## Example Usage

```c
// Initialization
H7_SPIHandler_s spi_handler;
H7_SPI_Init(&spi_handler, &hspi1);

// Configure CS pin
H7_SPI_Set_CS_Pin(&spi_handler, GPIOA, GPIO_PIN_4);

// Simple read-write transaction
uint8_t cmd[1] = {0x0F};  // Command to read
uint8_t response[6] = {0};

H7_SPI_CS_Low(&spi_handler);
H7_SPI_Transmit(&spi_handler, cmd, 1, 1000);
H7_SPI_Receive(&spi_handler, response, 6, 1000);
H7_SPI_CS_High(&spi_handler);

// Full duplex example
uint8_t tx[4] = {0xAA, 0xBB, 0xCC, 0xDD};
uint8_t rx[4] = {0};
H7_SPI_TransmitReceive(&spi_handler, tx, rx, 4, 1000);
```

