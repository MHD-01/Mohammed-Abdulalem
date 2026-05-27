# UART (Universal Asynchronous Receiver/Transmitter)

Generally in H7Lib1.0, most peripherals (if not all), have their own ==Peripheral Handle Structure==.

## Physical Ports

![UART Physical Ports](../attachment/uart_ports.jpg)

- USART1: PA9 (TX), PA10 (RX)
- USART2: PA2 (TX), PA3 (RX)
- USART3: PB10 (TX), PB11 (RX)
- UART4: PA0 (TX), PA1 (RX)
- UART5: PC12 (TX), PD2 (RX)
- USART6: PC6 (TX), PC7 (RX)
- UART7: PE8 (TX), PE7 (RX)
- UART8: PE1 (TX), PE0 (RX)

## Files

![UART Files Diagram](../attachment/uart.png)

## Peripheral Handle Structure

```c
typedef struct{
	UART_HandleTypeDef *huart;
	u8 txData[256];
	u8 rxData[256];
	uint16_t txSize;
	uint16_t rxSize;
	uint32_t baudrate;
	H7_state_e status;
} H7_UARTHandler_s;
```

- The user is recommended to use the handler for any use of the UART peripheral, to ensure smooth functionality.
- *txData/rxData*: Transmit and receive data buffers
- *txSize/rxSize*: Size of data to be transmitted or received
- *baudrate*: Configured baud rate for serial communication
- [!warning] These variables should be used for debugging purposes only and should not be modified directly.

## How to Use UART

### Enable the UART Port

- First thing is to define the UART port we are going to use.
- This can be done in UART.h, by uncommenting the macro.

```c
#define UART_PORT 1  // or 2, 3, 4, 5, 6, 7, 8
```

### UART Initialization in adapter.c

- The initialization function should be called during the adapter module initialization phase.
- Ensure that UART clock is enabled and GPIO pins are configured for TX and RX.

### UART Communication

#### Polling Mode
```c
// Transmit
HAL_UART_Transmit(&huart, txData, size, timeout);

// Receive
HAL_UART_Receive(&huart, rxData, size, timeout);
```

#### Interrupt Mode
```c
// Transmit with interrupts
HAL_UART_Transmit_IT(&huart, txData, size);

// Receive with interrupts (single character)
HAL_UART_Receive_IT(&huart, &rxData, 1);

// Start idle line detection for frame reception
__HAL_UART_ENABLE_IT(&huart, UART_IT_IDLE);
```

#### DMA Mode

- If using DMA with UART, declare your buffers with the *DMA_BUFFER* macro from [H7_system] header file.
- This ensures DMA has access to RAM_D2 for proper data transfer.

```c
DMA_BUFFER u8 uartTxBuffer[256];
DMA_BUFFER u8 uartRxBuffer[256];
```

## Common UART Configurations

| Parameter | Value | Notes |
|-----------|-------|-------|
| Baud Rate | 9600 | Standard speed |
| Baud Rate | 115200 | High-speed communication |
| Data Width | 8 bits | Standard configuration |
| Stop Bits | 1 | Most common |
| Parity | None | Standard for robotic applications |
| Flow Control | None | For simple applications |
| Flow Control | CTS/RTS | For reliable communication |

## Common Use Cases

### Serial Terminal Communication
- Used for debugging and monitoring
- Typical baud rate: 115200
- No flow control required

### Communication with External Modules (GPS, IMU, etc.)
- Configure matching baud rate with device
- May require specific data format
- Consider using DMA for continuous reception

### Wireless Communication (Bluetooth, RF modules)
- Configure according to module specifications
- Often requires RTS/CTS flow control

## Interrupt Callback Handling

- Implement UART callbacks in the callbacks module:

```c
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// Transmission complete - set completion flag
	uart_tx_complete = 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// Reception complete - process received data
	uart_rx_complete = 1;
}
```

## Notes and Warnings

- [!note] Use DMA mode for continuous or high-frequency data reception
- [!note] Implement idle line detection for variable-length frame reception
- [!warning] Do not perform complex processing in UART interrupt callbacks
- [!warning] Ensure proper baud rate configuration to match connected device
- [!warning] Add pull-up resistors if communicating over longer distances

## Common Issues and Solutions

### Garbled Data Reception
- Verify baud rate matches connected device exactly
- Check for proper ground connection
- Ensure stable power supply to UART transceiver

### No Data Reception
- Verify RX pin is properly connected
- Check UART peripheral is enabled
- Ensure receive interrupts or DMA are properly configured

### Overflow Errors
- Switch to DMA mode for continuous reception
- Increase buffer size
- Reduce data rate if possible

