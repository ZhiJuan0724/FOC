#include "Motor.h"

/**
 * 电机外设初始化完成检测
 * @param motor 电机结构体指针
 * @return 所有外设是否初始化
 */
uint8_t __Motor_HardWare_Init_Check(Motor_t *motor);


void Motor_Velocity_Control(Motor_t *pMotor);

void Motor_Velocity_Open_Control(Motor_t *pMotor);

void Motor_Position_Open_Control(Motor_t *pMotor);

void Motor_Velocity_Close_Control(Motor_t *pMotor);

void Motor_Position_Close_Control(Motor_t *pMotor);

void Motor_Init(Motor_t *motor, uint8_t polePairs, float voltage_supply)
{
    motor->polePairs = polePairs;
    motor->voltage_supply = voltage_supply;
    motor->voltage_limit = -1.0f;
    motor->velocity_limit = -1.0f;
    motor->init = 0;

    if (__Motor_HardWare_Init_Check(motor) == 1)
        motor->init = 1;
}

void Motor_FOC_Init(Motor_t *motor, FOC_t *foc, SVPWM_t *svpwm)
{
    if (motor->init == 0)
        return;

    uint32_t ts = 1e6 / motor->driver3_handle->freq;
    SVPWM_Init(svpwm, motor->driver3_handle, ts, motor->velocity_limit);
    FOC_Init(foc, svpwm, motor->sense_handle, motor->driver3_handle,
             motor->encoder_handle, motor->polePairs);
    motor->foc = foc;
}

void Motor_Link_Encoder(Motor_t *motor, Encoder_t *encoder)
{
    motor->encoder_handle = encoder;
}

void Motor_Link_Driver3(Motor_t *motor, Driver3_t *driver3)
{
    motor->driver3_handle = driver3;
}

void Motor_Link_Sense(Motor_t *motor, Sense_t *sense)
{
    motor->sense_handle = sense;
}

uint8_t __Motor_HardWare_Init_Check(Motor_t *motor)
{
    uint8_t check = 1;

    if (motor->voltage_limit > motor->voltage_supply)
        motor->voltage_limit = motor->voltage_supply;

    if (motor->sense_handle->init == 0)
        check = 0;

    if (motor->encoder_handle->init == 0)
        check = 0;

    if (motor->driver3_handle->init == 0)
        check = 0;

    return check;
}

void Motor_Set_Target(Motor_t *motor, Motor_Target_t target)
{
    motor->target = target;
}

void Motor_Run(Motor_t *motor)
{
    switch (motor->target.control_type)
    {
        case VELOCITY_OPEN_CONTROL:
            Motor_Velocity_Open_Control(motor);
            break;
        case POSITION_OPEN_CONTROL:
            Motor_Position_Open_Control(motor);
            break;
        case VELOCITY_CLOSE_CONTROL:
            Motor_Velocity_Close_Control(motor);
            break;
        case POSITION_CLOSE_CONTROL:
            Motor_Position_Close_Control(motor);
            break;
        case UNKNOW_CONTROL:
        default:
            return;
    }
}

__weak void Motor_Position_Close_Control(Motor_t *pMotor)
{

}

__weak void Motor_Velocity_Close_Control(Motor_t *pMotor)
{

}

__weak void Motor_Position_Open_Control(Motor_t *pMotor)
{

}

__weak void Motor_Velocity_Open_Control(Motor_t *pMotor)
{

}


void Motor_Stop(Motor_t *motor)
{
    Driver3_Stop(motor->driver3_handle);
}