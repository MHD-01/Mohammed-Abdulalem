@page _ BMM350_init()
# Initialization

```c
bmm350_status_e bmm350_init(struct bmm350_dev_s *dev, I2C_HandleTypeDef *hi2c);
```

* This internal function sets:

    * The I2C handler (hi2c)
    * The I2C address
    * Function pointers for read, write, and delay_us
    * Clears PMU command status memory

It also performs a null pointer check to ensure dev is not NULL.

* So at first the device structure is initialized with adaptive filter in this ine:
```c

rslt = bmm350_init_struct(dev, hi2c);

```

**note that in all functions uses "rslt" as a variable that always will hold the status of every operation. This to ensure stability and fast the debugging in case if fault.**

* Then we check if the device is ready for i2c communication, then check for dev structure, this Prevents segmentation faults.

```c
   if(rslt == BMM350_OK){
   	rslt = check_device(dev);
   	if(rslt == BMM350_OK){
    		rslt = null_ptr_check(dev);
   	}
   }
```

* After that there is a condition to check the validity of device structure.
* then initializing chip ID to 0.
* then enabling all axis as 7 is the value should be written to \ref BMM350_REG_PMU_CMD_AXIS_EN "BMM350_REG_PMU_CMD_AXIS_EN" register.

*  Delay.
```c
    if(rslt == BMM350_OK){
    	dev->chip_id = 0;

    	// Enable all axis
    	dev->axis_en = 0x07;

    	dev->delay_us(BMM350_START_UP_TIME_FROM_POR);
```

* Then we do soft reset by writing 0xB6 to the register (0x7E) \ref BMM350_REG_CMD "BMM350_REG_CMD", then check the operation validity and perform delay.

```c
if(rslt == BMM350_OK){
    /* Soft-reset */
    soft_reset = BMM350_CMD_SOFTRESET;

    /* Set the command in the command register */
    rslt = dev->write(dev, BMM350_REG_CMD, &soft_reset, 1);
    if(rslt == BMM350_OK){
        dev->delay_us(BMM350_SOFT_RESET_DELAY);
    }
}
```

* Then we read the chip ID to verify device presence, then check the validity of that ID it should be 0x33.
* If the ID is correct then we the function \ref otp_dump_after_boot() "otp_dump_after_boot()", "it is explained after this code".
* If that went ok, then power off the OTP by writing 0x80 to register (0x50) BMM350_REG_OTP_CMD_REG because it is not needed.
* Then reset the sensor using the function \ref bmm350_magnetic_reset_and_wait() "bmm350_magnetic_reset_and_wait()". "it is explained after otp_dump_after_boot() explanation"

```c
if(rslt == BMM350_OK){
    rslt = dev->read(dev, BMM350_REG_CHIP_ID, &chip_id, 1);
    if(rslt == BMM350_OK){
        dev->chip_id = chip_id;
    }
}

/* Check for chip id validity */
if((rslt == BMM350_OK) && (dev->chip_id == BMM350_CHIP_ID)){
    /* Download OTP memory */
    rslt = otp_dump_after_boot(dev);
    if(rslt == BMM350_OK){
            /* Power off OTP */
        rslt = dev->write(dev, BMM350_REG_OTP_CMD_REG, &otp_cmd, 1);
        dev->delay_us(BMM350_SOFT_RESET_DELAY);
        if(rslt == BMM350_OK){
            rslt = bmm350_magnetic_reset_and_wait(dev);
            dev->delay_us(BMM350_GOTO_SUSPEND_DELAY);
        }
    }
}else{
    rslt = BMM350_E_DEV_NOT_FOUND;
}
```

* \ref interrupt_config() "interrupt_config" this function configures the INT pin in the BMM350 Shuttle Board 3.0. its details is after bmm350_magnetic_reset_and_wait().

```c
if(rslt == BMM350_OK){
    rslt = interrupt_config(dev);
}
```

* \ref bmm350_odr_avg_config() "bmm350_odr_avg_config()", This function used to configures the odr (Output Data Rate) and avg (Measurements Averages) in register 0x04. First it specifies the values for ODR and AVG using Switch. Then it writes these values after masking them to register (0x06) \ref BMM350_REG_PMU_CMD "BMM350_REG_PMU_CMD", then perform a delay.

```c
bmm350_status_e bmm350_odr_avg_config(struct bmm350_dev_s *dev, uint16_t odr, uint16_t avg){
	bmm350_status_e rslt;

	uint8_t pmu_cmd_aggr = 0;
	uint8_t pmu_cmd_upd_oae = BMM350_PMU_CMD_UPD_OAE;
	switch(odr){
	case BMM350_ODR_400HZ:
	case BMM350_ODR_200HZ:
	case BMM350_ODR_100HZ:
	case BMM350_ODR_50HZ:
	case BMM350_ODR_25HZ:
	case BMM350_ODR_12_5HZ:
	case BMM350_ODR_6_25HZ:
	case BMM350_ODR_3_125HZ:
	case BMM350_ODR_1_5625HZ:
		rslt = BMM350_OK;
		break;
	default:
		rslt = BMM350_E_INV_ODR_AVG;
	}
	if(rslt == BMM350_OK){
		switch(avg){
		case BMM350_AVG_NO_AVG:
		case BMM350_AVG_2:
		case BMM350_AVG_4:
		case BMM350_AVG_8:
			rslt = BMM350_OK;
			break;
		default:
			rslt = BMM350_E_INV_ODR_AVG;
		}
	}

	if(rslt == BMM350_OK){
		pmu_cmd_aggr = (0xFF & ((avg << 4) | odr));
		rslt = dev->write(dev, BMM350_REG_PMU_CMD, &pmu_cmd_upd_oae, 1);
		dev->delay_us(BMM350_UPD_OAE_DELAY);
		if(rslt == BMM350_OK){
			rslt = dev->write(dev, BMM350_REG_PMU_CMD_AGGR_SET, &pmu_cmd_aggr, 1);
			dev->delay_us(300);
		}
	}
	return rslt;
}
```
* If this operation was successful then set the power mode to Normal_Mode, \ref bmm350_set_powermode() "bmm350_set_powermode" *is explained in bmm350_magnetic_reset_and_wait() below*.

* \ref bmm350_get_pmu_cmd_status() "bmm350_get_pmu_cmd_status()" this function reads both BMM350_REG_PMU_CMD_STATUS_0 and BMM350_REG_PMU_CMD_STATUS_1, and stores their values to the device structure.

```c
bmm350_status_e bmm350_get_pmu_cmd_status(struct bmm350_dev_s *dev, bmm350_pmu_cmd_status_s *pmu_cmd_stat){
	bmm350_status_e rslt;


    // Registros pmu_cmd_status
    uint8_t reg_pmu_cmd_status_0 = BMM350_REG_PMU_CMD_STATUS_0;
    uint8_t reg_pmu_cmd_status_1 = BMM350_REG_PMU_CMD_STATUS_1;

    uint8_t reg_data[12];

    if (pmu_cmd_stat != NULL)
    {
        /* Get PMU command status 0 data */
        rslt = dev->read(dev, 0x00, reg_data, sizeof(reg_data));

        if (rslt == BMM350_OK)
        {
        	// PMU_CMD_STATUS_0
            pmu_cmd_stat->status_0.pmu_cmd_busy = BMM350_GET_BITS_POS_0(reg_data[reg_pmu_cmd_status_0], BMM350_PMU_CMD_BUSY);

            pmu_cmd_stat->status_0.odr_ovwr = BMM350_GET_BITS(reg_data[reg_pmu_cmd_status_0], BMM350_ODR_OVWR);

            pmu_cmd_stat->status_0.avr_ovwr = BMM350_GET_BITS(reg_data[reg_pmu_cmd_status_0], BMM350_AVG_OVWR);

            pmu_cmd_stat->status_0.pwr_mode_is_normal = BMM350_GET_BITS(reg_data[reg_pmu_cmd_status_0], BMM350_PWR_MODE_IS_NORMAL);

            pmu_cmd_stat->status_0.cmd_is_illegal = BMM350_GET_BITS(reg_data[reg_pmu_cmd_status_0], BMM350_CMD_IS_ILLEGAL);

            pmu_cmd_stat->status_0.pmu_cmd_value = BMM350_GET_BITS(reg_data[reg_pmu_cmd_status_0], BMM350_PMU_CMD_VALUE);

            // PMU_CMD_STATUS_1
            pmu_cmd_stat->status_1.pmu_odr_s = BMM350_GET_BITS_POS_0(reg_data[reg_pmu_cmd_status_1], BMM350_PMU_ODR_S);
            pmu_cmd_stat->status_1.pmu_avg_s = BMM350_GET_BITS(reg_data[reg_pmu_cmd_status_1], BMM350_PMU_AVG_S);
        }
    }
    else
    {
        rslt = BMM350_E_PMU_CMD_S_NULL_PTR;
    }

	return rslt;
}
```
## otp_dump_after_boot() 
* \ref otp_dump_after_boot() "otp_dump_after_boot()" this function reads the factory-programmed calibration values from the OTP (One-Time Programmable) memory of the BMM350 sensor after boot, and stores them in the device structure. This enables accurate measurement compensation during operation.
* Inside this function there is \ref read_otp_word() "read_otp_word()" function, which is inside a for loop to read all the calibration values, and store it the otp_data[32] array inside the  device structure. The var_id (variant ID) is extracted from OTP and indicates different calibration profiles or hardware variants of the BMM350 sensor. It helps differentiate between sensor versions. However, in this library, var_id is only read and stored — it is not actively used in any logic or calibration decisions, maybe for future use and development.
* Then update magnetometer offset and sensitivity data in the function \ref update_mag_off_sens() "update_mag_off_sens()" using the OTP data.

```c
bmm350_status_e otp_dump_after_boot(struct bmm350_dev_s *dev){
	bmm350_status_e rslt;

	uint16_t otp_word = 0;


	for(uint8_t i = 0; i < BMM350_OTP_DATA_LENGTH; i++){
		rslt = read_otp_word(dev, i, &otp_word);
		dev->otp_data[i] = otp_word;
	}

	dev->var_id = (dev->otp_data[30] & 0x7f00) >> 9;

    /* Update magnetometer offset and sensitivity data. */
    update_mag_off_sens(dev);
	return rslt;
}
``` 
## bmm350_magnetic_reset_and_wait()

* \ref bmm350_magnetic_reset_and_wait() "bmm350_magnetic_reset_and_wait()", This function Performs a magnetic reset of the BMM350 sensor.
* The function first does check for mraw_override() function pointer, Its purpose is to override the default magnetic reset logic, but in our application it is not needed so I commented it.

```c
bmm350_status_e bmm350_magnetic_reset_and_wait(struct bmm350_dev_s *dev){
	bmm350_status_e rslt;

    uint8_t pmu_cmd = 0;
    bmm350_pmu_cmd_status_s pmu_cmd_stat = { 0 };
    uint8_t restore_normal = BMM350_DISABLE;

    rslt = null_ptr_check(dev);

//    if ((rslt == BMM350_OK) && (dev->mraw_override) && (dev->var_id >= BMM350_MIN_VAR)){
//
//            rslt = dev->mraw_override(dev);
//	}else{
    	/* Read PMU CMD status */
		rslt = bmm350_get_pmu_cmd_status(dev, &pmu_cmd_stat); // How Normal is set????

		/* Check the powermode is normal before performing magnetic reset */
		if((rslt == BMM350_OK) && (pmu_cmd_stat.status_0.pwr_mode_is_normal == BMM350_ENABLE)){
			restore_normal = BMM350_ENABLE;

			/* Reset can only can be triggered in suspend */
			rslt = bmm350_set_powermode(dev, BMM350_SUSPEND_MODE);
		}

		if(rslt == BMM350_OK){
			/* Set BR to PMU_CMD register */
			pmu_cmd = BMM350_PMU_CMD_BR;

			rslt = dev->write(dev, BMM350_REG_PMU_CMD, &pmu_cmd, 1);
			if(rslt == BMM350_OK){
				dev->delay_us(BMM350_BR_DELAY);
			}
		}

		if(rslt == BMM350_OK){
            /* Verify if PMU_CMD_STATUS_0 register has BR set */
            rslt = bmm350_get_pmu_cmd_status(dev, &pmu_cmd_stat);

            if ((rslt == BMM350_OK) && (pmu_cmd_stat.status_0.pmu_cmd_value != BMM350_PMU_CMD_STATUS_0_BR))
            {
                rslt = BMM350_E_PMU_CMD_VALUE;
            }
		}

		if(rslt == BMM350_OK){
            /* Set FGR to PMU_CMD register */
            pmu_cmd = BMM350_PMU_CMD_FGR;

            rslt = dev->write(dev, BMM350_REG_PMU_CMD, &pmu_cmd, 1);
            if(rslt == BMM350_OK){
            	dev->delay_us(BMM350_FGR_DELAY);
            }
		}

		if(rslt == BMM350_OK){
            /* Verify if PMU_CMD_STATUS_0 register has FGR set */
            rslt = bmm350_get_pmu_cmd_status(dev, &pmu_cmd_stat);

            if ((rslt == BMM350_OK) && (pmu_cmd_stat.status_0.pmu_cmd_value != BMM350_PMU_CMD_STATUS_0_FGR))
            {
                rslt = BMM350_E_PMU_CMD_VALUE;
            }
		}

        if ((rslt == BMM350_OK) && (restore_normal == BMM350_ENABLE))
        {
            rslt = bmm350_set_powermode(dev, BMM350_NORMAL_MODE);
        }

//	}

    return rslt;
}
```
* Then the function check the current power mode, and then put it in suspend mode if it was not.
```c
/* Check the powermode is normal before performing magnetic reset */
if((rslt == BMM350_OK) && (pmu_cmd_stat.status_0.pwr_mode_is_normal == BMM350_ENABLE)){
    restore_normal = BMM350_ENABLE;

    /* Reset can only can be triggered in suspend */
    rslt = bmm350_set_powermode(dev, BMM350_SUSPEND_MODE);
}
```
* Then the function performs a bit reset by writing 0x07 to register (0x06) ref BMM350_REG_PMU_CMD "BMM350_REG_PMU_CMD" then performs delay. After that checks if that was done successfully.

```c
if(rslt == BMM350_OK){
    /* Set BR to PMU_CMD register */
    pmu_cmd = BMM350_PMU_CMD_BR;

    rslt = dev->write(dev, BMM350_REG_PMU_CMD, &pmu_cmd, 1);
    if(rslt == BMM350_OK){
        dev->delay_us(BMM350_BR_DELAY);
    }
}

if(rslt == BMM350_OK){
    /* Verify if PMU_CMD_STATUS_0 register has BR set */
    rslt = bmm350_get_pmu_cmd_status(dev, &pmu_cmd_stat);

    if ((rslt == BMM350_OK) && (pmu_cmd_stat.status_0.pmu_cmd_value != BMM350_PMU_CMD_STATUS_0_BR))
    {
        rslt = BMM350_E_PMU_CMD_VALUE;
    }
}
```

* Then it Performs Flux Guide Reset (FGR), by writing 0x05 to register (0x06) \ref BMM350_REG_PMU_CMD "BMM350_REG_PMU_CMD" then perform delay, After that checks if that was done successfully.

```c
if(rslt == BMM350_OK){
/* Set FGR to PMU_CMD register */
pmu_cmd = BMM350_PMU_CMD_FGR;

rslt = dev->write(dev, BMM350_REG_PMU_CMD, &pmu_cmd, 1);
if(rslt == BMM350_OK){
    dev->delay_us(BMM350_FGR_DELAY);
}
}

if(rslt == BMM350_OK){
/* Verify if PMU_CMD_STATUS_0 register has FGR set */
rslt = bmm350_get_pmu_cmd_status(dev, &pmu_cmd_stat);

if ((rslt == BMM350_OK) && (pmu_cmd_stat.status_0.pmu_cmd_value != BMM350_PMU_CMD_STATUS_0_FGR))
{
    rslt = BMM350_E_PMU_CMD_VALUE;
}
}
```

* Then set power mode to MORMAL_MODE if it was.
```c
if ((rslt == BMM350_OK) && (restore_normal == BMM350_ENABLE))
{
    rslt = bmm350_set_powermode(dev, BMM350_NORMAL_MODE);
}
```






---
@author MOHAMMED ABDULALEM | محمد عبدالعالم