#ifndef __FASTPID_H
#define __FASTPID_H

#include "stdint.h"

// PID结构体
typedef struct
{
    float A0;        /**< The derived gain, A0 = Kp + Ki + Kd . */
    float A1;        /**< The derived gain, A1 = -Kp - 2Kd. */
    float A2;        /**< The derived gain, A2 = Kd . */
    float state[3];  /**< The state array of length 3. */
    float Kp;        /**< The proportional gain. */
    float Ki;        /**< The integral gain. */
    float Kd;        /**< The derivative gain. */
    uint32_t MaxOut; /**< The Max output. */
} fastPID_t;

// PID初始化
void FastPID_Init(fastPID_t *S);

// 增量式PID计算, 输入in为误差值
static inline float FastPID_Calc(fastPID_t *S, float in)
{
    float out;

    /* detPID = Kp*(Error_1(k)-Error_1(k-1)) + Ki*(Error_1(k)) + Kd*(Error_1(k)-2*Error_2(k-1)+Error_2(k-2)) */
    out = (S->A0 * in) + (S->A1 * S->state[0]) + (S->A2 * S->state[1]) + (S->state[2]);

    /* Update state */
    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    if (out > S->MaxOut)
    {
        return (S->MaxOut);
    }
    else if (out < -S->MaxOut)
    {
        return -(S->MaxOut);
    }

    return (out);
}

#endif
