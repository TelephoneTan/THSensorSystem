/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VOICE_Analog_in_Pin GPIO_PIN_0
#define VOICE_Analog_in_GPIO_Port GPIOC
#define LCD1602D7_Pin GPIO_PIN_2
#define LCD1602D7_GPIO_Port GPIOC
#define DHT11_data_Pin GPIO_PIN_1
#define DHT11_data_GPIO_Port GPIOA
#define LCD1602RS_Pin GPIO_PIN_2
#define LCD1602RS_GPIO_Port GPIOA
#define LCD1602RW_Pin GPIO_PIN_3
#define LCD1602RW_GPIO_Port GPIOA
#define LCD1602E_Pin GPIO_PIN_4
#define LCD1602E_GPIO_Port GPIOA
#define LCD1602D0_Pin GPIO_PIN_5
#define LCD1602D0_GPIO_Port GPIOA
#define LCD1602D1_Pin GPIO_PIN_6
#define LCD1602D1_GPIO_Port GPIOA
#define LCD1602D2_Pin GPIO_PIN_7
#define LCD1602D2_GPIO_Port GPIOA
#define LCD1602D3_Pin GPIO_PIN_4
#define LCD1602D3_GPIO_Port GPIOC
#define LCD1602D4_Pin GPIO_PIN_5
#define LCD1602D4_GPIO_Port GPIOC
#define LCD1602D5_Pin GPIO_PIN_0
#define LCD1602D5_GPIO_Port GPIOB
#define LCD1602D6_Pin GPIO_PIN_1
#define LCD1602D6_GPIO_Port GPIOB
#define SerialLog_TX_Pin GPIO_PIN_10
#define SerialLog_TX_GPIO_Port GPIOB
#define SerialLog_RX_Pin GPIO_PIN_11
#define SerialLog_RX_GPIO_Port GPIOB
#define GND_Pin GPIO_PIN_12
#define GND_GPIO_Port GPIOB
#define SerialData_TX_Pin GPIO_PIN_9
#define SerialData_TX_GPIO_Port GPIOA
#define SerialData_RX_Pin GPIO_PIN_10
#define SerialData_RX_GPIO_Port GPIOA
#define V_5V_Output_Pin GPIO_PIN_11
#define V_5V_Output_GPIO_Port GPIOA
#define V_5V_OutputA12_Pin GPIO_PIN_12
#define V_5V_OutputA12_GPIO_Port GPIOA
#define VOICE___Pin GPIO_PIN_3
#define VOICE___GPIO_Port GPIOB
#define VOICE_G_Pin GPIO_PIN_4
#define VOICE_G_GPIO_Port GPIOB
#define VOICE_Digital_in_Pin GPIO_PIN_5
#define VOICE_Digital_in_GPIO_Port GPIOB
#define VOICE_Digital_in_EXTI_IRQn EXTI9_5_IRQn
#define DHT11_power_Pin GPIO_PIN_9
#define DHT11_power_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
