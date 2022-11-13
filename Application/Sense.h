#ifndef _SENSE_H
#define _SENSE_H

#include "stm32f1xx_hal.h"

// DMA缓存
extern uint32_t __pADC_Data[3];

typedef void(*Sense_ConvCpltCallback)(struct __ADC_HandleTypeDef *hadc);

typedef struct Sense
{
    ADC_HandleTypeDef *adc_handle;
    TIM_HandleTypeDef *adc_trggier_handle;
    uint32_t trggier_channel;
    float v_ref;
    uint32_t ch_a;
    uint32_t ch_b;
    uint32_t ch_c;
    uint16_t ch_num;
    float gain_a;
    float gain_b;
    float gain_c;
    float offset_a;
    float offset_b;
    float offset_c;
    float val_a;
    float val_b;
    float val_c;
    float i_a;
    float i_b;
    float i_c;
    uint8_t swift;
    uint8_t skip_offset;
    uint8_t state;
    uint8_t init;
} Sense_t;

void Sense_Init(Sense_t *sense, ADC_HandleTypeDef *adc_handle, TIM_HandleTypeDef *adc_trggier_handle,
                uint32_t trggier_channel, float vref);

void Sense_Set_3Ch(Sense_t *sense, uint32_t ch_a, uint32_t ch_b, uint32_t ch_c, float gain_a, float gain_b, float gain_c);
void Sense_Set_2Ch(Sense_t *sense, uint32_t ch_a, uint32_t ch_b, float gain_a, float gain_b);
void Sense_Start(Sense_t *sense);
void Sense_Stop(Sense_t *sense);
void Sense_Update(Sense_t *sense);
void Sense_Config(Sense_t *sense, uint32_t pluse);
void Sense_Calibrate_Offsets(Sense_t *sense);

#endif
