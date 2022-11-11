#ifndef _FOC_H
#define _FOC_H

#include "SVPWM.h"
#include "stdio.h"
#include "Utils.h"
#include "stdint.h"
#include "PID.h"
#include "Encoder.h"
#include "Sense.h"

/*
FOC结构体
*/
typedef struct FOC
{
    uint8_t isEnable;  //电机使能标志
    uint8_t polePairs; //极对数

    float radian; //弧度

    float ia; // a相实际电流
    float ib; // b相实际电流
    float ic; // c相实际电流

    float Ialpha; // clarke变换后 * 2/3后 Iα
    float Ibeta;  // clarke变换后 * 2/3后 Iβ

    float tariq; //目标电流iq
    float tarid; //目标电流id
    float iq;    // park变换后 iq
    float id;    // park变换后 id

    float Ialpha_SVPWM; // park反变换后 Iα
    float Ibeta_SVPWM;  // park反变换后 Iβ

    struct Encoder *encoder;
    struct SVPWM *svpwm;
    struct Sense *sense;
    struct Driver3 *driver;

    struct PID idPID;
    struct PID iqPID;

} FOC_t;

typedef FOC_t *PFOC_t;

/**
 * @brief FOC对象初始化
 * @param xpolePairs 极对数
 * @param xiNum 电流采集相数(主要用来判断是否为三相电流)
 * @param xUpdatePreCurrent 获取三相电流值
 * @param pFOC Foc结构体指针
 */
void FOC_Init(PFOC_t pFOC, struct SVPWM *svpwm, struct Sense *sense,
              struct Driver3 *driver, struct Encoder *encoder, uint8_t xpolePairs);

/**
 * @brief FOC控制 **核心函数**
 * @param pFOC Foc结构体指针
 */
void FOC_Control(PFOC_t pFOC);

/**
 * @brief 设置D轴和Q轴的目标值
 * @param tarid：D轴目标电流
 * @param tariq：Q轴目标电流
 * @param pFOC Foc结构体指针
 */
void FOC_Set_CurrentTar(PFOC_t pFOC, float tarid, float tariq);

/**
 * 设置电流环参数
 * @param pFOC Foc结构体指针
 * @param id_pid Iq PID
 * @param iq_pid Id PID
 */
void FOC_Set_CurrentPID(PFOC_t pFOC, PID_t id_pid, PID_t iq_pid);


/**
 * @brief Park反变换
 * @param pFOC Foc结构体指针
 */
void FOC_Park_AntiTransform(PFOC_t pFOC);

#endif
