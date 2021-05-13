/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include "vlist.h"
#include "vutils.h"
#include "logme.h"
#include "DHT.h"
#include "pins.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum GPIO_OutputLevel{
    GPIO_OutputLevel_LOW = GPIO_PIN_RESET,
    GPIO_OutputLevel_HIGH = GPIO_PIN_SET,
    GPIO_OutputLevel_NOT_OUTPUT
} GPIO_OutputLevel;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define WELCOME "hello, world! From THSensorSystem :)"
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void logme_vprintf(const char* restrict format, va_list vlist){
    char buf[2048] = {0};
    vsnprintf(buf, sizeof(buf), format, vlist);
    uint32_t primask = __get_PRIMASK();
    __enable_irq();
    while (HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFFFFFF)!=HAL_OK);
//    __set_PRIMASK(primask);
}
long long logme_get_time(){
    return HAL_GetTick();
}
int logme_prepare(){
    return 1;
}
GPIO_TypeDef *getGPIOx(uint16_t pin){
    if (pin == PINVALIDE) return GPIOINVALIDE;
    switch (pin & 15U) {
        case 0U: return GPIOA;
        case 1U: return GPIOB;
        case 2U: return GPIOC;
        case 3U: return GPIOD;
        case 4U: return GPIOE;
        case 5U: return GPIOF;
        case 6U: return GPIOG;
        default: return GPIOINVALIDE;
    }
}
uint16_t getGPIO_Pin(uint16_t pin){
    if (pin == PINVALIDE) return GPIO_PIN_INVALID_E;
    switch (pin >> 4U) {
        case 0U: return GPIO_PIN_0;
        case 1U: return GPIO_PIN_1;
        case 2U: return GPIO_PIN_2;
        case 3U: return GPIO_PIN_3;
        case 4U: return GPIO_PIN_4;
        case 5U: return GPIO_PIN_5;
        case 6U: return GPIO_PIN_6;
        case 7U: return GPIO_PIN_7;
        case 8U: return GPIO_PIN_8;
        case 9U: return GPIO_PIN_9;
        case 10U: return GPIO_PIN_10;
        case 11U: return GPIO_PIN_11;
        case 12U: return GPIO_PIN_12;
        case 13U: return GPIO_PIN_13;
        case 14U: return GPIO_PIN_14;
        case 15U: return GPIO_PIN_15;
        default: return GPIO_PIN_INVALID_E;
    }
}
uint16_t getPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    uint16_t res = 0;
    switch (GPIO_Pin) {
        case GPIO_PIN_0: res += 0U<<4U; break;
        case GPIO_PIN_1: res += 1U<<4U; break;
        case GPIO_PIN_2: res += 2U<<4U; break;
        case GPIO_PIN_3: res += 3U<<4U; break;
        case GPIO_PIN_4: res += 4U<<4U; break;
        case GPIO_PIN_5: res += 5U<<4U; break;
        case GPIO_PIN_6: res += 6U<<4U; break;
        case GPIO_PIN_7: res += 7U<<4U; break;
        case GPIO_PIN_8: res += 8U<<4U; break;
        case GPIO_PIN_9: res += 9U<<4U; break;
        case GPIO_PIN_10: res += 10U<<4U; break;
        case GPIO_PIN_11: res += 11U<<4U; break;
        case GPIO_PIN_12: res += 12U<<4U; break;
        case GPIO_PIN_13: res += 13U<<4U; break;
        case GPIO_PIN_14: res += 14U<<4U; break;
        case GPIO_PIN_15: res += 15U<<4U; break;
        default: return PINVALIDE;
    }
    if (GPIOx == GPIOA){
        res += 0U;
    } else if (GPIOx == GPIOB){
        res += 1U;
    } else if (GPIOx == GPIOC){
        res += 2U;
    } else if (GPIOx == GPIOD){
        res += 3U;
    } else if (GPIOx == GPIOE){
        res += 4U;
    } else if (GPIOx == GPIOF){
        res += 5U;
    } else if (GPIOx == GPIOG){
        res += 6U;
    } else{
        return PINVALIDE;
    }
    return res;
}
void GPIO_Configure(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_OutputLevel output_level, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    switch (GPIO_Pin) {
        case GPIO_PIN_0:
        case GPIO_PIN_1:
        case GPIO_PIN_2:
        case GPIO_PIN_3:
        case GPIO_PIN_4:
        case GPIO_PIN_5:
        case GPIO_PIN_6:
        case GPIO_PIN_7:
        case GPIO_PIN_8:
        case GPIO_PIN_9:
        case GPIO_PIN_10:
        case GPIO_PIN_11:
        case GPIO_PIN_12:
        case GPIO_PIN_13:
        case GPIO_PIN_14:
        case GPIO_PIN_15:
            break;
        default:
            return;
    }

    /* GPIO Ports Clock Enable */
    if (GPIOx == GPIOA){
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (GPIOx == GPIOB){
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (GPIOx == GPIOC){
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (GPIOx == GPIOD){
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (GPIOx == GPIOE){
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (GPIOx == GPIOF){
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (GPIOx == GPIOG){
        __HAL_RCC_GPIOG_CLK_ENABLE();
    } else{
        return;
    }

    switch (output_level) {
        case GPIO_OutputLevel_LOW:
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
            break;
        case GPIO_OutputLevel_HIGH:
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }

    /* Configure GPIO pin */
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_Mode;
    GPIO_InitStruct.Pull = GPIO_Pull;
    /* For most families there are dedicated registers so it is
     * not so important, register can be set at any time.But for
     * families like F1, speed only applies to output. */
    if (GPIO_Mode == GPIO_MODE_OUTPUT_PP || GPIO_Mode == GPIO_MODE_OUTPUT_OD) {
        GPIO_InitStruct.Speed = GPIO_Speed;
    }
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
void DHT_pinMode_in(uint16_t pin, DHT_PIN_MODE mode, int default_output_low){
    switch (mode) {
        case DHT_PIN_MODE_INPUT:
            GPIO_Configure(getGPIOx(pin), getGPIO_Pin(pin), GPIO_OutputLevel_NOT_OUTPUT, GPIO_MODE_INPUT, GPIO_NOPULL,
#ifdef GPIO_SPEED_VERY_HIGH
                    GPIO_SPEED_VERY_HIGH
#else
                           GPIO_SPEED_HIGH
#endif
            );
            break;
        case DHT_PIN_MODE_INPUT_PULLUP:
            GPIO_Configure(getGPIOx(pin), getGPIO_Pin(pin), GPIO_OutputLevel_NOT_OUTPUT, GPIO_MODE_INPUT, GPIO_PULLUP,
#ifdef GPIO_SPEED_VERY_HIGH
                    GPIO_SPEED_VERY_HIGH
#else
                           GPIO_SPEED_HIGH
#endif
            );
            break;
        case DHT_PIN_MODE_OUTPUT:
            GPIO_Configure(getGPIOx(pin), getGPIO_Pin(pin), default_output_low ? GPIO_OutputLevel_LOW : GPIO_OutputLevel_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
#ifdef GPIO_SPEED_VERY_HIGH
                    GPIO_SPEED_VERY_HIGH
#else
                           GPIO_SPEED_HIGH
#endif
            );
            break;
        default:
            break;
    }
}
void DHT_pinMode_Output_Low(uint16_t pin, DHT_PIN_MODE mode){
    DHT_pinMode_in(pin, mode, 1);
}
void DHT_pinMode_Output_High(uint16_t pin, DHT_PIN_MODE mode){
    DHT_pinMode_in(pin, mode, 0);
}
void DHT_pinMode(uint16_t pin, DHT_PIN_MODE mode){
    DHT_pinMode_Output_Low(pin, mode);
}
void DHT_delay(unsigned long ms){
    while (ms > 0UL) {
        uint32_t temp_ms = ms > UINT32_MAX ? UINT32_MAX : ms;
        HAL_Delay(temp_ms);
        ms -= temp_ms;
    }
}
uint32_t getUs(void) {
    uint32_t usTicks = HAL_RCC_GetSysClockFreq() / (uint32_t)1000000;
    register uint32_t ms, cycle_cnt;
    do {
        ms = HAL_GetTick();
        cycle_cnt = SysTick->VAL;
    } while (ms != HAL_GetTick());
    return (ms * (uint32_t)1000) + (usTicks * (uint32_t)1000 - cycle_cnt) / usTicks;
}
void delayUs(uint32_t us) {
    uint32_t start = getUs();
    while (getUs() - start < us);
}
void delayUs_tim(uint16_t us){
    HAL_TIM_Base_Start(&htim2);
    __HAL_TIM_SetCounter(&htim2, us);
    while(__HAL_TIM_GetCounter(&htim2)>1);
    HAL_TIM_Base_Stop(&htim2);
}
void delayUs_tim_long_long(unsigned long long us){
    while (us > 0ULL){
        uint16_t temp_us = us > UINT16_MAX ? UINT16_MAX : us;
        delayUs_tim(temp_us);
        us -= temp_us;
    }
}
void DHT_delayMicroseconds(unsigned long us) {
    delayUs_tim_long_long(us);
}
void DHT_digitalWrite(uint16_t pin, DHT_PIN_VALUE value){
    switch (value) {
        case DHT_PIN_VALUE_LOW:
            HAL_GPIO_WritePin(getGPIOx(pin), getGPIO_Pin(pin), GPIO_PIN_RESET);
            break;
        case DHT_PIN_VALUE_HIGH:
            HAL_GPIO_WritePin(getGPIOx(pin), getGPIO_Pin(pin), GPIO_PIN_SET);
            break;
        default:
            break;
    }
}
DHT_PIN_VALUE DHT_digitalRead(uint16_t pin){
    switch (HAL_GPIO_ReadPin(getGPIOx(pin), getGPIO_Pin(pin))) {
        default:
        case GPIO_PIN_RESET:
            return DHT_PIN_VALUE_LOW;
        case GPIO_PIN_SET:
            return DHT_PIN_VALUE_HIGH;
    }
}
long long DHT_microsecondsToClockCycles(long long us){
    return ( (us) * ((long long)SystemCoreClock / 1000000LL) );
}
void DHT_disable_interrupt(){
    __disable_irq();
}
void DHT_enable_interrupt(){
    __enable_irq();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    logme_init();
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    DHT_digitalWrite(getPin(GPIOB, GPIO_PIN_9), DHT_PIN_VALUE_HIGH);
    HAL_Delay(1100); // wait for DHT to be stable
    dht_init(getPin(GPIOA, GPIO_PIN_1));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      vlist list = make_vlist(5);
      char *rabbit = zero_malloc(5);
      if (rabbit == NULL) continue;
//      LogMe.b("%s", WELCOME);
      LogMe.bt("%s", WELCOME);
      float temperature, humidity;
      int res = dht_readTemperatureAndHumidity(getPin(GPIOA, GPIO_PIN_1), dht_sensor_type_DHT11, dht_temperature_scale_Celcius, &temperature, &humidity);
      if (res){
          LogMe.bt("Temperature = %f (C), Humidity = %f", temperature, humidity);
      } else{
          LogMe.bt("Temperature = Fail (C), Humidity = Fail");
      }
//      HAL_Delay(2100);
      delayUs_tim_long_long(2100000ULL);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7;
  htim2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
