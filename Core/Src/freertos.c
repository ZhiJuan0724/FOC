/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for monitorTask */
osThreadId_t monitorTaskHandle;
const osThreadAttr_t monitorTask_attributes = {
        .name = "monitorTask",
        .stack_size = 256 * 4,
        .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for controlTask */
osThreadId_t controlTaskHandle;
const osThreadAttr_t controlTask_attributes = {
        .name = "controlTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void StartMonitorTask(void *argument);

void StartControlTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void Device_Init();

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */
    Device_Init();
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of monitorTask */
    monitorTaskHandle = osThreadNew(StartMonitorTask, NULL, &monitorTask_attributes);

    /* creation of controlTask */
    controlTaskHandle = osThreadNew(StartControlTask, NULL, &controlTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        Encoder_Start(&xencoder);
        Sense_Start(&xsense);
        Driver3_Start(&xdriver);


        osThreadExit();
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartMonitorTask */
/**
* @brief Function implementing the monitorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMonitorTask */
void StartMonitorTask(void *argument)
{
    /* USER CODE BEGIN StartMonitorTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartMonitorTask */
}

/* USER CODE BEGIN Header_StartControlTask */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument)
{
    /* USER CODE BEGIN StartControlTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Device_Init()
{
    Encoder_Init(&xencoder, &htim2, &htim6, 8000);

    Driver3_Init(&xdriver, &htim1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4,
                 GPIO_EN_A, GPIO_EN_B, GPIO_EN_C);
    Driver3_Config(&xdriver, 10000);


    Sense_Init(&xsense, &hadc1, &htim1, TIM_CHANNEL_1, 3.3f);
    xsense.skip_offset = 1;

    Sense_Set_2Ch(&xsense, ADC_CHANNEL_6, ADC_CHANNEL_7, 0.01f * 50, 0.01f * 50);
    Sense_Config(&xsense,xdriver.ARR);

    Motor_Link_Encoder(&xmotor, &xencoder);
    Motor_Link_Sense(&xmotor, &xsense);
    Motor_Link_Driver3(&xmotor, &xdriver);

    Motor_Init(&xmotor, 11, 24.0f);
    xmotor.voltage_limit = 18.0f;
    xmotor.velocity_limit = 20 * _PI;

    PID_Init(&xspeedPID);
    PID_Init(&xpositionPID);
    PID_Init(&xiqPID);
    PID_Init(&xidPID);

    PID_Set_Params(&xspeedPID, DELTA_PID, 1.0f, 0, 0);
    PID_Set_Params(&xpositionPID, POSITION_PID, 0.5f, 0, 0);

    xmotor.postion_pid = xpositionPID;
    xmotor.speed_pid = xspeedPID;
    xmotor.iq_pid = xiqPID;
    xmotor.id_pid = xidPID;

    Motor_FOC_Init(&xmotor, &xfoc, &xsvpwm);

    Motor_Target_t  motor_target = {};
    motor_target.target_val = 5 * _PI;
    motor_target.target_type = VELOCITY_TARGET;
    motor_target.control_type = VELOCITY_OPEN_CONTROL;

    Motor_Set_Target(&xmotor,motor_target);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    Sense_Update(&xsense);

    Motor_Run(&xmotor);
}

/* USER CODE END Application */

