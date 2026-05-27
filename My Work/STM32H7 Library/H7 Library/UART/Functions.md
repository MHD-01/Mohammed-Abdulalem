# UART Functions Reference

This document describes the main functions available for UART communication in H7Lib1.0.

## Initialization Functions

### `void H7_UART_Init(H7_UARTHandler_s *handler, UART_HandleTypeDef *huart)`
Initializes the UART handler structure with the provided HAL handle.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `huart`: Pointer to HAL UART handle

**Returns:** None

---

### `void H7_UART_DeInit(H7_UARTHandler_s *handler)`
De-initializes the UART peripheral.

**Parameters:**
- `handler`: Pointer to UART handler structure

**Returns:** None

---

## Data Transmission Functions

### `H7_state_e H7_UART_Transmit(H7_UARTHandler_s *handler, uint8_t *data, uint16_t size, uint32_t timeout)`
Transmits data in polling mode.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `data`: Pointer to data buffer
- `size`: Number of bytes to transmit
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_UART_Transmit_IT(H7_UARTHandler_s *handler, uint8_t *data, uint16_t size)`
Transmits data using interrupts.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `data`: Pointer to data buffer
- `size`: Number of bytes to transmit

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_UART_Transmit_DMA(H7_UARTHandler_s *handler, uint8_t *data, uint16_t size)`
Transmits data using DMA.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `data`: Pointer to data buffer (must be in DMA-accessible memory)
- `size`: Number of bytes to transmit

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Data Reception Functions

### `H7_state_e H7_UART_Receive(H7_UARTHandler_s *handler, uint8_t *data, uint16_t size, uint32_t timeout)`
Receives data in polling mode.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `data`: Pointer to receive buffer
- `size`: Number of bytes to receive
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_UART_Receive_IT(H7_UARTHandler_s *handler, uint8_t *data, uint16_t size)`
Receives data using interrupts.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `data`: Pointer to receive buffer
- `size`: Number of bytes to receive

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_UART_Receive_DMA(H7_UARTHandler_s *handler, uint8_t *data, uint16_t size)`
Receives data using DMA.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `data`: Pointer to receive buffer (must be in DMA-accessible memory)
- `size`: Number of bytes to receive

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Character I/O Functions

### `H7_state_e H7_UART_Putchar(H7_UARTHandler_s *handler, uint8_t ch)`
Transmits a single character.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `ch`: Character to transmit

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_UART_Getchar(H7_UARTHandler_s *handler, uint8_t *ch, uint32_t timeout)`
Receives a single character.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `ch`: Pointer to character variable
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

## String I/O Functions

### `H7_state_e H7_UART_Puts(H7_UARTHandler_s *handler, const char *str, uint32_t timeout)`
Transmits a null-terminated string.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `str`: Pointer to string
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

### `H7_state_e H7_UART_Gets(H7_UARTHandler_s *handler, char *str, uint16_t size, uint32_t timeout)`
Receives a string (until newline or buffer full).

**Parameters:**
- `handler`: Pointer to UART handler structure
- `str`: Pointer to string buffer
- `size`: Maximum string size
- `timeout`: Timeout in milliseconds

**Returns:** H7_OK on success, H7_ERROR on failure

---

## Formatted I/O Functions

### `int H7_UART_Printf(H7_UARTHandler_s *handler, const char *format, ...)`
Sends formatted output (printf-style).

**Parameters:**
- `handler`: Pointer to UART handler structure
- `format`: Format string
- `...`: Variable arguments

**Returns:** Number of characters sent

---

## Flow Control Functions

### `void H7_UART_Enable_RTS(H7_UARTHandler_s *handler)`
Enables RTS (Request To Send) signal.

**Parameters:**
- `handler`: Pointer to UART handler structure

**Returns:** None

---

### `void H7_UART_Disable_RTS(H7_UARTHandler_s *handler)`
Disables RTS signal.

**Parameters:**
- `handler`: Pointer to UART handler structure

**Returns:** None

---

## Status and Configuration Functions

### `H7_state_e H7_UART_Get_Status(H7_UARTHandler_s *handler)`
Gets current status of UART peripheral.

**Parameters:**
- `handler`: Pointer to UART handler structure

**Returns:** H7_OK, H7_BUSY, or H7_ERROR

---

### `void H7_UART_Set_Baudrate(H7_UARTHandler_s *handler, uint32_t baudrate)`
Sets the UART baud rate.

**Parameters:**
- `handler`: Pointer to UART handler structure
- `baudrate`: Baud rate in bits per second

**Returns:** None

---

## Interrupt Enable/Disable Functions

### `void H7_UART_Enable_RxIT(H7_UARTHandler_s *handler)`
Enables UART receive interrupt for idle line detection.

**Parameters:**
- `handler`: Pointer to UART handler structure

**Returns:** None

---

### `void H7_UART_Disable_RxIT(H7_UARTHandler_s *handler)`
Disables UART receive interrupt.

**Parameters:**
- `handler`: Pointer to UART handler structure

**Returns:** None

---

## Callback Functions

### `void H7_UART_TxCplt_Callback(H7_UARTHandler_s *handler)`
Called when UART transmission is complete. Implement in callbacks module.

---

### `void H7_UART_RxCplt_Callback(H7_UARTHandler_s *handler)`
Called when UART reception is complete. Implement in callbacks module.

---

### `void H7_UART_RxIdle_Callback(H7_UARTHandler_s *handler)`
Called when UART idle line is detected. Implement in callbacks module.

---

### `void H7_UART_Error_Callback(H7_UARTHandler_s *handler)`
Called on UART error (overrun, framing, parity). Implement in callbacks module.

---

## Example Usage

```c
// Initialization
H7_UARTHandler_s uart_handler;
H7_UART_Init(&uart_handler, &huart1);
H7_UART_Set_Baudrate(&uart_handler, 115200);

// Simple transmission
uint8_t msg[] = "Hello, World!";
H7_UART_Transmit(&uart_handler, msg, sizeof(msg) - 1, 1000);

// Character I/O
H7_UART_Putchar(&uart_handler, 'A');

// String functions
H7_UART_Puts(&uart_handler, "Enter data:\n", 1000);
char buffer[64];
H7_UART_Gets(&uart_handler, buffer, sizeof(buffer), 5000);

// Formatted output
H7_UART_Printf(&uart_handler, "Value: %d\n", 42);

// DMA reception for continuous data
DMA_BUFFER uint8_t rx_dma_buffer[256];
H7_UART_Receive_DMA(&uart_handler, rx_dma_buffer, 256);
H7_UART_Enable_RxIT(&uart_handler);
```

