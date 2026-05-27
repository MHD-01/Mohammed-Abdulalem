# UART Functions Reference

This document describes the main functions available for UART communication in H7Lib1.0.

## Initialization Functions

### `H7_state_e H7_UARTx_init_struct(H7_UARTHandler_s *h7uart, UART_HandleTypeDef *huart)`
Initialize the `H7_UARTHandler_s` structure and link it to the HAL handle.

**Parameters:**
- `h7uart`: Pointer to `H7_UARTHandler_s`
- `huart`: Pointer to HAL `UART_HandleTypeDef`

**Returns:** `H7_state_e` (current UART state)

---

### `H7_state_e H7_UARTx_init(H7_UARTHandler_s *h7uart, uint32_t BaudRate)`
Initialize the UART peripheral with the specified baud rate.

**Parameters:**
- `h7uart`: Pointer to `H7_UARTHandler_s`
- `BaudRate`: Baud rate in bits per second (e.g., 115200)

**Returns:** `H7_state_e` (current UART state)

[!NOTE] Call `H7_UARTx_init_struct` before using this function.

---

## DMA initialization

### `H7_state_e H7_UARTx_DMA_RX_Init(H7_UARTHandler_s *h7uart, u32 DMA_mode, u32 BaudRate)`
Initialize the UART peripheral for DMA-based reception.

**Parameters:**
- `h7uart`: Pointer to `H7_UARTHandler_s`
- `DMA_mode`: DMA transfer mode (e.g., `DMA_CIRCULAR` for continuous reception)
- `BaudRate`: Baud rate in bits per second

**Returns:** `H7_state_e` (current UART state)

[!NOTE] Call `H7_UARTx_init_struct` before using this function.

---

### `H7_state_e H7_UARTx_DMA_TX_Init(H7_UARTHandler_s *h7uart, u32 DMA_mode, u32 BaudRate)`
Initialize the UART peripheral for DMA-based transmission.

**Parameters:**
- `h7uart`: Pointer to `H7_UARTHandler_s`
- `DMA_mode`: DMA transfer mode (e.g., `DMA_CIRCULAR` for continuous transmission)
- `BaudRate`: Baud rate in bits per second

**Returns:** `H7_state_e` (current UART state)

[!NOTE] Call `H7_UARTx_init_struct` before using this function.

---

## Callback Functions
- Callback functions are in Platform/callback.c

### `void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)`
Called when UART transmission is complete. Implement in callbacks module.

---

### `void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)`
Called when UART reception is complete. Implement in callbacks module.

---

### `void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)`
Called on UART error (overrun, framing, parity). Implement in callbacks module.
