#include "encoder.h"

void Encoder_Init(Encoder_t *encoder,
                  TIM_HandleTypeDef *encoder_handle, TIM_HandleTypeDef *caculate_handle, int64_t cpr)
{
    encoder->encoder_handle = encoder_handle;
    encoder->caculate_handle = caculate_handle;
    encoder->cpr = cpr;
    encoder->angle = 0.0f;
    encoder->angle_el = 0.0f;
    encoder->circle = 0;
    encoder->now_CNT = 0;
    encoder->last_CNT = 0;
    encoder->velocity = 0.0f;
    encoder->init = 0;
    if (cpr * 2 < 65535)
    {
        __HAL_TIM_SET_AUTORELOAD(encoder_handle, cpr * 2);
        encoder->init = 1;
    }
    encoder->state = 0;
}

void Encoder_Start(Encoder_t *encoder)
{
    if (!encoder->init)
        return;
    encoder->state = 1;
    HAL_TIM_Encoder_Start(encoder->encoder_handle, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(encoder->caculate_handle);
}

void Encoder_Stop(Encoder_t *encoder)
{
    encoder->state = 0;
    HAL_TIM_Base_Stop_IT(encoder->caculate_handle);
    HAL_TIM_Encoder_Stop(encoder->encoder_handle, TIM_CHANNEL_ALL);
}

void Encoder_Clear(Encoder_t *encoder)
{
    encoder->angle = 0.0f;
    encoder->angle_el = 0.0f;
    encoder->circle = 0;
    encoder->now_CNT = 0;
    encoder->last_CNT = 0;
    encoder->velocity = 0.0f;
}

void Encoder_Update(Encoder_t *encoder)
{
    encoder->now_CNT = __HAL_TIM_GET_COUNTER(encoder->encoder_handle);

    // 正转不经过零点和反转不经过零点
    int64_t chg = (int64_t) encoder->now_CNT - (int64_t) encoder->last_CNT;
    if (chg < -encoder->cpr)
    {
        // 正转经过零点
        chg = chg + encoder->cpr;
    } else if (chg > encoder->cpr)
    {
        //反转经过零点
        chg = chg - encoder->cpr;
    }
    float tmp = (float) chg / (float) encoder->cpr * _2PI;
    encoder->angle += tmp;
    encoder->angle_el += tmp;
    // velocity (rad/ms)
    // TODO 这个地方速度计算假定用户定时器周期为5ms
    encoder->velocity = tmp / 5.0f;
    encoder->circle = (int32_t) (encoder->angle / _2PI);

    if (encoder->angle_el >= _2PI)
    {
        encoder->angle_el -= _2PI;
    } else if (encoder->angle_el < 0.0f)
    {
        encoder->angle_el += _2PI;
    }
    encoder->last_CNT = encoder->now_CNT;
}

float Encoder_Get_Angle_El(Encoder_t *encoder)
{
    return encoder->angle_el;
}

float Encoder_Get_Angle(Encoder_t *encoder)
{
    return encoder->angle;
}

float Encoder_Get_Velocity(Encoder_t *encoder)
{
    return encoder->velocity;
}