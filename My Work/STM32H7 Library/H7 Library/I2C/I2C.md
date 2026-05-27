# I2C (Inter-Integrated Circuit)

Generally in H7Lib1.0, most peripherals (if not all), have their own ==Peripheral Handle Structure==.

## Physical Ports

![I2C Physical Ports](../attachment/i2c_ports.jpg)

- I2C1: PB8 (SCL), PB9 (SDA)
- I2C2: PF1 (SCL), PF0 (SDA)
- I2C3: PH7 (SCL), PH8 (SDA)
- I2C4: PD12 (SCL), PD13 (SDA)

## Files

![I2C Files Diagram](../attachment/i2c.png)

## Peripheral Handle Structure

```c
typedef struct{
	I2C_HandleTypeDef *hi2c;
	u8 address;
	u8 txData[256];
	u8 rxData[256];
	uint16_t txSize;
	uint16_t rxSize;
	H7_state_e status;
} H7_I2CHandler_s;
```

- The user is recommended to use the handler for any use of the I2C peripheral, to ensure smooth functionality.
- *address*: I2C slave address configuration
- *txData/rxData*: Transmit and receive data buffers
- *txSize/rxSize*: Size of data to be transmitted or received
- [!warning] These variables should be used for debugging purposes only and should not be modified directly.

## How to Use I2C

### Enable the I2C Port

- First thing is to define the I2C port we are going to use.
- This can be done in I2C.h, by uncommenting the macro.

```c
#define I2C_PORT 1  // or 2, 3, 4
```

### I2C Initialization in adapter.c

- The initialization function should be called during the adapter module initialization phase.
- Ensure that I2C clock is enabled and GPIO pins are configured as open-drain with pull-up resistors.

### I2C Communication

#### Polling Mode
```c
// Transmit
HAL_I2C_Master_Transmit(&hi2c, address, txData, size, timeout);

// Receive
HAL_I2C_Master_Receive(&hi2c, address, rxData, size, timeout);
```

#### Interrupt Mode
```c
// Transmit with interrupts
HAL_I2C_Master_Transmit_IT(&hi2c, address, txData, size);

// Receive with interrupts
HAL_I2C_Master_Receive_IT(&hi2c, address, rxData, size);
```

#### DMA Mode

- If using DMA with I2C, declare your buffers with the *DMA_BUFFER* macro from [H7_system] header file.
- This ensures DMA has access to RAM_D2 for proper data transfer.

```c
DMA_BUFFER u8 i2cBuffer[256];
```

## Common I2C Configurations

| Parameter | Value | Notes |
|-----------|-------|-------|
| Speed Mode | Standard (100 kHz) | Default for most devices |
| Speed Mode | Fast (400 kHz) | For faster communication |
| Addressing Mode | 7-bit | Common for most I2C devices |
| Addressing Mode | 10-bit | For extended addressing |

## Notes and Warnings

- [!note] Use open-drain output configuration for SCL and SDA pins
- [!note] Pull-up resistors (typically 4.7kΩ) are required on both SCL and SDA lines
- [!warning] Do not perform complex processing in I2C interrupt callbacks; use flag variables instead
- [!warning] Ensure proper clock frequency configuration to match slave device requirements

## Common Issues and Solutions

### No ACK from Slave Device
- Verify slave address is correct
- Check if pull-up resistors are present and properly connected
- Ensure I2C peripheral is enabled in CubeMX

### Bus Hangup
- Reset I2C peripheral by disabling and re-enabling it
- Check for stuck SCL or SDA lines

