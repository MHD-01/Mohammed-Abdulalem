# SPI (Serial Peripheral Interface)

This document describes the SPI peripheral usage within H7Lib1.0. The library follows a common pattern where each peripheral exposes a handler structure and helper functions.

## Physical Ports



- SPI2: PD3 (SCK), PB14 (MISO), PB15 (MOSI), PB9 (NSS)

## Files

- spi.c
- spi.h

[!NOTE] H7_SPI library is located at BIOS/COM/.

## Peripheral Handle Structure

```c
typedef struct{
	SPI_HandleTypeDef *hspi;

	DMA_HandleTypeDef hdma_spi_rx;
	DMA_HandleTypeDef hdma_spi_tx;

	u8 rxData[SPI_RX_BUF_SIZE];
	u8 txData[SPI_TX_BUF_SIZE];
	/* DMA buffers (pointers into DMA-accessible memory) */
	u8 *rxData_DMA;
	u8 *txData_DMA;

	H7_state_e status;
} H7_SPIHandler_s;
```

- Use the handler for all SPI operations to ensure consistent behavior and traceability.
- `txData` / `rxData`: CPU-accessible transmit/receive buffers.
- `txData_DMA` / `rxData_DMA`: pointers to DMA-accessible buffers (must point into `DMA_BUFFER` memory).
- `SPI_RX_BUF_SIZE` / `SPI_TX_BUF_SIZE`: Macros that define buffer sizes.
- [!NOTE] DMA buffers referenced from `H7_SPIHandler_s` should point into global `DMA_BUFFER` regions.
- [!WARNING] When using DMA, mark buffers with the `DMA_BUFFER` attribute before defining them; this places them in RAM_D2 (32 KB reserved for DMA).

## How to Use SPI

### Initialization

- Initialize the handler structure, for example:

```c
H7_SPIx_struct_init(&h7spi2, &spi2);
```

- Initialize the peripheral. Example (master/slave mode and data size are configurable):

```c
H7_SPIx_Init(&h7spi2, SPI_MODE_SLAVE, SPI_SPEED_25MHZ, SPI_DATA_SIZE_8B);
```

The SPI initialization should be called during the adapter module initialization phase (see `adapter.c`).

### Characteristics of `H7_SPIx_Init`

- Interrupts are enabled by default.

| Parameter | Value | Notes |
|-----------|-------|-------|
| Mode | Configurable | Master or Slave |
| Direction | Full Duplex | Default mode |
| Clock Polarity (CPOL) | SPI_POLARITY_LOW | SCK idle low |
| Clock Phase (CPHA) | SPI_PHASE_1EDGE | Data sampled on first edge |
| Baud Rate | Configurable | From 100MHz to base clock divided by prescaler down to 781.25 kHz |
| Data Size | Configurable | Commonly 8 bits |

### SPI with DMA

- The library supports DMA for both Tx and Rx. Use `DMA_BUFFER` for DMA-accessible arrays, or point the handler's DMA pointers to `DMA_BUFFER` regions.

```c
H7_state_e H7_SPIx_rx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize);
H7_state_e H7_SPIx_tx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize);
```

## Chip Select (CS) Management

- [!NOTE] The library does not manage CS pins automatically; the user should control CS lines. Example:

```c
// Pull CS low
HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

// SPI communication
HAL_SPI_Transmit(&hspi, data, size, timeout);

// Pull CS high
HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
```

## Clock Configuration and Speed

- Equation used for estimating SPI clock (MHz):
$$ SPI\ Speed(MHz) = \frac{\left(\frac{8}{PLL3M} \times PLL3N\right)}{PLL3P \times SPI\_BAUDRATEPRESCALER} $$

- SPI clock configuration is performed in `H7_MSP.c`:

```c
PeriphClkInitStruct.PLL3.PLL3M = 4;
PeriphClkInitStruct.PLL3.PLL3N = 100;
PeriphClkInitStruct.PLL3.PLL3P = 1;
```

- `SPI_BAUDRATEPRESCALER` is selected by the `H7_SPI_speed_e` enum used by `H7_SPIx_Init`.

```c
typedef enum{
	SPI_SLAVE            = SPI_BAUDRATEPRESCALER_2,    // Speed ignored in slave mode
	SPI_SPEED_100MHZ     = SPI_BAUDRATEPRESCALER_2,
	SPI_SPEED_50MHZ      = SPI_BAUDRATEPRESCALER_4,
	SPI_SPEED_25MHZ      = SPI_BAUDRATEPRESCALER_8,
	SPI_SPEED_12_5MHZ    = SPI_BAUDRATEPRESCALER_16,
	SPI_SPEED_6_25MHZ    = SPI_BAUDRATEPRESCALER_32,
	SPI_SPEED_3_125MHZ   = SPI_BAUDRATEPRESCALER_64,
	SPI_SPEED_1_5625MHZ  = SPI_BAUDRATEPRESCALER_128,
	SPI_SPEED_781_25KHZ  = SPI_BAUDRATEPRESCALER_256
} H7_SPI_speed_e;
```

## Notes and Warnings

- [!NOTE] Chip select lines must be managed and initialized as GPIO by the user.
- [!WARNING] Initialize the `H7_SPI` handler structure before initializing the peripheral.
- [!WARNING] Mark DMA buffers with `DMA_BUFFER` when using DMA to ensure they are placed in DMA-capable memory.
- [!WARNING] Avoid heavy processing inside SPI interrupt callbacks; set flags and handle processing in the main loop.

## Common Issues and Solutions

### No Data Reception
- Verify SPI clock configuration matches the slave device.
- Check CPOL and CPHA settings.
- Ensure CS line is asserted/deasserted correctly.

### Data Corruption
- Reduce SPI clock speed if noise is present.
- Add decoupling capacitors near the SPI device.
- Review PCB routing and shielding.
