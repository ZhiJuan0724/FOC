#ifndef _DRIVER_H
#define _DRIVER_H

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "Utils.h"

/** 3相 **/
typedef struct Driver3
{
    TIM_HandleTypeDef *driver_handle;
    uint32_t channel_a;
    uint32_t channel_b;
    uint32_t channel_c;
    GPIO_Pin_Info_t en_a;
    GPIO_Pin_Info_t en_b;
    GPIO_Pin_Info_t en_c;
    // 1:运行   0:关闭
    uint32_t ARR;
    uint8_t state;
    uint8_t init;
    uint32_t freq;
} Driver3_t;

void Driver3_Init(Driver3_t *driver, TIM_HandleTypeDef *driver_handle,
                  uint32_t channel_a, uint32_t channel_b, uint32_t channel_c,
                  GPIO_Pin_Info_t en_a, GPIO_Pin_Info_t en_b, GPIO_Pin_Info_t en_c);

void Driver3_Start(Driver3_t *driver);

void Driver3_Stop(Driver3_t *driver);

void Driver3_Config(Driver3_t *driver, uint64_t freq);

void Driver3_Set_Pwm(Driver3_t *driver, uint32_t Pulse1, uint32_t Pulse2, uint32_t Pulse3);

/** 2相 **/
typedef struct Driver2
{
    TIM_HandleTypeDef *driver_handle;
    uint32_t channel_a;
    uint32_t channel_b;
    GPIO_Pin_Info_t en_a;
    GPIO_Pin_Info_t en_b;
    uint32_t freq;
} Driver2_t;

#endif
