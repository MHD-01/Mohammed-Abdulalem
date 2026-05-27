# UART (Universal Asynchronous Receiver/Transmitter)

This document describes the UART peripheral usage within H7Lib1.0. The library follows a common pattern where each peripheral exposes a handler structure and helper functions.

## Physical Ports




- UART4: PD1 (TX), PD0 (RX), Port UART3
- UART5: PC12 (TX), PD2 (RX), Port UART1
- UART7: PE8 (TX), PE7 (RX), Port UART2
- UART9: PD15 (TX), PD14  (RX), Port UART4
- [!NOTE] UART ports labeling in the PCB is different from actual peripheral number

## Files

- uart.c
- uart.h

[!NOTE] H7_UART library is located at BIOS/COM/.

## Peripheral Handle Structure

```c
typedef struct{
	UART_HandleTypeDef *huart;

	DMA_HandleTypeDef uart_dma_rx;
	DMA_HandleTypeDef uart_dma_tx;

	u8 rxData[UART_RX_BUF_SIZE];
	u8 txData[UART_TX_BUF_SIZE];
	// DMA Buffers
	u8 *rxData_DMA;
	u8 *txData_DMA;

	H7_state_e status;
} H7_UARTHandler_s;
```

- Use the handler for all UART operations to ensure consistent behavior and traceability.
- `txData` / `rxData`: CPU-accessible transmit/receive buffers.
- `txData_DMA` / `rxData_DMA`: pointers to DMA-accessible buffers (must point into `DMA_BUFFER` memory).
- `UART_RX_BUF_SIZE` / `UART_TX_BUF_SIZE`: Macros that define buffer sizes.
- [!NOTE] DMA buffers referenced from `H7_UARTHandler_s` should point into global `DMA_BUFFER` regions.
- [!WARNING] When using DMA, mark buffers with the `DMA_BUFFER` attribute before defining them; this ensures they are placed in RAM_D2 (32 KB reserved for DMA).


## How to Use UART

### Initialization

- Initialize the handler structure, for example:

```c
H7_UARTx_init_struct(&h7uart5, &huart5);
```

- Initialize the peripheral. Example (Baud Rate is configurable):

```c
H7_UARTx_init(&h7uart5, 115200);
```

The UART initialization should be called during the adapter module initialization phase (see `adapter.c`).

### Characteristics of `H7_UARTx_init`

- Interrupts are enabled by default.

| Parameter | Value | Notes |
|-----------|-------|-------|
| WordLength | UART_WORDLENGTH_8B | 8-bit |
| StopBits | UART_STOPBITS_1 | 1-bit |
| Parity | UART_PARITY_NONE | No parity bit |
| Mode | UART_MODE_TX_RX | Full-duplex |
| Baud Rate | Configurable | Common values(115200 or 9600) |

### UART with DMA

- The library supports DMA for both Tx and Rx. Use `DMA_BUFFER` for DMA-accessible arrays, or point the handler's DMA pointers to `DMA_BUFFER` regions.

```c
H7_state_e H7_UARTx_DMA_RX_Init(H7_UARTHandler_s *h7uart, u32 DMA_mode, u32 BaudRate);
H7_state_e H7_UARTx_DMA_TX_Init(H7_UARTHandler_s *h7uart, u32 DMA_mode, u32 BaudRate);
```


## Common Use Cases

### Serial Terminal Communication
- Used for debugging and monitoring
- Typical baud rate: 115200, 9600
- No flow control required

## Notes and Warnings

- [!NOTE] Use DMA mode for continuous or high-frequency data reception.
- [!NOTE] Implement idle line detection for variable-length frame reception.
- [!WARNING] Do not perform complex processing in UART interrupt callbacks; set flags instead.
- [!WARNING] Initialize the `H7_UARTHandler_s` structure before initializing the peripheral.
- [!WARNING] Ensure baud rate configuration matches the connected device exactly.
- [!WARNING] Add pull-up resistors when communicating over longer distances.

## Common Issues and Solutions

### Garbled Data Reception
- Verify baud rate matches connected device exactly
- Check for proper ground connection
- Ensure stable power supply to UART transceiver

### No Data Reception
- Verify RX pin is properly connected.
- Ensure the `H7_UARTHandler_s` structure is initialized before the peripheral is initialized.
- When using DMA, ensure buffers are marked with the `DMA_BUFFER` attribute for correct data communication.

### Overflow Errors
- Switch to DMA mode for continuous reception
- Increase buffer size
- Reduce data rate if possible

