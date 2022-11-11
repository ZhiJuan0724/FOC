/**
 ******************************************************************************
 * @file           : Encoder.h
 * @brief          : Encoder head
 ******************************************************************************
 * @attention
 * 使用STM32的TIM外设的Encoder Mode实现
 * 在STM32CUBEMX中配置和生成代码时需注意:
 * 1.选择 EncoderMode TI1 and TI2 时 cpr值设置为编码器标示值*4
 * 2.用于编码器更新和速度计算的定时器的触发时间尽量不高于10ms
 ******************************************************************************
 */ 
#ifndef _ENCODER_H
#define _ENCODER_H

#include "stm32f1xx_hal.h"
#include "tim.h"

/**
 * Encoder 结构体
 */
typedef struct Encoder
{
  TIM_HandleTypeDef *encoder_handle;
  TIM_HandleTypeDef *caculate_handle;
  int64_t cpr;
  uint32_t last_CNT;
  uint32_t now_CNT;
  float angle_el;
  float angle;
  int32_t circle;
  float velocity;
  uint8_t state;
  uint8_t init;
} Encoder_t;

/**
 * @brief Encoder结构体初始化
 * @param encoder 编码器结构体指针
 * @param encoder_handle 编码器使用的定时器句柄指针
 * @param caculate_handle 编码器用于更新和计算的定时器句柄指针
 * @param cpr 编码器旋转一圈触发数
 */
void Encoder_Init(Encoder_t *encoder,
                  TIM_HandleTypeDef *encoder_handle, TIM_HandleTypeDef *caculate_handle, int64_t cpr);

/**
 * @brief Encoder启动
 * @param encoder 编码器结构体指针
 */
void Encoder_Start(Encoder_t *encoder);

/**
 * @brief Encoder停止
 * @param encoder 编码器结构体指针
 */
void Encoder_Stop(Encoder_t *encoder);

/**
 * @brief Encoder数据清零
 * @param encoder 编码器结构体指针
 */
void Encoder_Clear(Encoder_t *encoder);

/**
 * @brief Encoder更新和速度计算
 * @param encoder 编码器结构体指针
 */
void Encoder_Update(Encoder_t *encoder);

/**
 * @brief 获取Encoder归一化角度
 * @param encoder 编码器结构体指针
 * @return 归一化角度
 */
float Encoder_Get_Angle_El(Encoder_t *encoder);

/**
 * @brief 获取Encoder角度
 * @param encoder 编码器结构体指针
 * @return 角度
 */
float Encoder_Get_Angle(Encoder_t *encoder);

/**
 * @brief 获取Encoder速度
 * @param encoder 编码器结构体指针
 * @return 速度
 */
float Encoder_Get_Velocity(Encoder_t *encoder);

#endif
