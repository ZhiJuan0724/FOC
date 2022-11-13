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
#include "Utils.h"
#include "Encoder.h"
#include "Driver.h"
#include "Monitor.h"
#include "SVPWM.h"
#include "Foc.h"
#include "Sense.h"
#include "Motor.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
const extern struct GPIO_Pin_Info GPIO_EN_A;
const extern struct GPIO_Pin_Info GPIO_EN_B;
const extern struct GPIO_Pin_Info GPIO_EN_C;
extern struct Encoder xencoder;
extern struct Driver3 xdriver;
extern struct SVPWM xsvpwm;
extern struct FOC xfoc;
extern struct Sense xsense;
extern struct Motor xmotor;
extern struct PID xspeedPID;
extern struct PID xpositionPID;
extern struct PID xiqPID;
extern struct PID xidPID;
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
#define GPIO_STOP_Pin GPIO_PIN_3
#define GPIO_STOP_GPIO_Port GPIOE
#define GPIO_STOP_EXTI_IRQn EXTI3_IRQn
#define LED_RUN_Pin GPIO_PIN_0
#define LED_RUN_GPIO_Port GPIOC
#define ADC_A_Pin GPIO_PIN_6
#define ADC_A_GPIO_Port GPIOA
#define ADC_B_Pin GPIO_PIN_7
#define ADC_B_GPIO_Port GPIOA
#define PWM_A_Pin GPIO_PIN_11
#define PWM_A_GPIO_Port GPIOE
#define PWM_B_Pin GPIO_PIN_13
#define PWM_B_GPIO_Port GPIOE
#define PWM_C_Pin GPIO_PIN_14
#define PWM_C_GPIO_Port GPIOE
#define EN_A_Pin GPIO_PIN_8
#define EN_A_GPIO_Port GPIOD
#define EN_B_Pin GPIO_PIN_9
#define EN_B_GPIO_Port GPIOD
#define EN_C_Pin GPIO_PIN_10
#define EN_C_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
