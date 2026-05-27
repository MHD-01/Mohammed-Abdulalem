# I2C (Inter-Integrated Circuit)

This document describes the I2C peripheral usage within H7Lib1.0. Each peripheral follows the same handler-based design and supports HAL-friendly initialization.

## Features

- Supports 7-bit and 10-bit I2C addressing
- Supports interrupt and DMA-based transfers
- Works with standard, fast, and fast-plus clock speeds
- Includes a peripheral handler structure for clean separation and state tracking

## Physical Ports



- I2C1: PB8 (SCL), PB7 (SDA)
- I2C5: PC11 (SCL), PC10 (SDA)

## Files

- i2c.c
- i2c.h

[!NOTE] H7_I2C library is located at BIOS/COM/.

## Peripheral Handle Structure

```c
typedef struct{
	I2C_HandleTypeDef *hi2c;

	DMA_HandleTypeDef hdma_i2c_rx;
	DMA_HandleTypeDef hdma_i2c_tx;

	u8 rxData[I2C_RX_BUF_SIZE];
	u8 txData[I2C_TX_BUF_SIZE];
	// DMA Buffers
	u8 *rxData_DMA;
	u8 *txData_DMA;

	H7_state_e status;
} H7_i2cHandler_s;
```

- Use the handler for all I2C operations to ensure consistent behavior and traceability.
- `txData` / `rxData`: CPU-accessible transmit/receive buffers.
- `txData_DMA` / `rxData_DMA`: pointers to DMA-accessible buffers (must point into `DMA_BUFFER` memory).
- `I2C_RX_BUF_SIZE` / `I2C_TX_BUF_SIZE`: Macros that define buffer sizes.
- [!NOTE] DMA buffers referenced from `H7_i2cHandler_s` should point into global `DMA_BUFFER` regions.
- [!WARNING] When using DMA, mark buffers with the `DMA_BUFFER` attribute before defining them; this ensures they are placed in RAM_D2 (32 KB reserved for DMA).

## How to Use I2C

### Initialization

- Initialize the handler structure, for example:

```c
H7_i2cx_init_struct(&h7i2c1, &hi2c1);
```

- Initialize the peripheral. Example (address, addressing mode, and speed are configurable):
```c
H7_i2cx_init(&h7i2c1, 8, I2C_ADDRESSINGMODE_7BIT, CLK_SPEED_400KHz);
```

[!NOTE] `H7_i2cx_init` handles bit shifting for 7-bit addresses automatically. For 10-bit addressing, pass the full 10-bit value according to the HAL convention.

The I2C initialization should be called during the adapter module initialization phase (see `adapter.c`).

### Characteristics of `H7_i2cx_init`

- Interrupts are enabled by default.

| Parameter | Value | Notes |
|-----------|-------|-------|
| Address | Configurable | Addresses range from 0 to 0x7F for 7-bit mode |
| Addressing Mode | Configurable | 7-bit or 10-bit |
| Clock Speed | Configurable | 100 kHz, 400 kHz, 1 MHz |
| Dual Address Mode | I2C_DUALADDRESS_DISABLE | Disabled |
| Analog Filter | I2C_ANALOGFILTER_ENABLE | Enabled |

### I2C with DMA

- The library supports DMA for both Tx and Rx. Use `DMA_BUFFER` for DMA-accessible arrays, or point the handler's DMA pointers to `DMA_BUFFER` regions.

```c
H7_state_e H7_i2cx_DMA_RX_init(H7_i2cHandler_s *i2c, u32 DMA_mode, u32 ownAdress, u32 addressingMode, H7_i2c_speed clockSpeed);
H7_state_e H7_i2cx_DMA_TX_init(H7_i2cHandler_s *i2c, u32 DMA_mode, u32 ownAdress, u32 addressingMode, H7_i2c_speed clockSpeed);
```


## Notes and Warnings

- [!NOTE] Pull-up resistors (typically 4.7 kΩ) are required on both SCL and SDA lines.
- [!NOTE] If 1 MHz communication is unstable or noise-sensitive, enable the digital filter in `H7_i2cx_init`, `H7_i2cx_DMA_RX_init`, or `H7_i2cx_DMA_TX_init`.
- [!WARNING] Do not perform complex processing in I2C interrupt callbacks; use flag variables instead.
- [!WARNING] Ensure proper clock frequency configuration to match the slave device requirements.

## Common Issues and Solutions

### No ACK from Slave Device
- Verify slave address is correct, not shifted in case of 7-bit address
- Check if pull-up resistors are present and properly connected
- Ensure the `H7_i2cHandler_s` structure is initialized before the peripheral is initialized.
- When using DMA, ensure buffers are marked with the `DMA_BUFFER` attribute for correct data communication.

### Bus Hangup
- Reset I2C peripheral by disabling and re-enabling it
- Check for stuck SCL or SDA lines

