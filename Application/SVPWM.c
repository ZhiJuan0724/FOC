#include "Svpwm.h"

/**
 * @brief 计算矢量作用时长
 * @param pSvpwm SVPWM结构体指针
 */
void __Calc_Vector_Duration(PSVPWM_t pSvpwm);

/**
 * @brief 扇区判断,利用Ualpha和Ubeta来判断扇区
 * @param pSvpwm SVPWM结构体指针
 */
void __SVPWM_Sector_Judgment(PSVPWM_t pSvpwm);

/**
 * @brief SVPWM生成周期函数,获取三相电流在一个SVPWM周期的高电平时间
 * @param pSvpwm SVPWM结构体指针
 */
void __SVPWM_Generate(PSVPWM_t pSvpwm);



void __SVPWM_Sector_Judgment(PSVPWM_t pSvpwm)
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t sector;

    pSvpwm->u1 = pSvpwm->Ubeta;
    pSvpwm->u2 = pSvpwm->Ualpha * _SQRT3_2 - pSvpwm->Ubeta / 2.0f;
    pSvpwm->u3 = -pSvpwm->Ualpha * _SQRT3_2 - pSvpwm->Ubeta / 2.0f;

    if (pSvpwm->u1 > 0)
    {
        a = 1;
    } else
    {
        a = 0;
    }
    if (pSvpwm->u2 > 0)
    {
        b = 1;
    } else
    {
        b = 0;
    }
    if (pSvpwm->u3 > 0)
    {
        c = 1;
    } else
    {
        c = 0;
    }

    sector = 4 * c + 2 * b + a;
    switch (sector)
    {
        case 3:
            pSvpwm->sector = 1;
            break;
        case 1:
            pSvpwm->sector = 2;
            break;
        case 5:
            pSvpwm->sector = 3;
            break;
        case 4:
            pSvpwm->sector = 4;
            break;
        case 6:
            pSvpwm->sector = 5;
            break;
        case 2:
            pSvpwm->sector = 6;
            break;
    }
}

void __Calc_Vector_Duration(PSVPWM_t pSvpwm)
{
    switch (pSvpwm->sector)
    {
        case 1:
            pSvpwm->t4 = _SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u2;
            pSvpwm->t6 = _SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u1;
            pSvpwm->t0 = pSvpwm->t7 = (pSvpwm->ts - pSvpwm->t4 - pSvpwm->t6) / 2.0f;
            break;
        case 2:
            pSvpwm->t2 = -_SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u2;
            pSvpwm->t6 = -_SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u3;
            pSvpwm->t0 = pSvpwm->t7 = (pSvpwm->ts - pSvpwm->t2 - pSvpwm->t6) / 2.0f;
            break;
        case 3:
            pSvpwm->t2 = _SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u1;
            pSvpwm->t3 = _SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u3;
            pSvpwm->t0 = pSvpwm->t7 = (pSvpwm->ts - pSvpwm->t2 - pSvpwm->t3) / 2.0f;
            break;
        case 4:
            pSvpwm->t1 = -_SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u1;
            pSvpwm->t3 = -_SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u2;
            pSvpwm->t0 = pSvpwm->t7 = (pSvpwm->ts - pSvpwm->t1 - pSvpwm->t3) / 2.0f;
            break;
        case 5:
            pSvpwm->t1 = _SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u3;
            pSvpwm->t5 = _SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u2;
            pSvpwm->t0 = pSvpwm->t7 = (pSvpwm->ts - pSvpwm->t1 - pSvpwm->t5) / 2.0f;
            break;
        case 6:
            pSvpwm->t4 = -_SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u3;
            pSvpwm->t5 = -_SQRT3 * pSvpwm->ts / pSvpwm->udc * pSvpwm->u1;
            pSvpwm->t0 = pSvpwm->t7 = (pSvpwm->ts - pSvpwm->t4 - pSvpwm->t5) / 2.0f;
            break;
        default:
            break;
    }
}

void __SVPWM_Generate(PSVPWM_t pSvpwm)
{
    float ta;
    float tb;
    float tc;
    switch (pSvpwm->sector)
    {
        case 1:
            ta = pSvpwm->t4 + pSvpwm->t6 + pSvpwm->t7;
            tb = pSvpwm->t6 + pSvpwm->t7;
            tc = pSvpwm->t7;
            break;
        case 2:
            ta = pSvpwm->t6 + pSvpwm->t7;
            tb = pSvpwm->t2 + pSvpwm->t6 + pSvpwm->t7;
            tc = pSvpwm->t7;
            break;
        case 3:
            ta = pSvpwm->t7;
            tb = pSvpwm->t2 + pSvpwm->t3 + pSvpwm->t7;
            tc = pSvpwm->t3 + pSvpwm->t7;
            break;
        case 4:
            ta = pSvpwm->t7;
            tb = pSvpwm->t3 + pSvpwm->t7;
            tc = pSvpwm->t1 + pSvpwm->t3 + pSvpwm->t7;
            break;
        case 5:
            ta = pSvpwm->t5 + pSvpwm->t7;
            tb = pSvpwm->t7;
            tc = pSvpwm->t1 + pSvpwm->t5 + pSvpwm->t7;
            break;
        case 6:
            ta = pSvpwm->t4 + pSvpwm->t5 + pSvpwm->t7;
            tb = pSvpwm->t7;
            tc = pSvpwm->t5 + pSvpwm->t7;
            break;
    }

    // TODO:到底是 算完就改 还是 算完缓存后再改
    uint32_t ARR = pSvpwm->driver->ARR;
    float ts = pSvpwm->ts;
    uint32_t pluseA = ta / ts * ARR;
    uint32_t pluseB = tb / ts * ARR;
    uint32_t pluseC = tc / ts * ARR;

    Driver3_Set_Pwm(pSvpwm->driver, pluseA, pluseB, pluseC);
}

void SVPWM_Control(PSVPWM_t pSvpwm, float ualpha, float ubeta)
{
    pSvpwm->Ualpha = ualpha;
    pSvpwm->Ubeta = ubeta;
    // 1.扇区判断
    __SVPWM_Sector_Judgment(pSvpwm);
    // 2.计算矢量作用时长
    __Calc_Vector_Duration(pSvpwm);
    // 3.SVPWM生成
    __SVPWM_Generate(pSvpwm);
}

void SVPWM_Init(PSVPWM_t pSvpwm, struct Driver3 *driver, uint8_t Ts, uint8_t Udc)
{
    pSvpwm->driver = driver;
    pSvpwm->Ualpha = 0;
    pSvpwm->Ubeta = 0;
    pSvpwm->u1 = 0;
    pSvpwm->u2 = 0;
    pSvpwm->u3 = 0;
    pSvpwm->t0 = 0;
    pSvpwm->t1 = 0;
    pSvpwm->t2 = 0;
    pSvpwm->t3 = 0;
    pSvpwm->t4 = 0;
    pSvpwm->t5 = 0;
    pSvpwm->t6 = 0;
    pSvpwm->t7 = 0;
    pSvpwm->ts = Ts;
    pSvpwm->udc = Udc;
    pSvpwm->sector = 0;
}

uint8_t SVPWM_Get_Sector(PSVPWM_t pSvpwm)
{
    return pSvpwm->sector;
}
