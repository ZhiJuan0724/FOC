#include "Driver.h"

void Driver3_Init(Driver3_t *driver, TIM_HandleTypeDef *driver_handle,
                  uint32_t channel_a, uint32_t channel_b, uint32_t channel_c,
                  GPIO_Pin_Info_t en_a, GPIO_Pin_Info_t en_b, GPIO_Pin_Info_t en_c)
{
    driver->driver_handle = driver_handle;
    driver->channel_a = channel_a;
    driver->channel_b = channel_b;
    driver->channel_c = channel_c;
    driver->en_a = en_a;
    driver->en_b = en_b;
    driver->en_c = en_c;
    driver->ARR = __HAL_TIM_GET_AUTORELOAD(driver_handle);
    driver->init = 1;
    driver->freq = 1000;
    driver->state = 0;
}

void Driver3_Start(Driver3_t *driver)
{
    if (!driver->init)
        return;
    driver->state = 1;
    HAL_TIM_PWM_Start(driver->driver_handle, driver->channel_a);
    HAL_TIM_PWM_Start(driver->driver_handle, driver->channel_b);
    HAL_TIM_PWM_Start(driver->driver_handle, driver->channel_c);

    Driver3_Set_Pwm(driver, 0, 0, 0);
    HAL_GPIO_WritePin(driver->en_a.GPIOX, driver->en_a.Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(driver->en_b.GPIOX, driver->en_b.Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(driver->en_c.GPIOX, driver->en_c.Pin, GPIO_PIN_SET);
}

void Driver3_Stop(Driver3_t *driver)
{
    if (driver->state == 1)
    {
        driver->state = 0;
        HAL_TIM_PWM_Stop(driver->driver_handle, driver->channel_a);
        HAL_TIM_PWM_Stop(driver->driver_handle, driver->channel_b);
        HAL_TIM_PWM_Stop(driver->driver_handle, driver->channel_c);
        HAL_GPIO_WritePin(driver->en_a.GPIOX, driver->en_a.Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(driver->en_b.GPIOX, driver->en_b.Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(driver->en_c.GPIOX, driver->en_c.Pin, GPIO_PIN_RESET);
    }
}

void Driver3_Config(Driver3_t *driver, uint64_t freq)
{
    // 必须停止后才能配置
    if (driver->state)
        return;

    driver->freq = freq;
    freq *= 2;

    uint32_t ARR = (HAL_RCC_GetHCLKFreq() / freq) - 1;
    if (driver->ARR == ARR)
        return;

    if (ARR >= 0xFFFF)
        ARR = 0xFFFF - 1;

    __HAL_TIM_SET_AUTORELOAD(driver->driver_handle, ARR);
    driver->ARR = __HAL_TIM_GET_AUTORELOAD(driver->driver_handle);
}

void Driver3_Set_Pwm(Driver3_t *driver, uint32_t pulse1, uint32_t pulse2, uint32_t pulse3)
{
    uint32_t ARR_MAX = driver->ARR;

    pulse1 = pulse1 > ARR_MAX ? (ARR_MAX - 1) : pulse1;
    pulse2 = pulse2 > ARR_MAX ? (ARR_MAX - 1) : pulse2;
    pulse3 = pulse3 > ARR_MAX ? (ARR_MAX - 1) : pulse3;

    __HAL_TIM_SET_COMPARE(driver->driver_handle, driver->channel_a, pulse1);
    __HAL_TIM_SET_COMPARE(driver->driver_handle, driver->channel_b, pulse2);
    __HAL_TIM_SET_COMPARE(driver->driver_handle, driver->channel_c, pulse3);
}
