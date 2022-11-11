#include "PID.h"
//
////速度PID初始化
//void SpeedPID_Init(PSpeedPID_t pSpeedPID, uint8_t xOutMax,
//                   Get_Motor_Speed_ft xGet_Motor_Speed, Set_SpeedPID_Tarout_ft xSet_SpeedPID_Tarout)
//{
//    struct PID sdefPID = {0};
//    pSpeedPID->SPID = sdefPID;
//    pSpeedPID->SPID.outMax = xOutMax;
//    pSpeedPID->Get_Motor_Speed = xGet_Motor_Speed;
//    pSpeedPID->Set_SpeedPID_Tarout = xSet_SpeedPID_Tarout;
//}
//
////速度PID控制
//void SpeedPID_Calc(PSpeedPID_t pSpeedPID)
//{
//    pSpeedPID->SPID.pre = pSpeedPID->Get_Motor_Speed();  //获取电机实际转速
//    pSpeedPID->SPID.err = pSpeedPID->SPID.tar - pSpeedPID->SPID.pre;
//    pSpeedPID->SPID.out += pSpeedPID->SPID.kp * (pSpeedPID->SPID.err - pSpeedPID->SPID.lastErr) +
//                           pSpeedPID->SPID.ki * pSpeedPID->SPID.err;
//    pSpeedPID->SPID.lastErr = pSpeedPID->SPID.err;
//
//    if (pSpeedPID->SPID.out > pSpeedPID->SPID.outMax)
//    {
//        pSpeedPID->SPID.out = pSpeedPID->SPID.outMax;
//    } else if (pSpeedPID->SPID.out < -pSpeedPID->SPID.outMax)
//    {
//        pSpeedPID->SPID.out = -pSpeedPID->SPID.outMax;
//    }
//
//    pSpeedPID->Set_SpeedPID_Tarout(pSpeedPID->SPID.out, 0);
//}
//
////速度PID循环函数(2ms)
//void SpeedPID_Cycle(PSpeedPID_t pSpeedPID)
//{
//    //2ms计算一次pid
//    SpeedPID_Calc(pSpeedPID);
//}
//
////设置速度PID的pid参数
//void Set_SpeedPID_Params(PSpeedPID_t pSpeedPID, float kp, float ki, float kd)
//{
//    pSpeedPID->SPID.kp = kp;
//    pSpeedPID->SPID.ki = ki;
//    pSpeedPID->SPID.kd = kd;
//}
//
////设置速度环的目标速度speed_ref
//void Set_SpeedPID_VelocityTar(PSpeedPID_t pSpeedPID, float tar)
//{
//    pSpeedPID->SPID.tar = tar;
//}
//
///*-----------------------------------------------------------------------------------------------*/
////位置PID初始化
//void PositionPID_Init(PPositionPID_t PPositionPID, uint8_t xOutMax,
//                      Get_Motor_Position_ft xGet_Motor_Position, Set_PositionPID_Tarout_ft xSet_PositionPID_Tarout)
//{
//    struct PID pdefPID = {0};
//    PPositionPID->PPID = pdefPID;
//    PPositionPID->PPID.outMax = xOutMax;
//    PPositionPID->Get_Motor_Position = xGet_Motor_Position;
//    PPositionPID->Set_PositionPID_Tarout = xSet_PositionPID_Tarout;
//}
//
//void PositionPID_Calc(PPositionPID_t PPositionPID)
//{
//    float pre_err;
//    PPositionPID->PPID.pre = PPositionPID->Get_Motor_Position();
//
//    pre_err = PPositionPID->PPID.pre - PPositionPID->PPID.tar;
//    if (pre_err > 180)
//    {
//        pre_err -= 360;
//    } else if (pre_err < -180)
//    {
//        pre_err += 360;
//    }
//
//    PPositionPID->PPID.err = 0 - pre_err;
//    PPositionPID->PPID.out = PPositionPID->PPID.kp * PPositionPID->PPID.err
//                             + PPositionPID->PPID.ki * PPositionPID->PPID.AllErr
//                             + PPositionPID->PPID.kd * (PPositionPID->PPID.err - PPositionPID->PPID.lastErr);
//    PPositionPID->PPID.lastErr = PPositionPID->PPID.err;
//    PPositionPID->PPID.AllErr += PPositionPID->PPID.err;
//
//    if (PPositionPID->PPID.out > PPositionPID->PPID.outMax)
//    {
//        PPositionPID->PPID.out = PPositionPID->PPID.outMax;
//    } else if (PPositionPID->PPID.out < -PPositionPID->PPID.outMax)
//    {
//        PPositionPID->PPID.out = -PPositionPID->PPID.outMax;
//    }
//    PPositionPID->Set_PositionPID_Tarout(PPositionPID->PPID.out);
//}
//
////位置PID循环函数(5ms)
//void PositionPID_Cycle(PPositionPID_t PPositionPID)
//{
//    //5ms计算一次pid
//    PositionPID_Calc(PPositionPID);
//}
//
////设置位置PID的pid参数
//void Set_PositionPID_Params(PPositionPID_t PPositionPID, float kp, float ki, float kd)
//{
//    PPositionPID->PPID.kp = kp;
//    PPositionPID->PPID.ki = ki;
//    PPositionPID->PPID.kd = kd;
//}
//
////设置位置环的目标位置position_ref
//void Set_PositionPID_PositionTar(PPositionPID_t PPositionPID, float tar)
//{
//    PPositionPID->PPID.tar = tar;
//}

/*-----------------------------------------------------------------------------------------------*/
void PID_Init(PID_t *pid)
{
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->err = 0;
    pid->lastErr = 0;
    pid->AllErr = 0;
    pid->out = 0;
    pid->outMax = -1.0f;
    pid->type = DELTA_PID;
}

void PID_Set_Params(PID_t *pid, PID_Type_t type, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->type = type;
}

float PID_Calc(PID_t *pid, float pre, float tar)
{
    pid->err = tar - pre;

    if (pid->type == DELTA_PID)
        pid->out += pid->kp * (pid->err - pid->lastErr) + pid->ki * (pid->err);
    else if (pid->type == POSITION_PID)
        pid->out += pid->kp * (pid->err) + pid->ki * (pid->AllErr) + pid->kd * (pid->err - pid->lastErr);

    pid->lastErr = pid->err;

    if (pid->outMax > 0)
        pid->out = (pid->out > pid->outMax) ? pid->outMax : pid->out;

    return pid->out;
}

