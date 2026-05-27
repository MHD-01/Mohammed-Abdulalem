# SPI Functions Reference

This document describes the main functions available for SPI communication in H7Lib1.0.

## Initialization Functions

### `H7_state_e H7_struct_init(H7_SPIHandler_s *spi, SPI_HandleTypeDef *hspi)`
Initializes the H7_SPI handler structure.

**Parameters:**
- `spi`: Pointer to H7_SPI handler structure, it is already declared in the Header file
- `hspi`: Pointer to HAL SPI handle, it is already declared in the Header file

**Returns:** None

---

### `H7_state_e H7_SPIx_Init(H7_SPIHandler_s *spi, u32 spiMode, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize)`
Initializes the SPI peripheral.

**Parameters:**
- `spi`: Pointer to H7_SPI handler structure, it is already declared in the Header file
- `spiMode`: To indicate whether to be a Master or a slave, can find Macro in spi.h
- `spiSpeed`: To indicate spped of the communication, argument should be a member of @H7_SPI_speed_e
- `dataSize`: To indicate the size of the data in the communication, argument should be a member of @H7_SPI_dataSize_e

![NOTE] H7_struct_init function  must be called before calling this function
**Returns:** H7_SPI State

---

## DMA initialization

### `H7_state_e H7_SPIx_rx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize)`
Initialize the SPI peripheral with DMA Receiving.

**Parameters:**
- `spi`: Pointer to H7_SPI handler structure, it is already declared in the Header file
- `spiMode`: To indicate whether to be a Master or a slave, can find Macro in spi.h
- `DMA_mode`: To indicate how the DMA stores the data, for continuous receiving @DMA_CIRCULAR
- `priority`: To indicate the priority of the stream inside DMA controller, macros can be found in spi.h
- `spiSpeed`: To indicate spped of the communication, argument should be a member of @H7_SPI_speed_e
- `dataSize`: To indicate the size of the data in the communication, argument should be a member of @H7_SPI_dataSize_e

![NOTE] H7_struct_init function  must be called before calling this function
**Returns:** H7_SPI State
---

### `H7_state_e H7_SPIx_tx_DMA_init(H7_SPIHandler_s *spi, u32 spiMode, u32 DMA_mode, u32 priority, H7_SPI_speed_e spiSpeed, H7_SPI_dataSize_e dataSize)`
Initialize the SPI peripheral with DMA Transmitting.

**Parameters:**
- `spi`: Pointer to H7_SPI handler structure, it is already declared in the Header file
- `spiMode`: To indicate whether to be a Master or a slave, can find Macro in spi.h
- `DMA_mode`: To indicate how the DMA stores the data, for continuous receiving @DMA_CIRCULAR
- `priority`: To indicate the priority of the stream inside DMA controller, macros can be found in spi.h
- `spiSpeed`: To indicate spped of the communication, argument should be a member of @H7_SPI_speed_e
- `dataSize`: To indicate the size of the data in the communication, argument should be a member of @H7_SPI_dataSize_e

![NOTE] H7_struct_init function  must be called before calling this function
**Returns:** H7_SPI State

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

