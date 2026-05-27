# SPI (Serial Peripheral Interface)

Generally in H7Lib1.0, most peripherals (if not all), have their own ==Peripheral Handle Structure==.

## Physical Ports

![SPI Physical Ports](../attachment/spi_ports.jpg)

- SPI2: PD3 (SCK), PB14 (MISO), PB15 (MOSI), PB9 (NSS)


## Files

- spi.c
- spi.h

## Peripheral Handle Structure

```c
typedef struct{
	SPI_HandleTypeDef *hspi;

	DMA_HandleTypeDef hdma_spi_rx;
	DMA_HandleTypeDef hdma_spi_tx;

	u8 rxData[SPI_RX_BUF_SIZE];
	u8 txData[SPI_TX_BUF_SIZE];
	// DMA Buffers
	DMA_BUFFER u8 rxData_DMA[SPI_RX_BUF_SIZE];
	DMA_BUFFER u8 txData_DMA[SPI_TX_BUF_SIZE];

	H7_state_e status;
} H7_SPIHandler_s;
```

- The user must use the handler for any use of the SPI peripheral, to ensure smooth functionality and traceability.
- *txData/rxData*: Transmit and receive data buffers
- *txData_DMA/rxData_DMA*: Transmit and receive data buffers for DMA use
- *SPI_RX_BUF_SIZE/SPI_TX_BUF_SIZE*: Macro to indicate size of data to be transmitted or received
- [!warning] In case of using DMA, You have to use the <mark>_DMA_BUFFER_</mark> macro before initializing or defining the variable as this will allocate memory in RAM_D2 which is 32Kb size reserved for DMA.

## How to Use SPI

### Initialization

- Initialize the structure

- Example 
```c
H7_struct_init(&h7spi2, &spi2);
```

- Initialize the peripheral

- Example, Master SPI
```c
H7_SPIx_Init(&h7spi2, SPI_MODE_SLAVE, SPI_SPEED_25MHZ, SPI_DATA_SIZE_8B);
```

### SPI Initialization in adapter.c

- The initialization function should be called during the adapter module initialization phase.

### Characteristics of H7_SPIx_Init
- Inturrupts is Enabled.

| Parameter | Value | Notes |
|-----------|-------|-------|
| Mode | Configurable | Master Slave |
| Direction | Full Duplex | Default mode |
| Clock Polarity (CPOL) | SPI_POLARITY_LOW | SCK idle low |
| Clock Phase (CPHA) | SPI_PHASE_1EDGE | Data sampled on first edge |
| Baud Rate | Configurable | Up to 100MHz, devided by half as you go down until 781.25 KHz |
| Data Size | Configurable | Commonly 8 bits |

### SPI DMA

- The library supports the use of DMA for Tx and Rx.
- Remember to use <mark>_DMA_BUFFER_</mark> as the attribute of your buffers, or use the peripheral handler structre's buffers.
```c
H7_state_e H7_SPIx_rx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize);
H7_state_e H7_SPIx_tx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize);
```

## Chip Select (CS) Management

- [!NOTE] Chip select should be handled by user, example:
```c
// Pull CS low
HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

// SPI communication
HAL_SPI_Transmit(&hspi, data, size, timeout);

// Pull CS high
HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
```

## Clock Configuration and speed

- Equation for calculating SPI speed
$ SPI Speed(MHz) = ((((8 / PLL3M) * PLL3N) / PLL3P) / SPI_BAUDRATEPRESCALER)$

- Clock Configuration of SPI is in H7_MSP.c file
```c
PeriphClkInitStruct.PLL3.PLL3M = 4;
PeriphClkInitStruct.PLL3.PLL3N = 100;
PeriphClkInitStruct.PLL3.PLL3P = 1;
```
- SPI_BAUDRATEPRESCALER, is set inside [H7_SPIx_Init](### Initialization) and configured by <mark>_H7_SPI_speed_e_</mark> enum.
```c
typedef enum{
	SPI_SLAVE			= SPI_BAUDRATEPRESCALER_2,	// Speed is ignored in slave mode
	SPI_SPEED_100MHZ 	= SPI_BAUDRATEPRESCALER_2,
	SPI_SPEED_50MHZ		= SPI_BAUDRATEPRESCALER_4,
	SPI_SPEED_25MHZ		= SPI_BAUDRATEPRESCALER_8,
	SPI_SPEED_12_5MHZ	= SPI_BAUDRATEPRESCALER_16,	// 12.5 MHz
	SPI_SPEED_6_25MHZ	= SPI_BAUDRATEPRESCALER_32,	// 6.25 MHz
	SPI_SPEED_3_125MHZ	= SPI_BAUDRATEPRESCALER_64,	// 3.125 MHz
	SPI_SPEED_1_5625MHZ	= SPI_BAUDRATEPRESCALER_128,// 1.1625 MHz
	SPI_SPEED_781_25KHZ	= SPI_BAUDRATEPRESCALER_256	// 781.25 KHz
} H7_SPI_speed_e;
```

## Notes and Warnings

- [!NOTE] Chip select should be handled by user
- [!WARNING] Use <mark>_DMA_BUFFER_</mark> as the attribute of your buffers, in case you are using DMA
- [!WARNING] Do not perform complex processing in SPI interrupt callbacks


## Common Issues and Solutions

### No Data Reception
- Verify SPI clock configuration matches slave device
- Check CPOL and CPHA settings
- Ensure CS line is properly managed

### Data Corruption
- Reduce SPI clock speed if noise is present
- Add decoupling capacitors near SPI device
- Check PCB routing and shielding
