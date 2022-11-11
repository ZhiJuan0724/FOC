#include "FOC.h"

/**
 * @brief 获取电机角度
 * @param pFOC Foc结构体指针
 */
void FOC_Update_Angle(PFOC_t pFOC);

/**
 * @brief 电流重构
 * @param pFOC Foc结构体指针
 */
void FOC_Current_Select(PFOC_t pFOC);

/**
 * @brief Clarke正变换
 * @param pFOC Foc结构体指针
 */
void FOC_Clarke_Transform(PFOC_t pFOC);

/**
 * @brief Park正变换
 * @param pFOC Foc结构体指针
 */
void FOC_Park_Transform(PFOC_t pFOC);


void FOC_Update_Angle(PFOC_t pFOC)
{
    pFOC->mAngle = Encoder_Get_Angle_El(pFOC->encoder);
    pFOC->angle = pFOC->mAngle * (float) pFOC->polePairs;
    pFOC->radian = pFOC->angle * ANGLE_TO_RADIN;
}

void FOC_Current_Select(PFOC_t pFOC)
{
    Sense_Update(pFOC->sense);
    pFOC->ia = pFOC->sense->i_a;
    pFOC->ib = pFOC->sense->i_b;
    pFOC->ic = pFOC->sense->i_c;

    if (pFOC->sense->ch_num < 3)
        return;
    //判断扇区索引
    //根据SVPWM波形发现每个扇区都有两相电流变化较小，一相电流变化较大
    //因此三电阻采样时,使用采集电流中变化较小的两相进行计算
    switch (SVPWM_Get_Sector(pFOC->svpwm))
    {
        case 1:
            pFOC->ia = 0.0f - pFOC->ib - pFOC->ic;
            break;
        case 2:
        case 3:
            pFOC->ib = 0.0f - pFOC->ia - pFOC->ic;
            break;
        case 4:
        case 5:
            pFOC->ic = 0.0f - pFOC->ia - pFOC->ib;
            break;
        case 6:
            pFOC->ia = 0.0f - pFOC->ib - pFOC->ic;
            break;
        default:
            break;
    }
}

void FOC_Clarke_Transform(PFOC_t pFOC)
{
    pFOC->Ialpha = pFOC->ia;
    pFOC->Ibeta = (pFOC->ia + 2.0f * pFOC->ib) / _SQRT3_2;
}

void FOC_Park_Transform(PFOC_t pFOC)
{
    pFOC->id = pFOC->Ialpha * fast_cos(pFOC->radian) + pFOC->Ibeta * fast_sin(pFOC->radian);
    pFOC->iq = -pFOC->Ibeta * fast_sin(pFOC->radian) + pFOC->Ibeta * fast_cos(pFOC->radian);
}

void FOC_Park_AntiTransform(PFOC_t pFOC)
{
    pFOC->Ialpha_SVPWM = pFOC->idPID.out * fast_cos(pFOC->radian) - pFOC->iqPID.out * fast_sin(pFOC->radian);
    pFOC->Ibeta_SVPWM = pFOC->idPID.out * fast_sin(pFOC->radian) + pFOC->iqPID.out * fast_cos(pFOC->radian);
}

void FOC_Control(PFOC_t pFOC)
{
    if (!pFOC->isEnable)
        return;
    // 1.获取电气角度
    FOC_Update_Angle(pFOC);
    // 2.计算实际电流值
    // 2.1电流重构
    FOC_Current_Select(pFOC);
    // 2.2Clarke变换
    FOC_Clarke_Transform(pFOC);
    // 2.3Park变换
    FOC_Park_Transform(pFOC);
    // 3.做PID闭环
    PID_Calc(&pFOC->iqPID, pFOC->iq, pFOC->tariq);
    PID_Calc(&pFOC->idPID, pFOC->id, pFOC->tarid);
    // 4.计算SVPWM输入值Ualpha,Ubeta
    FOC_Park_AntiTransform(pFOC);
    // 5.输出SVPWM
    SVPWM_Control(pFOC->svpwm, pFOC->Ialpha_SVPWM, pFOC->Ibeta_SVPWM);

}

void FOC_Set_CurrentTar(PFOC_t pFOC, float tarid, float tariq)
{
    pFOC->tarid = tarid;
    pFOC->tariq = tariq;
}

void FOC_Set_CurrentPID(PFOC_t pFOC, float kp, float ki, float kd, float outMax)
{
    pFOC->idPID.kp = kp;
    pFOC->idPID.ki = ki;
    pFOC->idPID.kd = kd;
    pFOC->idPID.outMax = outMax;

    pFOC->iqPID.kp = kp;
    pFOC->iqPID.ki = ki;
    pFOC->iqPID.kd = kd;
    pFOC->iqPID.outMax = outMax;
}

float FOC_Read_Angle(PFOC_t pFOC)
{
    return pFOC->mAngle;
}

void FOC_Init(PFOC_t pFOC, PSVPWM_t svpwm, Sense_t *sense,
              Driver3_t *driver, Encoder_t *encoder, uint8_t xpolePairs)
{
    pFOC->isEnable = 1;
    pFOC->polePairs = xpolePairs;

    pFOC->mAngle = 0;
    pFOC->angle = 0;
    pFOC->radian = 0;

    pFOC->ia = 0;
    pFOC->ib = 0;
    pFOC->ic = 0;

    pFOC->Ialpha = 0;
    pFOC->Ibeta = 0;

    pFOC->tariq = 0;
    pFOC->tarid = 0;
    pFOC->iq = 0;
    pFOC->id = 0;

    pFOC->Ialpha_SVPWM = 0;
    pFOC->Ibeta_SVPWM = 0;

    pFOC->driver = driver;
    pFOC->svpwm = svpwm;
    pFOC->encoder = encoder;
    pFOC->sense = sense;

    // 电流环统一使用增量式PID
    struct PID defPID = {0};
    defPID.type = DELTA_PID;

    pFOC->iqPID = defPID;
    pFOC->idPID = defPID;
}
