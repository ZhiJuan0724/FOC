#ifndef Svpwm_h
#define Svpwm_h

#include "stm32f1xx_hal.h"
#include "Driver.h"
#include "Utils.h"

typedef struct SVPWM
{
    float Ualpha;                                                              //阿尔法轴目标电压 	(最大值为母线电压 * sqrt(3) / 3)
    float Ubeta;                                                               //贝塔轴目标电压   	(最大值为母线电压 * sqrt(3) / 3)
    float u1;                                                                  //用于扇区判断
    float u2;                                                                  //用于扇区判断
    float u3;                                                                  //用于扇区判断
    float t0;                                                                  // 0矢量作用时长
    float t1;                                                                  // 1矢量作用时长
    float t2;                                                                  // 2矢量作用时长
    float t3;                                                                  // 3矢量作用时长
    float t4;                                                                  // 4矢量作用时长
    float t5;                                                                  // 5矢量作用时长
    float t6;                                                                  // 6矢量作用时长
    float t7;                                                                  // 7矢量作用时长
    float ts;                                                                  // SVPWM周期
    float udc;                                                                 //母线电压
    uint8_t sector;                                                            //扇区索引
    struct Driver3 *driver;
    
}SVPWM_t;

typedef SVPWM_t *PSVPWM_t;

/**
 * @brief SVPWM结构体初始化
 * @param Ts SVPWM周期
 * @param Udc 母线电压
 * @param xSetChannelHighLeaveTime_us 设置三相绕组高电平时间
 * @param
 */  
void SVPWM_Init(PSVPWM_t pSvpwm,struct Driver3 *driver,float Ts,float Udc);

/**
 * @brief SVPWM生成周期函数,获取三相电流在一个SVPWM周期的高电平时间
 * @param pSvpwm SVPWM结构体指针
 * @param Ualpha 阿尔法轴目标电压
 * @param Ubeta 贝塔轴目标电压
 */
void SVPWM_Control(PSVPWM_t pSvpwm, float ualpha, float ubeta);

/**
 * @brief 获取SVPWM扇区
 * @param pSvpwm SVPWM结构体指针
 */
uint8_t SVPWM_Get_Sector(PSVPWM_t pSvpwm);

#endif
