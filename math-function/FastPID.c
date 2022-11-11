#include "FastPID.h"

void FastPID_Init(fastPID_t *S)
{
    S->A0 = S->Kp + S->Ki + S->Kd;

    S->A1 = (-S->Kp) - ((float)2.0f * S->Kd);

    S->A2 = S->Kd;
}
