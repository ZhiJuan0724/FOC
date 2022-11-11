#ifndef _SPEEDPID_H
#define _SPEEDPID_H

#include "Utils.h"

typedef enum PID_Type
{
    POSITION_PID = 1,
    DELTA_PID = 2,
}PID_Type_t;

typedef struct PID
{
    float kp;      //比例
    float ki;      //积分
    float kd;      //微分
    float err;     //目标与实际之前差值（目标-实际）
    float lastErr; //上次差值
    float AllErr;   //累积差值
    float out;     //输出
    float outMax;  //输出限幅
    PID_Type_t type;
}PID_t;

void PID_Init(PID_t *pid);

void PID_Set_Params(PID_t *pid, PID_Type_t type, float kp, float ki, float kd);

float PID_Calc(PID_t *pid, float pre, float tar);

#endif
