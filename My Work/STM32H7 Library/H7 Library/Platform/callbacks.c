/**
 * @file    callback.c
 * @brief   The file contains all callback functions
 * @author  Mohammed Abdulalem
 * @date    2026-2-12
 *
 *
 * @note
 */

#include "callbacks.h"

extern int test1;

//------------------- Tiemr Callbacks -------------------//


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note	Use volatile variables for best performance
  * @param  htim : TIM handle
  */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1){

	}else if(htim->Instance == TIM4){

	}

	else if (htim->Instance == TIM6)
	{
		HAL_IncTick();
	}

	if (htim->Instance == TIM7)
	{
	//	TimerCallback++;
//		PSxConnectionHandler(&ps4);
		static uint8_t led = 0;
		if (++led > 4) {
			led2 = !led2;
			led = 0;
	//			PSxConnectionHandler(&ps4);
	//		if (HAL_GetTick()-modbus.time > 50){
	//			Modbus_request_new_data(&modbus);
	//		}
		}

	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) {
	  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1)){
		  QEI1.count--;
	  }
	  else {
		  QEI1.count++;
	  }
    }

    if (htim->Instance == TIM8) {
	  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim8)){
		  QEI2.count--;
	  }
	  else {
		  QEI2.count++;
	  }
    }

}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_TriggerCallback could be implemented in the user file
   */
}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_ErrorCallback could be implemented in the user file
   */
}


//------------------- UART Callbacks -------------------//
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_TxCpltCallback can be implemented in the user file.
   */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == IMU.huartx) {
		IMU_Handler(&IMU);
	}
	else if (huart == IMU63.huartx) {
		WTIMU63_Handler(&IMU63);
	}

	if(huart->Instance == UART9){
//		HAL_UART_Receive_IT(&huart9, &buff, 1);
	}
	if(huart->Instance == UART5){
//		HAL_UART_Receive_IT(&huart5, &buff, 1);
	}
	if(huart->Instance == UART7){
//		HAL_UART_Receive_IT(&huart7, &buff, 1);
	}
	if(huart->Instance == UART4){
//		HAL_UART_Receive_IT(&huart4, &buff, 1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback can be implemented in the user file.
   */
}


//------------------- ADC Callbacks -------------------//


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(hadc);

	if(hadc->Instance == ADC1){

	}else if(hadc->Instance == ADC2){

	}else if(hadc->Instance == ADC3){

	}

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
}
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(hadc);
	if(hadc->Instance == ADC1){

	}else if(hadc->Instance == ADC2){

	}else if(hadc->Instance == ADC3){

	}
  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ErrorCallback must be implemented in the user file.
  */
}



//------------------- I2C Callbacks -------------------//

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterTxCpltCallback could be implemented in the user file
   */
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterRxCpltCallback could be implemented in the user file
   */
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_SlaveTxCpltCallback could be implemented in the user file
   */
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == ps4.h7i2cps4->hi2c){
		PSx_SlaveHandler(&ps4);
	}
//	else if(hi2c==enc.hi2c){
//		PMW3901_SlaveHandler(&enc);
//	}
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MemTxCpltCallback could be implemented in the user file
   */
}


void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MemRxCpltCallback could be implemented in the user file
   */
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_ErrorCallback could be implemented in the user file
   */
}



//------------------- FDCAN Callbacks -------------------//

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	PACKET_t source = 0;
	//** FDCAN1 **//
	if(hfdcan->Instance == FDCAN1){
		if(HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &h7fdcan1.rxHeader, h7fdcan1.rxData) != HAL_OK){
			Error_Handler();
		}
		// Handle Extended IDs
		if(h7fdcan1.rxHeader.IdType == FDCAN_EXTENDED_ID){ // VESc and cyberGear
			if ((h7fdcan1.rxHeader.Identifier & 0xFF) >= 112 && (h7fdcan1.rxHeader.Identifier & 0xFF) < 127) {
				vescmsg.Rxmsg = h7fdcan1.rxHeader;
				memcpy(vescmsg.Data, h7fdcan1.rxData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			} else {
				Cybergear_FDCAN_Handler(&cybergear1, &h7fdcan1.rxHeader, h7fdcan1.rxData);
				source = CYBERGEAR_PACKET;
			}
		}
		// Standard IDs
		else if(h7fdcan1.rxHeader.IdType == FDCAN_STANDARD_ID){
			uint8_t j60_cmd = (h7fdcan1.rxHeader.Identifier >> 5);
			uint8_t j60_id 	= (h7fdcan1.rxHeader.Identifier & 0x1F);


			if (j60_id == J60_1.id) {

				// Direct the data based on the command type
				if (j60_cmd == J60_CMD_CONTROL) {
					// This updates position, velocity, torque, and temp
					J60_ParseFeedback(&J60_1, h7fdcan1.rxData, h7fdcan1.rxHeader.Identifier);
				}
				else if (j60_cmd == J60_CMD_GET_STATUS) {
					// Specifically handle the status/error word
					J60_1.error_code = (h7fdcan1.rxData[0] | (h7fdcan1.rxData[1] << 8));
					// You can also extract voltage/current here if needed
				}
			}
			else if(h7fdcan1.rxHeader.Identifier >= 0x201 && h7fdcan1.rxHeader.Identifier <= 0x208){
				RBMS_CAN_Handler(&h7fdcan1.rxHeader, h7fdcan1.rxData, 1);

			}
		}
		CAN_PROCESS(source);

		if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){
			Error_Handler();
		}
	}



	//** FDCAN2 **//
	if(hfdcan->Instance == FDCAN2){

		if(HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &h7fdcan2.rxHeader, h7fdcan2.rxData) != HAL_OK){
			Error_Handler();
		}
		// Handle Extended IDs
		if(h7fdcan2.rxHeader.IdType == FDCAN_EXTENDED_ID){ // VESc and cyberGear
			if ((h7fdcan2.rxHeader.Identifier & 0xFF) >= 112 && (h7fdcan2.rxHeader.Identifier & 0xFF) < 127) {
				vescmsg.Rxmsg = h7fdcan2.rxHeader;
				memcpy(vescmsg.Data, h7fdcan2.rxData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			}else {
				Cybergear_FDCAN_Handler(&cybergear1, &h7fdcan2.rxHeader, h7fdcan2.rxData);
				source = CYBERGEAR_PACKET;
			}
		}else if(h7fdcan2.rxHeader.IdType == FDCAN_STANDARD_ID){
			uint8_t j60_cmd = (h7fdcan2.rxHeader.Identifier >> 5);
			uint8_t j60_id 	= (h7fdcan2.rxHeader.Identifier & 0x1F);


			if (j60_id == J60_1.id) {

				// Direct the data based on the command type
				if (j60_cmd == J60_CMD_CONTROL) {
					// This updates position, velocity, torque, and temp
					J60_ParseFeedback(&J60_1, h7fdcan2.rxData, h7fdcan2.rxHeader.Identifier);
				}
				else if (j60_cmd == J60_CMD_GET_STATUS) {
					// Specifically handle the status/error word
					J60_1.error_code = (h7fdcan2.rxData[0] | (h7fdcan2.rxData[1] << 8));
					// You can also extract voltage/current here if needed
				}
			}
			else if(h7fdcan2.rxHeader.Identifier >= 0x201 && h7fdcan2.rxHeader.Identifier <= 0x208){
				RBMS_CAN_Handler(&h7fdcan2.rxHeader, h7fdcan2.rxData, 2);

			}
		}
		CAN_PROCESS(source);

		if(HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){
			Error_Handler();
		}
	}


	//** FDCAN3 **//
	if(hfdcan->Instance == FDCAN3){

		if(HAL_FDCAN_GetRxMessage(&hfdcan3, FDCAN_RX_FIFO0, &h7fdcan3.rxHeader, h7fdcan3.rxData) != HAL_OK){
			Error_Handler();
		}
		// Handle Extended IDs
		if(h7fdcan3.rxHeader.IdType == FDCAN_EXTENDED_ID){ // VESc and cyberGear
			if ((h7fdcan3.rxHeader.Identifier & 0xFF) >= 112 && (h7fdcan3.rxHeader.Identifier & 0xFF) < 127) {
				vescmsg.Rxmsg = h7fdcan3.rxHeader;
				memcpy(vescmsg.Data, h7fdcan3.rxData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			}else {
				Cybergear_FDCAN_Handler(&cybergear1, &h7fdcan3.rxHeader, h7fdcan3.rxData);
				source = CYBERGEAR_PACKET;
			}

			// Handle Standard IDs
		}else if(h7fdcan3.rxHeader.IdType == FDCAN_STANDARD_ID){
			uint8_t j60_cmd = (h7fdcan3.rxHeader.Identifier >> 5);
			uint8_t j60_id 	= (h7fdcan3.rxHeader.Identifier & 0x1F);


			if (j60_id == J60_1.id) {

				// Direct the data based on the command type
				if (j60_cmd == J60_CMD_CONTROL) {
					// This updates position, velocity, torque, and temp
					J60_ParseFeedback(&J60_1, h7fdcan3.rxData, h7fdcan3.rxHeader.Identifier);
				}
				else if (j60_cmd == J60_CMD_GET_STATUS) {
					// Specifically handle the status/error word
					J60_1.error_code = (h7fdcan3.rxData[0] | (h7fdcan3.rxData[1] << 8));
					// You can also extract voltage/current here if needed
				}
			}
			else if(h7fdcan3.rxHeader.Identifier >= 0x201 && h7fdcan3.rxHeader.Identifier <= 0x208){
				RBMS_CAN_Handler(&h7fdcan3.rxHeader, h7fdcan3.rxData, 3);

			}
		}
		CAN_PROCESS(source);
		if(HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){
			Error_Handler();
		}
	}
        // Check for FIFO overrun
//        if (RxFifo0ITs & FDCAN_IT_RX_FIFO0_MESSAGE_LOST)
//        {
//            can_errors++;
//        }


}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs){
	PACKET_t source = 0;

	//** FDCAN1 **//
	if(hfdcan->Instance == FDCAN1){
		if(HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO1, &h7fdcan1.rxHeader, h7fdcan1.rxData) != HAL_OK){
			Error_Handler();
		}
		// Handle Extended IDs
		if(h7fdcan1.rxHeader.IdType == FDCAN_EXTENDED_ID){ // VESc and cyberGear
			if ((h7fdcan1.rxHeader.Identifier & 0xFF) >= 112 && (h7fdcan1.rxHeader.Identifier & 0xFF) < 127) {
				vescmsg.Rxmsg = h7fdcan1.rxHeader;
				memcpy(vescmsg.Data, h7fdcan1.rxData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			} else {
				Cybergear_FDCAN_Handler(&cybergear1, &h7fdcan1.rxHeader, h7fdcan1.rxData);
				source = CYBERGEAR_PACKET;
			}
		}else if(h7fdcan1.rxHeader.IdType == FDCAN_STANDARD_ID){
			uint8_t j60_cmd = (h7fdcan1.rxHeader.Identifier >> 5);
			uint8_t j60_id 	= (h7fdcan1.rxHeader.Identifier & 0x1F);


			if (j60_id == J60_1.id) {

				// Direct the data based on the command type
				if (j60_cmd == J60_CMD_CONTROL) {
					// This updates position, velocity, torque, and temp
					J60_ParseFeedback(&J60_1, h7fdcan1.rxData, h7fdcan1.rxHeader.Identifier);
				}
				else if (j60_cmd == J60_CMD_GET_STATUS) {
					// Specifically handle the status/error word
					J60_1.error_code = (h7fdcan1.rxData[0] | (h7fdcan1.rxData[1] << 8));
					// You can also extract voltage/current here if needed
				}
			}
			else if(h7fdcan1.rxHeader.Identifier >= 0x201 && h7fdcan1.rxHeader.Identifier <= 0x208){
				RBMS_CAN_Handler(&h7fdcan1.rxHeader, h7fdcan1.rxData, 1);

			}
		}
		CAN_PROCESS(source);

		if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK){
			Error_Handler();
		}
	}

	//** FDCAN2 **//
	if(hfdcan->Instance == FDCAN2){

		if(HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO1, &h7fdcan2.rxHeader, h7fdcan2.rxData) != HAL_OK){
			Error_Handler();
		}
		// Handle Extended IDs
		if(h7fdcan2.rxHeader.IdType == FDCAN_EXTENDED_ID){ // VESc and cyberGear
			if ((h7fdcan2.rxHeader.Identifier & 0xFF) >= 112 && (h7fdcan2.rxHeader.Identifier & 0xFF) < 127) {
				vescmsg.Rxmsg = h7fdcan2.rxHeader;
				memcpy(vescmsg.Data, h7fdcan2.rxData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			}else {
				Cybergear_FDCAN_Handler(&cybergear1, &h7fdcan2.rxHeader, h7fdcan2.rxData);
				source = CYBERGEAR_PACKET;
			}
		}else if(h7fdcan2.rxHeader.IdType == FDCAN_STANDARD_ID){
			uint8_t j60_cmd = (h7fdcan2.rxHeader.Identifier >> 5);
			uint8_t j60_id 	= (h7fdcan2.rxHeader.Identifier & 0x1F);


			if (j60_id == J60_1.id) {

				// Direct the data based on the command type
				if (j60_cmd == J60_CMD_CONTROL) {
					// This updates position, velocity, torque, and temp
					J60_ParseFeedback(&J60_1, h7fdcan2.rxData, h7fdcan2.rxHeader.Identifier);
				}
				else if (j60_cmd == J60_CMD_GET_STATUS) {
					// Specifically handle the status/error word
					J60_1.error_code = (h7fdcan2.rxData[0] | (h7fdcan2.rxData[1] << 8));
					// You can also extract voltage/current here if needed
				}
			}
			else if(h7fdcan2.rxHeader.Identifier >= 0x201 && h7fdcan2.rxHeader.Identifier <= 0x208){
				RBMS_CAN_Handler(&h7fdcan2.rxHeader, h7fdcan2.rxData, 2);

			}
		}
		CAN_PROCESS(source);

		if(HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK){
			Error_Handler();
		}
	}


		//** FDCAN3 **//
	if(hfdcan->Instance == FDCAN3){
		if(HAL_FDCAN_GetRxMessage(&hfdcan3, FDCAN_RX_FIFO1, &h7fdcan3.rxHeader, h7fdcan3.rxData) != HAL_OK){
			Error_Handler();
		}
		// Handle Extended IDs
		if(h7fdcan3.rxHeader.IdType == FDCAN_EXTENDED_ID){ // VESc and cyberGear
			if ((h7fdcan3.rxHeader.Identifier & 0xFF) >= 112 && (h7fdcan3.rxHeader.Identifier & 0xFF) < 127) {
				vescmsg.Rxmsg = h7fdcan3.rxHeader;
				memcpy(vescmsg.Data, h7fdcan3.rxData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			}else {
				Cybergear_FDCAN_Handler(&cybergear1, &h7fdcan3.rxHeader, h7fdcan3.rxData);
				source = CYBERGEAR_PACKET;
			}

		}
		// Handle Standard IDs
		else if(h7fdcan3.rxHeader.IdType == FDCAN_STANDARD_ID){
			uint8_t j60_cmd = (h7fdcan3.rxHeader.Identifier >> 5);
			uint8_t j60_id 	= (h7fdcan3.rxHeader.Identifier & 0x1F);


			if (j60_id == J60_1.id) {

				// Direct the data based on the command type
				if (j60_cmd == J60_CMD_CONTROL) {
					// This updates position, velocity, torque, and temp
					J60_ParseFeedback(&J60_1, h7fdcan3.rxData, h7fdcan3.rxHeader.Identifier);
				}
				else if (j60_cmd == J60_CMD_GET_STATUS) {
					// Specifically handle the status/error word
					J60_1.error_code = (h7fdcan3.rxData[0] | (h7fdcan3.rxData[1] << 8));
					// You can also extract voltage/current here if needed
				}
			}
			else if(h7fdcan3.rxHeader.Identifier >= 0x201 && h7fdcan3.rxHeader.Identifier <= 0x208){
				RBMS_CAN_Handler(&h7fdcan3.rxHeader, h7fdcan3.rxData, 3);

			}
		}
		CAN_PROCESS(source);
		if(HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK){
			Error_Handler();
		}
	}
}


void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorCodeITs)
{
    if (ErrorCodeITs & FDCAN_IT_BUS_OFF) {
//        can_errors = 10;
    }
}



//------------------- EXTI Callbacks -------------------//
/**
 * @brief 	This callback for EXTI pins
 * @param	GPIO_Pin
 * @warning	This should be used carfully is a pin number shares all GPIO ports,
 * so select only one port for each oin number
 * @note	ADC pins are recomended to be used
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	// Handling ADC in the board Pins
    switch(GPIO_Pin) {
        case GPIO_PIN_0: // A5 GPIOB0
            // Handle PA0, PB0, PC0, etc.

            break;

        case GPIO_PIN_1: // A6 GPIOB1

            break;

        case GPIO_PIN_2: // A2 GPIOC2
//        	PWMEncoder_Angle(&htim12, GPIOC, GPIO_PIN_2, &enc2);
            break;

        case GPIO_PIN_3: // A1 GPIOC3
//        	PWMEncoder_Angle(&htim12, GPIOC, GPIO_PIN_3, &enc1);
            break;

        case GPIO_PIN_4: // A4 GPIOA4
//        	PWMEncoder_Angle(&htim12, GPIOA, GPIO_PIN_4, &enc4);
            break;

        case GPIO_PIN_5: // A3 GPIOA5
//        	PWMEncoder_Angle(&htim12, GPIOA, GPIO_PIN_5, &enc3);
            break;

        case GPIO_PIN_6:

            break;

        case GPIO_PIN_7:

            break;

        case GPIO_PIN_8:

            break;

        case GPIO_PIN_9:

            break;

        case GPIO_PIN_10:

            break;

        case GPIO_PIN_11:

            break;

        case GPIO_PIN_12:

            break;

        case GPIO_PIN_13:

            break;

        case GPIO_PIN_14:

            break;

        case GPIO_PIN_15:

            break;

        default:

            break;
    }
}




//------------------- SPI Callbacks -------------------//


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxCpltCallback should be implemented in the user file
   */
}

/**
  * @brief Rx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_RxCpltCallback should be implemented in the user file
   */
}

/**
  * @brief Tx and Rx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxCpltCallback should be implemented in the user file
   */
}

/**
  * @brief Tx Half Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxHalfCpltCallback should be implemented in the user file
   */
}

/**
  * @brief Rx Half Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_RxHalfCpltCallback() should be implemented in the user file
   */
}

/**
  * @brief Tx and Rx Half Transfer callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxHalfCpltCallback() should be implemented in the user file
   */
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) /* Derogation MISRAC2012-Rule-8.13 */
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_ErrorCallback should be implemented in the user file
   */
  /* NOTE : The ErrorCode parameter in the hspi handle is updated by the SPI processes
            and user can use HAL_SPI_GetError() API to check the latest error occurred
   */
}
