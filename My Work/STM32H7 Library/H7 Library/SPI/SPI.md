# SPI (Serial Peripheral Interface)

Generally in H7Lib1.0, most peripherals (if not all), have their own ==Peripheral Handle Structure==.

## Physical Ports

![SPI Physical Ports](../attachment/spi_ports.jpg)

- SPI1: PA5 (SCK), PA6 (MISO), PA7 (MOSI), PA4 (NSS)
- SPI2: PB10 (SCK), PC2 (MISO), PC3 (MOSI), PB9 (NSS)
- SPI3: PC10 (SCK), PC11 (MISO), PC12 (MOSI), PA15 (NSS)
- SPI4: PE2 (SCK), PE5 (MISO), PE6 (MOSI), PE4 (NSS)
- SPI5: PF7 (SCK), PF8 (MISO), PF9 (MOSI), PF6 (NSS)
- SPI6: PG13 (SCK), PG12 (MISO), PG14 (MOSI), PG8 (NSS)

## Files

![SPI Files Diagram](../attachment/spi.png)

## Peripheral Handle Structure

```c
typedef struct{
	SPI_HandleTypeDef *hspi;
	u8 txData[512];
	u8 rxData[512];
	uint16_t txSize;
	uint16_t rxSize;
	uint8_t cs_pin;
	GPIO_TypeDef *cs_port;
	H7_state_e status;
} H7_SPIHandler_s;
```

- The user is recommended to use the handler for any use of the SPI peripheral, to ensure smooth functionality.
- *txData/rxData*: Transmit and receive data buffers
- *txSize/rxSize*: Size of data to be transmitted or received
- *cs_pin/cs_port*: Chip Select pin and port configuration
- [!warning] These variables should be used for debugging purposes only and should not be modified directly.

## How to Use SPI

### Enable the SPI Port

- First thing is to define the SPI port we are going to use.
- This can be done in SPI.h, by uncommenting the macro.

```c
#define SPI_PORT 1  // or 2, 3, 4, 5, 6
```

### SPI Initialization in adapter.c

- The initialization function should be called during the adapter module initialization phase.
- Ensure that SPI clock is enabled and GPIO pins are configured properly for the required mode.

### SPI Communication

#### Polling Mode
```c
// Transmit
HAL_SPI_Transmit(&hspi, txData, size, timeout);

// Receive
HAL_SPI_Receive(&hspi, rxData, size, timeout);

// Transmit and Receive (Full Duplex)
HAL_SPI_TransmitReceive(&hspi, txData, rxData, size, timeout);
```

#### Interrupt Mode
```c
// Transmit with interrupts
HAL_SPI_Transmit_IT(&hspi, txData, size);

// Receive with interrupts
HAL_SPI_Receive_IT(&hspi, rxData, size);

// Full duplex with interrupts
HAL_SPI_TransmitReceive_IT(&hspi, txData, rxData, size);
```

#### DMA Mode

- If using DMA with SPI, declare your buffers with the *DMA_BUFFER* macro from [H7_system] header file.
- This ensures DMA has access to RAM_D2 for proper data transfer.

```c
DMA_BUFFER u8 spiTxBuffer[512];
DMA_BUFFER u8 spiRxBuffer[512];
```

## Common SPI Configurations

| Parameter | Value | Notes |
|-----------|-------|-------|
| Clock Polarity (CPOL) | 0 | SCK idle low |
| Clock Polarity (CPOL) | 1 | SCK idle high |
| Clock Phase (CPHA) | 0 | Data sampled on first edge |
| Clock Phase (CPHA) | 1 | Data sampled on second edge |
| Baud Rate | Configurable | Up to 45 MHz for SPI1/2/3/4/5/6 |
| Data Size | 4-16 bits | Commonly 8 bits |
| Direction | Full Duplex | Default mode |
| Direction | Half Duplex | RX or TX only |

## Chip Select (CS) Management

- Manual CS handling:
```c
// Pull CS low
HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

// SPI communication
HAL_SPI_Transmit(&hspi, data, size, timeout);

// Pull CS high
HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
```

## Notes and Warnings

- [!note] NSS (CS) line should be managed by software for most applications
- [!note] Ensure proper matching of CPOL and CPHA with slave device
- [!warning] Do not perform complex processing in SPI interrupt callbacks
- [!warning] Keep SPI transmission line as short as possible to minimize EMI
- [!warning] Terminate unused MOSI line if operating in receive-only mode

## Common Issues and Solutions

### No Data Reception
- Verify SPI clock configuration matches slave device
- Check CPOL and CPHA settings
- Ensure CS line is properly managed

### Data Corruption
- Reduce SPI clock speed if noise is present
- Add decoupling capacitors near SPI device
- Check PCB routing and shielding

