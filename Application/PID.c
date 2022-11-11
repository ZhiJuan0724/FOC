#include "PID.h"

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

