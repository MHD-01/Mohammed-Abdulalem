

## DMA Application

When using DMA in H7Lib1.0, DMA data buffers must be declared with the `DMA_BUFFER` macro defined in `H7_system.h`.

- `DMA_BUFFER` places the buffer in DMA-capable memory, typically `RAM_D2`.
- The library reserves 32 KB for DMA buffers in the linker configuration.
- If a buffer is not declared with `DMA_BUFFER`, the DMA engine may not access it correctly, which can cause stale data or transfer failures.

### Example

```c
DMA_BUFFER u8 spi_rx_buffer[SPI_RX_BUF_SIZE];
DMA_BUFFER u8 uart_tx_buffer[UART_TX_BUF_SIZE];
```

### Notes

- Always declare DMA source and destination buffers with `DMA_BUFFER` before using them.
- This ensures the data is placed in the correct memory region for the STM32H7 DMA controller.
- If you experience transfer errors or corrupted data, verify that your DMA buffers are marked with `DMA_BUFFER`.
