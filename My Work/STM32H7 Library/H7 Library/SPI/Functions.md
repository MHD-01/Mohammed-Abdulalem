# SPI Functions Reference

This document describes the main functions available for SPI communication in H7Lib1.0.

## Initialization Functions

### `H7_state_e H7_struct_init(H7_SPIHandler_s *spi, SPI_HandleTypeDef *hspi)`
Initializes the `H7_SPIHandler_s` structure and links it to the HAL SPI handle.

**Parameters:**
- `spi`: Pointer to `H7_SPIHandler_s` (declared in the header file)
- `hspi`: Pointer to HAL `SPI_HandleTypeDef` (declared in the header file)

**Returns:** None

---

### `H7_state_e H7_SPIx_Init(H7_SPIHandler_s *spi, u32 spiMode, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize)`
Initializes the SPI peripheral according to the specified mode, speed, and data size.

**Parameters:**
- `spi`: Pointer to `H7_SPIHandler_s` (declared in the header file)
- `spiMode`: Master or Slave selection; see macros in `spi.h`
- `spiSpeed`: Communication speed; use a value from `H7_SPI_speed_e`
- `dataSize`: Data width; use a value from `H7_SPI_dataSize_e`

[!NOTE] `H7_struct_init` must be called before using this function.

**Returns:** `H7_state_e` (current SPI state)

---

## DMA initialization

### `H7_state_e H7_SPIx_rx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize)`
Initialize the SPI peripheral for DMA-based reception.

**Parameters:**
- `spi`: Pointer to `H7_SPIHandler_s` (declared in the header file)
- `spiMode`: Master or Slave selection; see macros in `spi.h`
- `DMA_mode`: DMA transfer mode (e.g., `DMA_CIRCULAR` for continuous reception)
- `priority`: DMA stream priority macro
- `spiSpeed`: Communication speed; use a value from `H7_SPI_speed_e`
- `dataSize`: Data width; use a value from `H7_SPI_dataSize_e`

[!NOTE] `H7_struct_init` must be called before using this function.

**Returns:** `H7_state_e` (current SPI state)
---

### `H7_state_e H7_SPIx_tx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize)`
Initialize the SPI peripheral for DMA-based transmission.

**Parameters:**
- `spi`: Pointer to `H7_SPIHandler_s` (declared in the header file)
- `spiMode`: Master or Slave selection; see macros in `spi.h`
- `DMA_mode`: DMA transfer mode
- `priority`: DMA stream priority macro
- `spiSpeed`: Communication speed; use a value from `H7_SPI_speed_e`
- `dataSize`: Data width; use a value from `H7_SPI_dataSize_e`

[!NOTE] `H7_struct_init` must be called before using this function.

**Returns:** `H7_state_e` (current SPI state)

---

## Callback Functions
- Callback functions are in Platform/callback.c
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

