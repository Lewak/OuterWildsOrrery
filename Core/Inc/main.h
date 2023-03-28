/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define led_Pin GPIO_PIN_13
#define led_GPIO_Port GPIOC
#define DFplayer_busy_Pin GPIO_PIN_14
#define DFplayer_busy_GPIO_Port GPIOC
#define encoder_clk_Pin GPIO_PIN_0
#define encoder_clk_GPIO_Port GPIOA
#define encoder_dt_Pin GPIO_PIN_1
#define encoder_dt_GPIO_Port GPIOA
#define Dfplayer_TX_Pin GPIO_PIN_2
#define Dfplayer_TX_GPIO_Port GPIOA
#define Dfplayer_RX_Pin GPIO_PIN_3
#define Dfplayer_RX_GPIO_Port GPIOA
#define encoder_button_Pin GPIO_PIN_4
#define encoder_button_GPIO_Port GPIOA
#define stepper_dir_Pin GPIO_PIN_12
#define stepper_dir_GPIO_Port GPIOB
#define stepper_m1_Pin GPIO_PIN_14
#define stepper_m1_GPIO_Port GPIOB
#define stepper_m0_Pin GPIO_PIN_15
#define stepper_m0_GPIO_Port GPIOB
#define stepper_step_Pin GPIO_PIN_8
#define stepper_step_GPIO_Port GPIOA
#define neopixel_data_Pin GPIO_PIN_10
#define neopixel_data_GPIO_Port GPIOA
#define stepper_enable_Pin GPIO_PIN_12
#define stepper_enable_GPIO_Port GPIOA
#define lcd_scl_Pin GPIO_PIN_6
#define lcd_scl_GPIO_Port GPIOB
#define lcd_sda_Pin GPIO_PIN_7
#define lcd_sda_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
