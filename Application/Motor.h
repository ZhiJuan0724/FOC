#ifndef _MOTOR_H
#define _MOTOR_H

#include "Encoder.h"
#include "Driver.h"
#include "Sense.h"
#include "SVPWM.h"
#include "Foc.h"
#include "PID.h"
#include "Connectivity.h"
#include "Monitor.h"
#include "Utils.h"

typedef enum Motor_Control_Type
{
    UNKNOW_CONTROL,
    VELOCITY_OPEN_CONTROL = 1,
    POSITION_OPEN_CONTROL = 2,
    VELOCITY_CLOSE_CONTROL = 3,
    POSITION_CLOSE_CONTROL = 4,

}Motor_Control_Type_t;

typedef enum Motor_Target_Type
{
    UNKNOW_TARGET,
    VELOCITY_TARGET = 1,
    POSITION_TARGET = 2,
}Motor_Target_Type_t;

typedef struct Motor_Target
{
    enum Motor_Control_Type control_type;
    enum Motor_Target_Type target_type;
    float target_val;
}Motor_Target_t;

typedef struct Motor
{
    struct Encoder *encoder_handle;
    struct Driver3 *driver3_handle;
    struct Sense *sense_handle;
    struct FOC *foc;
    uint8_t polePairs;
    float voltage_limit;
    float velocity_limit;
    float voltage_supply;
    // 不能为指针 直接传值,改变外部不影响内部PID和目标
    struct Motor_Target target;
    uint8_t init;
    struct PID speed_pid;
    struct PID postion_pid;
    struct PID iq_pid;
    struct PID id_pid;
} Motor_t;



/**
 * 电机初始化
 * @param motor 电机指针
 * @param polePairs 电机的极对数
 * @param voltage_supply 驱动板供电电压
 * @brief 在初始化前需要初始化外设并绑定外设
 *        在初始化后可以设置限制电压和速度（可选）
 */
void Motor_Init(Motor_t *motor, uint8_t polePairs, float voltage_supply);


/**
 * 电机FOC初始化
 * @param motor 电机指针
 * @param foc FOC指针
 * @param svpwm SVPWM指针
 */
void Motor_FOC_Init(Motor_t *motor, FOC_t *foc, SVPWM_t *svpwm);

/**
 * 电机链接编码器
 * @param motor 电机指针
 * @param encoder 编码器指针
 * @brief 推荐在链接前先对外设初始化
 */
void Motor_Link_Encoder(Motor_t *motor, Encoder_t *encoder);

/**
 * 电机链接驱动器
 * @param motor 电机指针
 * @param driver3 驱动器指针
 * @brief 推荐在链接前先对外设初始化
 */
void Motor_Link_Driver3(Motor_t *motor, Driver3_t *driver3);

/**
 * 电机链接电流传感器
 * @param motor 电机指针
 * @param sense 传感器指针
 * @brief 推荐在链接前先对外设初始化
 */
void Motor_Link_Sense(Motor_t *motor, Sense_t *sense);

/**
 * 设置电机运行目标
 * @param motor 电机指针
 * @param target 目标指针
 */
void Motor_Set_Target(Motor_t *motor, Motor_Target_t target);

/**
 * 更新FOC电流PID参数
 * @param motor 电机指针
 */
void Motor_Update_FOC_PID(Motor_t *motor);

/**
 * 电机运行循环
 * @param motor 电机指针
 */
void Motor_Run(Motor_t *motor);
/**
 * 电机输出停止
 * @param motor 电机指针
 */
void Motor_Stop(Motor_t *motor);

/**
 * 闭环位置控制 可以被重写
 * @param pMotor 电机指针
 */
__weak void Motor_Position_Close_Control(Motor_t *pMotor);

/**
 * 闭环速度控制 可以被重写
 * @param pMotor 电机指针
 */
__weak void Motor_Velocity_Close_Control(Motor_t *pMotor);

/**
 * 开环位置控制 可以被重写
 * @param pMotor 电机指针
 */
__weak void Motor_Position_Open_Control(Motor_t *pMotor);

/**
 * 开环速度控制 可以被重写
 * @param pMotor 电机指针
 */
__weak void Motor_Velocity_Open_Control(Motor_t *pMotor);

#endif
