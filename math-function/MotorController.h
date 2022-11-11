#ifndef __MOTORCONTROLLER_H
#define __MOTORCONTROLLER_H

#include "main.h"

/**
 * Park变换
 * 静止坐标系转换为旋转坐标系
 */
__STATIC_FORCEINLINE void Park(float Ialpha, float Ibeta, float *pId, float *pIq, float sinVal, float cosVal)
{
    *pId = Ialpha * cosVal + Ibeta * sinVal;

    *pIq = -Ialpha * sinVal + Ibeta * cosVal;
}

/**
 * 反Park变换
 * 旋转坐标系转换为静止坐标系
 */
__STATIC_FORCEINLINE void Inv_Park(float Id, float Iq, float *pIalpha, float *pIbeta, float sinVal, float cosVal)
{
    *pIalpha = Id * cosVal - Iq * sinVal;

    *pIbeta = Id * sinVal + Iq * cosVal;
}

/**
 * Clarke变换
 * 三相坐标系转换为直角坐标系
 */
__STATIC_FORCEINLINE void Clarke(float Ia, float Ib, float *pIalpha, float *pIbeta)
{
    *pIalpha = Ia;
    *pIbeta = (0.57735026919f * Ia + 1.15470053838f * Ib);
}

/**
 * 反Clarke变换
 * 直角坐标系转换为三相坐标系
 */
__STATIC_FORCEINLINE void Inv_Clarke(float Ialpha, float Ibeta, float *pIa, float *pIb)
{
    *pIa = Ialpha;

    *pIb = -0.5f * Ialpha + 0.8660254039f * Ibeta;
}

#endif
