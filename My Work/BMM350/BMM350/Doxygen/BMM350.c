
/********************************* بسم الله الرحمن الرحيم *********************************
 * 	@file BMM350.c
 *	@brief This file contains the API functions of BMM350 magnetic sensor
 *  @date April 27, 2025
 *  recreated on : May 3, 2025
 *
 *  @author MOAHMMED ABDULALEM | محمد عبدالعالم
 *	@version 1.00

 !!!!!!!!!!!!!!!!!!!!! Important Note !!!!!!!!!!!!!!!!!!!!!!!!!
 * -User type
 * 	function is made for user.
 *
 * -Internal & User type
 *  functions are not necessary for user but if he to adjust internal
 * 	settings of the sensor. *  user must understand the function first *
 *
 * -Internal type
 * functions are not for user, except for library adjustment, fixing errors and
 *  developments.
 */
 


#ifndef SRC_BMM350_IMU_BMM350_C_
#define SRC_BMM350_IMU_BMM350_C_

#include "BMM350.h"






/*************************************************************
 * ****************  Functions definitions *******************
 ************************************************************/
/** @defgroup user USER FUNCTIONS
 *  This is the users function group
 *  @{
 */
/************************ USER FUNCTIONS ***********************/


/*!
 * @note		This is a user type function.
 * Note that there is predeclred device structure "BMM350"
 * 			You must call this function before getting the data.
 * @brief		This function to initialize the BMM350 registers and enable Normal Mode.	
 *
 * @param[out] dev 	structure pointer to device information structure.
 * @param[in]  hi2c	structure pointer to the HAL I2C.
 *
 * @return bmm350_status_e -> enum that holds the status of the initialization process.
 * @code
 * 		bmm350_init(&BMM350, &hi2c2);
 * @endcode
 */
bmm350_status_e bmm350_init(struct bmm350_dev_s *dev, I2C_HandleTypeDef *hi2c){
	bmm350_status_e rslt;

	rslt = bmm350_init_struct(dev, hi2c);
	// Chip ID
	uint8_t chip_id;

    // Variable to store the command to power-off the OTP
    uint8_t otp_cmd = BMM350_OTP_CMD_PWR_OFF_OTP;

    /* Variable to store soft-reset command */
    uint8_t soft_reset;

   if(rslt == BMM350_OK){
   	rslt = check_device(dev);
   	if(rslt == BMM350_OK){
    		rslt = null_ptr_check(dev);
   	}
   }
    if(rslt == BMM350_OK){
    	dev->chip_id = 0;

    	// Enable all axis
    	dev->axis_en = 0x07;

    	dev->delay_us(BMM350_START_UP_TIME_FROM_POR);

    	if(rslt == BMM350_OK){
            /* Soft-reset */
            soft_reset = BMM350_CMD_SOFTRESET;

            /* Set the command in the command register */
    		rslt = dev->write(dev, BMM350_REG_CMD, &soft_reset, 1);
    		if(rslt == BMM350_OK){
    			dev->delay_us(BMM350_SOFT_RESET_DELAY);
    		}
    	}

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

    	if(rslt == BMM350_OK){
    		rslt = interrupt_config(dev);
    	}

    	if(rslt == BMM350_OK){
    		rslt = bmm350_odr_avg_config(dev, BMM350_ODR_400HZ, BMM350_AVG_NO_AVG);
    		if((rslt == BMM350_OK)){
    			rslt = bmm350_set_powermode(dev, BMM350_NORMAL_MODE);
    		}
    	}

    	if(rslt == BMM350_OK){
    		rslt = bmm350_get_pmu_cmd_status(dev, &dev->pmu_cmd_status);
    	}
    }
	return rslt;
}

/*!
 * @note			This is a user type function.
 * @brief 			This function is used to get yaw value
 *
 * @param[in] dev	 	structure pointer to device information structure
 * @param[in] unit 		enum to specify the type of the returned value (radian or degree)
 * @param[in] range 	enum to specify the range of degree returned **if unit is radian then put NON**
 *
 * @retval 			yaw -> The YAW angle value
 * @code
 * 		yaw = BMM350_read_yaw(&BMM350, DEGREE, FROM_0_TO_360);
 * @endcode
 */
float bmm350_read_yaw(struct bmm350_dev_s *dev, bmm350_ang_unt_rng_e unit, bmm350_ang_unt_rng_e range){
	bmm350_status_e rslt;
	static float yaw[2] = {0.0f}; // [0] radian, [1] degree
	static float NUV[2] = {0.0f}; // Normalized unit vector for, [0] X, [1] Y

	rslt = read_intr(dev, &dev->data);
	if(rslt == BMM350_INT_DATA_READY){
		NUV[0] = (dev->data.x / sqrt((pow(dev->data.y, 2) + pow(dev->data.x, 2))));
		NUV[1] = (dev->data.y / sqrt((pow(dev->data.y, 2) + pow(dev->data.x, 2))));

		yaw[0] = atan2(-1 * NUV[1], NUV[0]);
		dev->yaw[0] = (yaw[0] * (180.0 / PI));		// Initialize the raw yaw in the structure for kalman filtering
		if(dev->yaw[0] < 0) dev->yaw[0] += 360.0;	// Makes sure it is in the of 0 to 360 for better filtering

		dev->yaw[1] = adaptive_filter(&BMM350_yaw_f, dev->yaw[0]);

		yaw[1] = dev->yaw[1];

		if(range == FROM_NEG_180_TO_POS_180){
			yaw[1] -= (yaw[1] > 180)? 360.0f : 0;
		}
	}
	return (unit == RADIAN)? yaw[0] : yaw[1];
}

/*!
 * @note					This is a user type function.
 * @brief 					this function checks the drdy_data_reg (0x30) *data ready interrupt*
 * 						then returns the status of the register, if interrupt is triggered it will
 * 						the magnetic and temperature data to bmm350_mag_temp_data_s structure
 *
 * @param[in]  dev 			structure pointer to device information structure
 * @param[out] output_data	structure pointer to the magnetic field and temperature data
 *
 * @retval 					rslt: the status of the function from bmm350_status_e enum
 * @retval 					BMM350_INT_DATA_READY		-> data is ready to be read
 * @retval 					BMM350_INT_DATA_NOT_READY	-> data is not ready to be read
 *
 * @code
 *  	rslt = read_intr(&BMM350, &BMM350.data);
 * @endcode
 */
bmm350_status_e read_intr(struct bmm350_dev_s *dev, bmm350_mag_temp_data_s *output_data){
	bmm350_status_e rslt;

	uint8_t int_status;

	rslt = dev->read(dev, BMM350_REG_INT_STATUS, &int_status, 1);
	if((rslt == BMM350_OK) && (int_status & 0x04)){
		rslt = BMM350_INT_DATA_READY;
		bmm350_get_compensated_mag_xyz_temp_data(dev, output_data);
	}else{
		rslt = BMM350_INT_DATA_NOT_READY;
	}
	return rslt;
}


/*!
 * @note			This is a user type function
 * 
 * @brief 			Performs a magnetic reset of the BMM350 sensor. This is necessary when:
 *				The sensor may have been exposed to excessively strong magnetic fields (e.g. >400 mT).
 *  			Long periods of inactivity have caused capacitor discharge on the CRST pin.
 *  			You're resuming from suspend mode and want to guarantee accuracy.
 * 
 *	@param[out] dev structure pointer to device information structure.
 *
 * 	@retval 		BMM350_OK				-> 	as it says
 *	@retval 		BMM350_E_PMU_CMD_VALUE	-> 	PMU_CMD register 0x6 does not hold the configured values
 *
 * @code
 *	 	bmm350_magnetic_reset_and_wait(&BMM350);
 * @endcode
 */

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

/** @} */ // end of user functions

/** @defgroup user_internal USER & INTERNAL FUNCTIONS
 *  This is the USER & INTERNAL FUNCTIONS group
 *  @{
 */
/************************ USER & INTERNAL FUNCTIONS ***********************/


/*!
 * @note	This is a internal & user type function
 
 * @brief	This function filters the data of angle got from magnetic readings
 *
 * @param[in] 	filter: 	structure pointer to filter
 * @param[out] 	raw_data:	The angle data
 * @return  filtered angle
 *
 * @code 	
 * 		adaptive_filter(&BMM350_yaw_f, dev->yaw[0]);
 * @endcode
 */

float adaptive_filter(bmm350_filter *filter, float raw_data){
    float unwrapped = unwrap_angle(filter->data, raw_data);
    float delta = fabsf(unwrapped - filter->data);


    filter->alpha = ((delta > CHANGE_THRESHOLD)? ALPHA_MAX : ALPHA_MIN	 /*+ (delta / CHANGE_THRESHOLD) * (ALPHA_MAX - ALPHA_MIN)*/);

    filter->data = filter->alpha * unwrapped + (1.0f - filter->alpha) * filter->data;
    return wrap_360(filter->data);
}


/*!
 * @note			This is a internal & user type function
 * @brief			This function checks I2C communication status
 * @param[in] dev 	structure pointer to device information structure
 * @return  		Device I2C connection status
 * @code 
 * 		check_device(&BMM350);
 * @endcode
 *
 */
bmm350_status_e check_device(struct bmm350_dev_s *dev){
	bmm350_status_e rslt;

	if(HAL_I2C_IsDeviceReady(dev->hi2c, (dev->address << 1), 3, HAL_MAX_DELAY) == HAL_OK){
		rslt = BMM350_OK;
	}else{
		rslt = Bmm350_DEV_NOT_READY;
	}
	return rslt;
}


/*!
 * @note					This is a internal & user type function
 * @brief					This function is used to read a register from the BMM350 sensor.
 *
 *@param[in] dev: 			structure pointer to device information structure.
 *@param[in] MemAddress:	the memory address *you can find all registers addresses in BMM350.h.
 *@param[in] pData:			the buffer that will hold the register data.
 *@param[in] len:			the length of the data buffer.
 *
 * 	@retval 				BMM350_OK			-> 	as it says
 *	@retval 				BMM350_E_R_COM_FAIL	->  the read process failed
 *	@retval 				BMM350_E_NULL_PTR	->	dev is NULL, maybe uninitialized.
 *
 * @code 
 *  	BMM350_I2C_read(&BMM350, BMM350_REG_CHIP_ID, &chip_id, 1);
 * @endcode
 */

bmm350_status_e bmm350_I2C_read(bmm350_dev_s *dev,
								uint16_t MemAddress,
								uint8_t *pData,
								uint16_t len){
	bmm350_status_e rslt;

	uint16_t temp_len = len + BMM350_DUMMY_BYTES;
	uint8_t temp_buf[BMM350_READ_BUFFER_LENGTH];
	uint16_t index = 0;

	rslt = null_ptr_check(dev);
	if((rslt == BMM350_OK) && (pData != NULL)){
		if(HAL_I2C_Mem_Read(dev->hi2c,
							(dev->address << 1),
							MemAddress,
							I2C_MEMADD_SIZE_8BIT,
							temp_buf,
							temp_len, HAL_MAX_DELAY) != HAL_OK){
			rslt = BMM350_E_R_COM_FAIL;
		}else{
			rslt = BMM350_OK;
		}

		if(rslt == BMM350_OK){
			while(index < len){
				pData[index] = temp_buf[index + BMM350_DUMMY_BYTES];
				index++;
			}
		}
	}else{
		rslt = BMM350_E_NULL_PTR;
	}

//	if((HAL_I2C_GetState(dev->hi2c) == HAL_I2C_STATE_READY) && (pData != NULL)){
//		if(HAL_I2C_Mem_Read_IT(dev->hi2c,
//							(dev->address << 1),
//							MemAddress,
//							I2C_MEMADD_SIZE_8BIT,
//							pData,
//							Size) != HAL_OK){
//			rslt = BMM350_E_R_COM_FAIL;
//		}else{
//			rslt = BMM350_OK;
//		}
//	}else{
//		rslt = BMM350_E_NULL_PTR;
//	}

	return rslt;
}

/*!
 * @note					This is a internal & user type function
 * @brief					This function is used to write data to a register in the BMM350 sensor.
 *
 *	@param[in] dev: 		structure pointer to device information structure.
 *	@param[in] MemAddress:	the memory address *you can find all registers addresses in BMM350.h*.
 *	@param[in] pData:		the buffer that will contain the register data.
 *	@param[in] len:			the length of the data buffer.
 *
 * 	@retval 				BMM350_OK			-> 	as it says
 *	@retval 				BMM350_E_W_COM_FAIL	->  the write process failed
 *	@retval 				BMM350_E_NULL_PTR	->	dev is NULL, maybe uninitialized.
 *
 * @code
 *  	BMM350_I2C_write(&BMM350, BMM350_REG_MAG_X_XLSB, &lsb, 1);
 * @endcode
 */
bmm350_status_e bmm350_I2C_write(bmm350_dev_s *dev,
								uint16_t MemAddress,
								uint8_t *pData,
								uint16_t Size){
	bmm350_status_e rslt;
	rslt = null_ptr_check(dev);

	if((rslt == BMM350_OK) && (pData != NULL) && (Size != 0)){

		if((HAL_I2C_GetState(dev->hi2c) == HAL_I2C_STATE_READY)  && (pData != NULL)){
			if(HAL_I2C_Mem_Write(	dev->hi2c,
									(dev->address << 1),
									MemAddress,
									I2C_MEMADD_SIZE_8BIT,
									pData,
									Size, HAL_MAX_DELAY) != HAL_OK){
				rslt = BMM350_E_R_COM_FAIL;
			}else{
				rslt = BMM350_OK;
			}
		}else{
			rslt = BMM350_E_NULL_PTR;
		}
	}


	return rslt;
}

/*!
 * @note						This is an internal & user type function
 * @brief						This internal function is used to perform compensation calculations
 * 							for raw magnetometer and temperature data, and assign it to structure.
 *
 * @param[in] dev 				structure pointer to device information structure.
 * @param[out] mag_temp_data	structure pointer to the magnetic field and temperature 
 * data x, y, z, temperature.
 *  
 *	@retval 					BMM350_OK			-> as it says
 *	@retval 					BMM350_E_NULL_PTR	-> Second argument is NULL
 *
 * @code 	
 * 		bmm350_get_compensated_mag_xyz_temp_data(&BMM350, &raw_data);
 * @endcode
 */

bmm350_status_e bmm350_get_compensated_mag_xyz_temp_data(struct bmm350_dev_s *dev, bmm350_mag_temp_data_s *mag_temp_data){
	bmm350_status_e rslt;

    uint8_t indx;
    float out_data[4] = { 0.0f };
    float dut_offset_coef[3], dut_sensit_coef[3], dut_tco[3], dut_tcs[3];
    float cr_ax_comp_x, cr_ax_comp_y, cr_ax_comp_z;

    if(mag_temp_data != NULL){

    	rslt = read_out_raw_data(dev, out_data);
    	if(rslt == BMM350_OK){
            /* Apply compensation to temperature reading */
            out_data[3] = (1 + dev->mag_comp.dut_sensit_coef.t_sens) * out_data[3] + dev->mag_comp.dut_offset_coef.t_offs;

            /* Store magnetic compensation structure to an array */
            dut_offset_coef[0] = dev->mag_comp.dut_offset_coef.offset_x;
            dut_offset_coef[1] = dev->mag_comp.dut_offset_coef.offset_y;
            dut_offset_coef[2] = dev->mag_comp.dut_offset_coef.offset_z;

            dut_sensit_coef[0] = dev->mag_comp.dut_sensit_coef.sens_x;
            dut_sensit_coef[1] = dev->mag_comp.dut_sensit_coef.sens_y;
            dut_sensit_coef[2] = dev->mag_comp.dut_sensit_coef.sens_z;

            dut_tco[0] = dev->mag_comp.dut_tco.tco_x;
            dut_tco[1] = dev->mag_comp.dut_tco.tco_y;
            dut_tco[2] = dev->mag_comp.dut_tco.tco_z;

            dut_tcs[0] = dev->mag_comp.dut_tcs.tcs_x;
            dut_tcs[1] = dev->mag_comp.dut_tcs.tcs_y;
            dut_tcs[2] = dev->mag_comp.dut_tcs.tcs_z;

            /* Compensate raw magnetic data */
            for (indx = 0; indx < 3; indx++)
            {
                out_data[indx] *= 1 + dut_sensit_coef[indx];
                out_data[indx] += dut_offset_coef[indx];
                out_data[indx] += dut_tco[indx] * (out_data[3] - dev->mag_comp.dut_t0);
                out_data[indx] /= 1 + dut_tcs[indx] * (out_data[3] - dev->mag_comp.dut_t0);
            }

            cr_ax_comp_x = (out_data[0] - dev->mag_comp.cross_axis.cross_x_y * out_data[1]) /
                           (1 - dev->mag_comp.cross_axis.cross_y_x * dev->mag_comp.cross_axis.cross_x_y);
            cr_ax_comp_y = (out_data[1] - dev->mag_comp.cross_axis.cross_y_x * out_data[0]) /
                           (1 - dev->mag_comp.cross_axis.cross_y_x * dev->mag_comp.cross_axis.cross_x_y);
            cr_ax_comp_z =
                (out_data[2] +
                 (out_data[0] *
                  (dev->mag_comp.cross_axis.cross_y_x * dev->mag_comp.cross_axis.cross_z_y -
                   dev->mag_comp.cross_axis.cross_z_x) - out_data[1] *
                  (dev->mag_comp.cross_axis.cross_z_y - dev->mag_comp.cross_axis.cross_x_y *
                   dev->mag_comp.cross_axis.cross_z_x)) /
                 (1 - dev->mag_comp.cross_axis.cross_y_x * dev->mag_comp.cross_axis.cross_x_y));

            out_data[0] = cr_ax_comp_x;
            out_data[1] = cr_ax_comp_y;
            out_data[2] = cr_ax_comp_z;
    	}

        if (rslt == BMM350_OK)
        {
            if ((dev->axis_en & BMM350_EN_X_MSK) == BMM350_DISABLE)
            {
                mag_temp_data->x = BMM350_DISABLE;
            }
            else
            {
                mag_temp_data->x = out_data[0];
            }

            if ((dev->axis_en & BMM350_EN_Y_MSK) == BMM350_DISABLE)
            {
                mag_temp_data->y = BMM350_DISABLE;
            }
            else
            {
                mag_temp_data->y = out_data[1];
            }

            if ((dev->axis_en & BMM350_EN_Z_MSK) == BMM350_DISABLE)
            {
                mag_temp_data->z = BMM350_DISABLE;
            }
            else
            {
                mag_temp_data->z = out_data[2];
            }

            mag_temp_data->temperature = out_data[3];
        }
    }else{
    	rslt = BMM350_E_NULL_PTR;
    }
	return rslt;
}

/*!
 * @note					This is an internal & user type function
 * @brief					This function is used to set the power mode of the sensor.
 *
 * @param[in] dev			structure pointer to device information structure.
 * @param[in] powermode:	enum from bmm350_power_modes_e for indicating the power mode
 *
 * @retval					BMM350_OK				-> 	as it says
 * @retval					BMM350_E_INVALID_CONFIG	-> 	invalid powermode
 *
 * @code 	
 * 		bmm350_set_powermode(&BMM350, NORMAL_MODE);
 * @endcode
 */

bmm350_status_e bmm350_set_powermode(struct bmm350_dev_s *dev, bmm350_power_modes_e powermode){
	bmm350_status_e rslt;

    uint8_t last_pwr_mode;
    uint8_t reg_data;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);
    if(rslt == BMM350_OK){
    	rslt = dev->read(dev, BMM350_REG_PMU_CMD, &last_pwr_mode, 1);
    	if(rslt == BMM350_OK){
    		if(last_pwr_mode > BMM350_PMU_CMD_BR_FAST){
    			rslt = BMM350_E_INVALID_CONFIG;
    		}

    		if((rslt == BMM350_OK) && ((last_pwr_mode == BMM350_PMU_CMD_NM) || (last_pwr_mode == BMM350_PMU_CMD_UPD_OAE))){
    			reg_data = BMM350_PMU_CMD_SUS;

    			/* Set PMU command configuration  to SUSPEND mode*/
    			rslt = dev->write(dev, BMM350_REG_PMU_CMD, &reg_data, 1);
    			if(rslt == BMM350_OK){
    				dev->delay_us(BMM350_GOTO_SUSPEND_DELAY);
    			}
    		}
    		if(rslt == BMM350_OK){
    			rslt = set_powermode(dev, powermode);
    		}
    	}
    }

	return rslt;
}

/*!
 * @note					This is an internal & user type function
 * @brief					This function gets the PMU command status 0 register (0x07) and
 * 						PMU command status 1 register (0x08) values
 *
 * @param[in] dev: 			structure pointer to device information structure.
 * @param[out] pmu_cmd_stat	structure pointer that holds the registers's structure
 *
 * 	@retval					BMM350_OK					-> 	as it says
 *	@retval 				BMM350_E_PMU_CMD_S_NULL_PTR	-> 	pmu_cmd_stat is NULL, maybe because not initialized
 * 
 * @code
 * 		bmm350_get_pmu_cmd_status(&BMM350, &pmu_cmd_stat);
 * @endcode
 */

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

/** @} */ // end of user & internal functions

/** @defgroup internal INTERNAL FUNCTIONS
 *  This is the INTERNAL FUNCTIONS group
 *  @{
 */
/************************ INTERNAL FUNCTIONS ***********************/

/*!
 * @note			This is an internal type function
 * @brief			This function initialize the BMM350 device structure and the adaptive filter
 *
 * 	@param[in] dev 	structure pointer to device information structure
 * 	@param[in] hi2c	structure pointer to the HAL I2C
 *
 *	@retval  		The status of the function
 *
 * @code
 * 		 BMM350_init_struct(&BMM350, &hi2c1);
 * @endcode
 */
bmm350_status_e bmm350_init_struct(struct bmm350_dev_s *dev, I2C_HandleTypeDef *hi2c){

	dev->hi2c 			= hi2c;
	dev->address 		= BMM350_I2C_ADDRESS;
	dev->read 			= bmm350_I2C_read;
	dev->write 			= bmm350_I2C_write;
	dev->delay_us 		= DWT_delay_us;

	memset(&dev->pmu_cmd_status, 0, sizeof(dev->pmu_cmd_status));

	return null_ptr_check(dev);
}


/*!
 * @note				This is an internal type function
 * @brief				This function wrapes the angle input to make sure it is between 0 and 360 degrees
 *
 * 	@param[in] angle 	The angle that need to be wraped
 *
 *	@retval  			The wraped angle
 *
 * @code
 * 		 wrap_360(angle);
 * @endcode
 */
float wrap_360(float angle){
    while (angle >= 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;
	return angle;
}


/*!
 * @note				This is an internal type function
 * @brief				This function smooths the operation of interpolation between angles 
 * 					specially for jumps from 0 to 360 or vice versa
 *
 * 	@param[in] prev 	The current angle
 *	@param[in] curr 	The previous angle 

 *	@retval  			The angle added with delta (Difference between previous and current angle)
 *
 * @code
 * 		 unwrap_angle(prev_angle, angle);
 * @endcode
 */
float unwrap_angle(float prev, float curr){
	float delta = curr - prev;

	if(delta > 180.0f){
		delta -= 360.0f;
	}else if(delta < -180){
		delta += 360.0f;
	}
	return (prev + delta);
}


/*!
 * @note				This is an internal type function
 * @brief				This function initialize the device structure and configures the INT_CTRL register (0x2E)
 *
 *	@param[out] dev 	structure pointer to device information structure
 *
 *	@retval 			BMM350_OK			-> as it says
 *	@retval 			BMM350_E_INTR_INV	-> configuration was not successful
 *
 * @code 
 * 		interrupt_config(&BMM350);
 * @endcode
 */

bmm350_status_e interrupt_config(struct bmm350_dev_s *dev){
	bmm350_status_e rslt;

	// These can be configured if needed only otherwise leave it
	dev->intr_setting.latch 	= BMM350_PULSED;
	dev->intr_setting.polarity 	= BMM350_ACTIVE_HIGH;
	dev->intr_setting.drive 	= BMM350_INTR_PUSH_PULL;
	dev->intr_setting.map		= BMM350_UNMAP_FROM_PIN;

	uint8_t reg_data = 0;
//	uint8_t reg_data_test[4];

	rslt = dev->read(dev, BMM350_REG_INT_CTRL, &reg_data, 1);
	if(rslt == BMM350_OK){
        reg_data = BMM350_SET_BITS_POS_0(reg_data, BMM350_INT_MODE, dev->intr_setting.latch);
        reg_data = BMM350_SET_BITS(reg_data, BMM350_INT_POL, dev->intr_setting.polarity);
        reg_data = BMM350_SET_BITS(reg_data, BMM350_INT_OD, dev->intr_setting.drive);
        reg_data = BMM350_SET_BITS(reg_data, BMM350_INT_OUTPUT_EN, dev->intr_setting.map);
        reg_data = BMM350_SET_BITS(reg_data, BMM350_DRDY_DATA_REG_EN, BMM350_ENABLE);

        rslt = dev->write(dev, BMM350_REG_INT_CTRL, &reg_data, 1);
	}

	if(rslt == BMM350_OK){
		rslt = dev->read(dev, BMM350_REG_INT_CTRL, &reg_data, 1);
		if(rslt == BMM350_OK){
			if(!(reg_data & 0x86)){
				rslt = BMM350_E_INTR_INV;
			}
		}
	}

	return rslt;
}

/*!
 * @note		This is an internal type function
 * @brief		This function is used to read (ADC) mag and temperature data.
 *
 * @param[in] 	dev 		structure pointer to device information structure
 * @param[out] 	raw_data	structure pointer to raw ADC data
 *
 *	@retval 	BMM350_OK			-> as it says
 *	@retval 	BMM350_E_NULL_PTR	-> Second argument is NULL
 *
 * @code
 *  bmm350_read_uncomp_mag_temp_data(&BMM350, &raw_data);
 * @endcode
 */

bmm350_status_e bmm350_read_uncomp_mag_temp_data(struct bmm350_dev_s *dev, bmm350_raw_mag_data_s *raw_data){
	bmm350_status_e rslt;

	uint8_t mag_data[12] = { 0 };
	uint32_t raw_mag_x, raw_mag_y, raw_mag_z, raw_temp;



	if(raw_data != NULL){

		rslt = dev->read(dev, BMM350_REG_MAG_X_XLSB, mag_data, BMM350_MAG_TEMP_DATA_LEN);
		if(rslt == BMM350_OK){
            raw_mag_x = mag_data[0] + ((uint32_t)mag_data[1] << 8) + ((uint32_t)mag_data[2] << 16);
            raw_mag_y = mag_data[3] + ((uint32_t)mag_data[4] << 8) + ((uint32_t)mag_data[5] << 16);
            raw_mag_z = mag_data[6] + ((uint32_t)mag_data[7] << 8) + ((uint32_t)mag_data[8] << 16);
            raw_temp = mag_data[9] + ((uint32_t)mag_data[10] << 8) + ((uint32_t)mag_data[11] << 16);

            if ((dev->axis_en & BMM350_EN_X_MSK) == BMM350_DISABLE)
            {
                raw_data->raw_xdata = BMM350_DISABLE;
            }
            else
            {
                raw_data->raw_xdata = fix_sign(raw_mag_x, BMM350_SIGNED_24_BIT);
            }

            if ((dev->axis_en & BMM350_EN_Y_MSK) == BMM350_DISABLE)
            {
                raw_data->raw_ydata = BMM350_DISABLE;
            }
            else
            {
                raw_data->raw_ydata = fix_sign(raw_mag_y, BMM350_SIGNED_24_BIT);
            }

            if ((dev->axis_en & BMM350_EN_Z_MSK) == BMM350_DISABLE)
            {
                raw_data->raw_zdata = BMM350_DISABLE;
            }
            else
            {
                raw_data->raw_zdata = fix_sign(raw_mag_z, BMM350_SIGNED_24_BIT);
            }

            raw_data->raw_data_t = fix_sign(raw_temp, BMM350_SIGNED_24_BIT);

		}
	}else{
		rslt = BMM350_E_NULL_PTR;
	}

	return rslt;
}


/*!
 * @note					This is an internal type function
 * @brief					This internal function is used to read uncompensated raw magnetic x,y and z axis
 * 						along with temperature.
 *
 *	@param[in] 	dev			structure pointer to device information structure.
 *	@param[out] out_data	float pointer to hold uncompensated data of x, y, z, temperature.
 *
 *	@retval 				BMM350_OK			-> as it says
 *	@retval 				BMM350_E_NULL_PTR	-> Second argument is NULL
 *
 * @code
 * 	read_out_raw_data(&BMM350, &out_data);
 * @endcode
 */

bmm350_status_e read_out_raw_data(struct bmm350_dev_s *dev, float *out_data){
	bmm350_status_e rslt;

	float temp = 0.0;
	bmm350_raw_mag_data_s raw_data = { 0 };

    /* Float variable to convert mag lsb to uT and temp lsb to degC */
    float lsb_to_ut_degc[4];

    if(out_data != NULL){
    	rslt = bmm350_read_uncomp_mag_temp_data(dev, &raw_data);

    	if(rslt == BMM350_OK){
    		/* Convert mag lsb to uT and temp lsb to degC */
    		update_default_coefiecents(lsb_to_ut_degc);
            out_data[0] = (float)raw_data.raw_xdata * lsb_to_ut_degc[0];
            out_data[1] = (float)raw_data.raw_ydata * lsb_to_ut_degc[1];
            out_data[2] = (float)raw_data.raw_zdata * lsb_to_ut_degc[2];
            out_data[3] = (float)raw_data.raw_data_t * lsb_to_ut_degc[3];

            if (out_data[3] > 0.0)
            {
                temp = (float)(out_data[3] - (1 * 25.49));
            }
            else if (out_data[3] < 0.0)
            {
                temp = (float)(out_data[3] - (-1 * 25.49));
            }
            else
            {
                temp = (float)(out_data[3]);
            }

            out_data[3] = temp;
    	}
    }else{
    	rslt = BMM350_E_NULL_PTR;
    }
	return rslt;
}

/*!
 * @note		This is an internal type function
 * @brief		This internal function is used to update uncompensated lsb data to uT and degC.
 *
 *	@param[out] lsb_to_ut_degC float array that will hold the the coefficients 
 *
 * @return of course nothing
 *
 * @code
 *  	update_default_coefiecents(lsb_to_ut_degC);
 * @endcode
 */

void update_default_coefiecents(float *lsb_to_ut_degC){
    float bxy_sens, bz_sens, temp_sens, ina_xy_gain_trgt, ina_z_gain_trgt, adc_gain, lut_gain;
    float power;

    bxy_sens = 14.55f;
    bz_sens = 9.0f;
    temp_sens = 0.00204f;

    ina_xy_gain_trgt = 19.46f;

    ina_z_gain_trgt = 31.0;

    adc_gain = 1 / 1.5f;
    lut_gain = 0.714607238769531f;

    power = (float)(1000000.0 / 1048576.0);

    lsb_to_ut_degC[0] = (power / (bxy_sens * ina_xy_gain_trgt * adc_gain * lut_gain));
    lsb_to_ut_degC[1] = (power / (bxy_sens * ina_xy_gain_trgt * adc_gain * lut_gain));
    lsb_to_ut_degC[2] = (power / (bz_sens * ina_z_gain_trgt * adc_gain * lut_gain));
    lsb_to_ut_degC[3] = 1 / (temp_sens * adc_gain * lut_gain * 1048576);
}


/*!
 * @note		This is an internal type function
 * @brief		This internal function configures the odr (Output Data Rate) and
 * 			avg (Measurements Averages) in register 0x04.
 *
 *	@param[out] dev structure pointer to device information structure.
 *	@param[in] 	odr	values can be got from macros AVG and ODR, this indicates
 *						the values that will be written for odr and avg.
 *  @param[in] 	avg values can be got from macros AVG and ODR, this indicates
 *						the values that will be written for odr and avg.
 *
 *
 * 	@retval BMM350_OK				-> as it says
 *	@retval BMM350_E_INV_ODR_AVG	-> odr and or avg value is wrong.
 *
 * @code
 *  	bmm350_odr_avg_config(&BMM350, BMM350_ODR_400HZ, BMM350_AVG_NO_AVG);
 * @endcode
 */

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

/*!
 * @note		This is an internal type function
 * @brief		This function is used to switch from suspend mode to normal mode or forced mode.
 *
 *	@param[out] dev: 		structure pointer to device information structure.
 *	@param[in] 	powermode:	enum from bmm350_power_modes_e for indicating the power mode
 *
 * 	@retval 	BMM350_OK				-> 	as it says
 *	@retval 	BMM350_E_INV_PWR_MODE	-> 	invalid powermode
 *
 * @code
 *  	set_powermode(&BMM350, NORMAL_MODE);
 * @endcode
 */


bmm350_status_e set_powermode(struct bmm350_dev_s *dev, bmm350_power_modes_e powermode){
	bmm350_status_e rslt;

    uint8_t reg_data = powermode;
    uint8_t get_avg;

    /* Array to store suspend to forced mode delay */
    uint32_t sus_to_forced_mode[4] =
    { BMM350_SUS_TO_FORCEDMODE_NO_AVG_DELAY, BMM350_SUS_TO_FORCEDMODE_AVG_2_DELAY, BMM350_SUS_TO_FORCEDMODE_AVG_4_DELAY,
      BMM350_SUS_TO_FORCEDMODE_AVG_8_DELAY };

    /* Array to store suspend to forced mode fast delay */
    uint32_t sus_to_forced_mode_fast[4] =
    { BMM350_SUS_TO_FORCEDMODE_FAST_NO_AVG_DELAY, BMM350_SUS_TO_FORCEDMODE_FAST_AVG_2_DELAY,
      BMM350_SUS_TO_FORCEDMODE_FAST_AVG_4_DELAY, BMM350_SUS_TO_FORCEDMODE_FAST_AVG_8_DELAY };

    uint8_t avg = 0;
    uint32_t delay_us = 0;

    rslt = null_ptr_check(dev);
    if(rslt == BMM350_OK){
    	/* Set PMU command configuration to desired power mode */
    	rslt = dev->write(dev, BMM350_REG_PMU_CMD, &reg_data, 1);
    	if(rslt == BMM350_OK){
    		/* Get average configuration */
    		rslt = dev->read(dev, BMM350_REG_PMU_CMD_AGGR_SET, &get_avg, 1);
    		if(rslt == BMM350_OK){
    			/* Mask the average value */
    			avg = ((get_avg & BMM350_AVG_MSK) >> BMM350_AVG_POS);
    		}
    	}
    }

    if(rslt == BMM350_OK){

    	switch(powermode){
    	case BMM350_SUSPEND_MODE:
    		dev->bmm350_err = BMM350_E_SUSPEND_MODE;
    		break;
    	case BMM350_NORMAL_MODE:
    		delay_us = BMM350_SUSPEND_TO_NORMAL_DELAY;
    		break;
    	case BMM350_FORCED_MODE:
    		delay_us = sus_to_forced_mode[avg];
    		break;
    	case BMM350_FORCED_MODE_FAST:
    		delay_us = sus_to_forced_mode_fast[avg];
    		break;
    	default:
    		dev->bmm350_err = BMM350_E_INV_PWR_MODE;
    		rslt = BMM350_E_INV_PWR_MODE;
    		return rslt;
    		break;
    	}
    	/* Perform delay based on power mode */
    	dev->delay_us(delay_us);
    }

	return rslt;
}

/*!
 * @note		This is an internal type function
 * @brief:		This internal function is used to read OTP data after boot in user mode.
 *
 *	@param[out] dev: structure pointer to device information structure.
 *
 * 	@retval 	BMM350_OK	-> 	as it says
 *	@retval 	others.....	->  usually the error will explain itself
 *
 * @code
 *  	otp_dump_after_boot(&BMM350);
 * @endcode
 */

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

/*!
 * @note			This is an internal type function
 * @brief			This internal function is used to update magnetometer offset and sensitivity data.
 *
 * @param[out] dev 	structure pointer to device information structure.
 *
 * @retval 			nothing 
 
 * @code
 * 		update_mag_off_sens(&BMM350);
 * @endcode
 */

void update_mag_off_sens(struct bmm350_dev_s *dev){
	uint16_t off_x_lsb_msb, off_y_lsb_msb, off_z_lsb_msb, t_off;
	uint8_t sens_x, sens_y, sens_z, t_sens;
	uint8_t tco_x, tco_y, tco_z;
	uint8_t tcs_x, tcs_y, tcs_z;
	uint8_t cross_x_y, cross_y_x, cross_z_x, cross_z_y;

	off_x_lsb_msb = dev->otp_data[BMM350_MAG_OFFSET_X] & 0x0FFF;
	off_y_lsb_msb = ((dev->otp_data[BMM350_MAG_OFFSET_X] & 0xF000) >> 4) +
					(dev->otp_data[BMM350_MAG_OFFSET_Y] & BMM350_LSB_MASK);
	off_z_lsb_msb = (dev->otp_data[BMM350_MAG_OFFSET_Y] & 0x0F00) +
					(dev->otp_data[BMM350_MAG_OFFSET_Z] & BMM350_LSB_MASK);
	t_off = dev->otp_data[BMM350_TEMP_OFF_SENS] & BMM350_LSB_MASK;

	dev->mag_comp.dut_offset_coef.offset_x = fix_sign(off_x_lsb_msb, BMM350_SIGNED_12_BIT);
	dev->mag_comp.dut_offset_coef.offset_y = fix_sign(off_y_lsb_msb, BMM350_SIGNED_12_BIT);
	dev->mag_comp.dut_offset_coef.offset_z = fix_sign(off_z_lsb_msb, BMM350_SIGNED_12_BIT);
	dev->mag_comp.dut_offset_coef.t_offs = fix_sign(t_off, BMM350_SIGNED_8_BIT) / 5.0f;

	sens_x = (dev->otp_data[BMM350_MAG_SENS_X] & BMM350_MSB_MASK) >> 8;
	sens_y = (dev->otp_data[BMM350_MAG_SENS_Y] & BMM350_LSB_MASK);
	sens_z = (dev->otp_data[BMM350_MAG_SENS_Z] & BMM350_MSB_MASK) >> 8;
	t_sens = (dev->otp_data[BMM350_TEMP_OFF_SENS] & BMM350_MSB_MASK) >> 8;

	dev->mag_comp.dut_sensit_coef.sens_x = fix_sign(sens_x, BMM350_SIGNED_8_BIT) / 256.0f;
	dev->mag_comp.dut_sensit_coef.sens_y = (fix_sign(sens_y, BMM350_SIGNED_8_BIT) / 256.0f) + BMM350_SENS_CORR_Y;
	dev->mag_comp.dut_sensit_coef.sens_z = fix_sign(sens_z, BMM350_SIGNED_8_BIT) / 256.0f;
	dev->mag_comp.dut_sensit_coef.t_sens = fix_sign(t_sens, BMM350_SIGNED_8_BIT) / 512.0f;

	tco_x = (dev->otp_data[BMM350_MAG_TCO_X] & BMM350_LSB_MASK);
	tco_y = (dev->otp_data[BMM350_MAG_TCO_Y] & BMM350_LSB_MASK);
	tco_z = (dev->otp_data[BMM350_MAG_TCO_Z] & BMM350_LSB_MASK);

	dev->mag_comp.dut_tco.tco_x = fix_sign(tco_x, BMM350_SIGNED_8_BIT) / 32.0f;
	dev->mag_comp.dut_tco.tco_y = fix_sign(tco_y, BMM350_SIGNED_8_BIT) / 32.0f;
	dev->mag_comp.dut_tco.tco_z = fix_sign(tco_z, BMM350_SIGNED_8_BIT) / 32.0f;

	tcs_x = (dev->otp_data[BMM350_MAG_TCS_X] & BMM350_MSB_MASK) >> 8;
	tcs_y = (dev->otp_data[BMM350_MAG_TCS_Y] & BMM350_MSB_MASK) >> 8;
	tcs_z = (dev->otp_data[BMM350_MAG_TCS_Z] & BMM350_MSB_MASK) >> 8;

	dev->mag_comp.dut_tcs.tcs_x = fix_sign(tcs_x, BMM350_SIGNED_8_BIT) / 16384.0f;
	dev->mag_comp.dut_tcs.tcs_y = fix_sign(tcs_y, BMM350_SIGNED_8_BIT) / 16384.0f;
	dev->mag_comp.dut_tcs.tcs_z = (fix_sign(tcs_z, BMM350_SIGNED_8_BIT) / 16384.0f) - BMM350_TCS_CORR_Z;

	dev->mag_comp.dut_t0 = (fix_sign(dev->otp_data[BMM350_MAG_DUT_T_0], BMM350_SIGNED_16_BIT) / 512.0f) + 23.0f;

	cross_x_y = (dev->otp_data[BMM350_CROSS_X_Y] & BMM350_LSB_MASK);
	cross_y_x = (dev->otp_data[BMM350_CROSS_Y_X] & BMM350_MSB_MASK) >> 8;
	cross_z_x = (dev->otp_data[BMM350_CROSS_Z_X] & BMM350_LSB_MASK);
	cross_z_y = (dev->otp_data[BMM350_CROSS_Z_Y] & BMM350_MSB_MASK) >> 8;

	dev->mag_comp.cross_axis.cross_x_y = fix_sign(cross_x_y, BMM350_SIGNED_8_BIT) / 800.0f;
	dev->mag_comp.cross_axis.cross_y_x = fix_sign(cross_y_x, BMM350_SIGNED_8_BIT) / 800.0f;
	dev->mag_comp.cross_axis.cross_z_x = fix_sign(cross_z_x, BMM350_SIGNED_8_BIT) / 800.0f;
	dev->mag_comp.cross_axis.cross_z_y = fix_sign(cross_z_y, BMM350_SIGNED_8_BIT) / 800.0f;
}

/*!
 * @note					This is an internal type function
 * @brief					This internal function is used to read OTP word.
 *
 * @param[out] 	dev 		structure pointer to device information structure.
 * @param[in] 	add			address of the word to be masked in the function
 * @param[out] 	lsb_msb		the otp word
 *
 * @retval 					BMM350_OK		-> 	as it says
 * @retval 					OTP error.....	->  usually the error will explain itself
 *
 * @code
 * 		read_otp_word(&BMM350, index, &otp_word);
 * @endcode
 */

bmm350_status_e read_otp_word(struct bmm350_dev_s *dev, uint8_t addr, uint16_t *lsb_msb){
	bmm350_status_e rslt;

	uint8_t otp_cmd, otp_status = 0, otp_err = BMM350_OTP_STATUS_NO_ERROR, lsb = 0, msb = 0;

	if(lsb_msb != NULL){
		/* Set OTP command at specified address */
		otp_cmd = BMM350_OTP_CMD_DIR_READ | (addr & BMM350_OTP_WORD_ADDR_MSK);
		rslt = dev->write(dev, BMM350_REG_OTP_CMD_REG, &otp_cmd, 1);
		if(rslt == BMM350_OK){
			do{
				dev->delay_us(300);

				//Get OTP status
				rslt = dev->read(dev, BMM350_REG_OTP_STATUS_REG, &otp_status, 1);

				otp_err = BMM350_OTP_STATUS_ERROR(otp_status);
				if(otp_err != BMM350_OTP_STATUS_NO_ERROR){
					rslt = OTP_E_handler(dev, otp_err);
					break;
				}
			}while((!(otp_status & BMM350_OTP_STATUS_CMD_DONE)) && (rslt == BMM350_OK));
		}

		if(rslt == BMM350_OK){
			/* Get OTP MSB data */
			rslt =dev->read(dev, BMM350_REG_OTP_DATA_MSB_REG, &msb, 1);
			if(rslt == BMM350_OK){
				/* Get OTP LSB data */
				rslt = dev->read(dev, BMM350_REG_OTP_DATA_LSB_REG, &lsb, 1);
				*lsb_msb = ((uint16_t)(msb << 8) | lsb) & 0xFFFF;

			}
		}
	}else{
		rslt = BMM350_E_NULL_PTR;
	}
	return rslt;
}


/*!
 * @note			This is an internal type function
 * @brief:			This internal function* is used to validate the device structure pointer for
 * 				null conditions.
 *
 * @param[in] dev 	structure pointer to device information structure.
 *
 * @retval  		BMM350_OK			-> 	as it says
 * @retval 			BMM350_E_NULL_PTR	->  device structure and or one of its members are NULL
 *
 * @code
 *  	null_ptr_check(&BMM350);
 * @endcode
 * */

bmm350_status_e null_ptr_check(struct bmm350_dev_s *dev){
	bmm350_status_e rslt;

	if((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_us == NULL)){
        /* Device structure pointer is not valid */
        rslt = BMM350_E_NULL_PTR;
	}else{
        /* Device structure is fine */
        rslt = BMM350_OK;
	}

	return rslt;
}

/*!
 * @note						This is an internal type function
 * @brief:						This function converts the ADC data from the BMM350 data registers to
 * 							32-bit signed integer.
 *
 * @param[in] inval 			value that need to be fixed
 * @param[in] number_of_bits	number of bits for the value
 *
 * @return 						Fixed number as needed
 *
 * @code
 *  	fix_sign(raw_mag_x, BMM350_SIGNED_24_BIT);
 * @endcode
 */

int32_t fix_sign(uint32_t inval, int8_t number_of_bits){
    int32_t power = 0;
    int32_t retval;

    switch (number_of_bits)
    {
        case BMM350_SIGNED_8_BIT:
            power = 128; /* 2^7 */
            break;

        case BMM350_SIGNED_12_BIT:
            power = 2048; /* 2^11 */
            break;

        case BMM350_SIGNED_16_BIT:
            power = 32768; /* 2^15 */
            break;

        case BMM350_SIGNED_21_BIT:
            power = 1048576; /* 2^20 */
            break;

        case BMM350_SIGNED_24_BIT:
            power = 8388608; /* 2^23 */
            break;

        default:
            power = 0;
            break;
    }

    retval = (int32_t)inval;

    if (retval >= power)
    {
        retval = retval - (power * 2);
    }

    return retval;
}

/*!
 * @note				This is an internal type function
 * @brief:				This function handles errors when reading OTP register.
 *
 * @param[out] dev 		structure pointer to device information structure.
 * @param[in]  otp_err	the error number the function will investigate.
 *
 * @return the enum explains the error
 *
 * @code
 *  	BMM350_I2C_read(&BMM350, BMM350_REG_CHIP_ID, &chip_id, 1);
 * @endcode
 */

bmm350_status_e OTP_E_handler(struct bmm350_dev_s *dev, uint8_t otp_err){
	bmm350_status_e rslt;
	switch(otp_err){
    case BMM350_OTP_STATUS_BOOT_ERR:
        rslt = BMM350_E_OTP_BOOT;
        break;
    case BMM350_OTP_STATUS_PAGE_RD_ERR:
        rslt = BMM350_E_OTP_PAGE_RD;
        break;
    case BMM350_OTP_STATUS_PAGE_PRG_ERR:
        rslt = BMM350_E_OTP_PAGE_PRG;
        break;
    case BMM350_OTP_STATUS_SIGN_ERR:
        rslt = BMM350_E_OTP_SIGN;
        break;
    case BMM350_OTP_STATUS_INV_CMD_ERR:
        rslt = BMM350_E_OTP_INV_CMD;
        break;
    default:
        rslt = BMM350_E_OTP_UNDEFINED;
        break;
	}
	dev->bmm350_err = rslt;
	return rslt;
}
/** @} */ // end of internal functions



#endif /* SRC_BMM350_IMU_BMM350_C_ */
