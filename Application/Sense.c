#include "Sense.h"

uint32_t __pADC_Data[3] = {0};

void Sense_Init(Sense_t *sense, ADC_HandleTypeDef *adc_handle, TIM_HandleTypeDef *adc_trggier_handle,
                uint32_t trggier_channel, float vref)
{
    sense->adc_handle = adc_handle;
    sense->adc_trggier_handle = adc_trggier_handle;
    sense->trggier_channel = trggier_channel;
    sense->v_ref = vref;
    sense->ch_num = 0;
    // uint32_t ch_a;
    // uint32_t ch_b;
    // uint32_t ch_c;
    sense->gain_a = 0;
    sense->gain_b = 0;
    sense->gain_c = 0;
    sense->offset_a = 0;
    sense->offset_b = 0;
    sense->offset_c = 0;
    sense->val_a = 0;
    sense->val_b = 0;
    sense->val_c = 0;
    sense->i_a = 0;
    sense->i_b = 0;
    sense->i_c = 0;
    sense->skip_offset = 0;
    sense->init = 1;
    sense->state = 0;
    sense->swift = 0;
}

void Sense_Set_2Ch(Sense_t *sense, uint32_t ch_a, uint32_t ch_b, float gain_a, float gain_b)
{
    sense->ch_a = ch_a;
    sense->ch_b = ch_b;
    sense->gain_a = gain_a;
    sense->gain_b = gain_b;
    sense->ch_num = 2;
}

void Sense_Set_3Ch(Sense_t *sense, uint32_t ch_a, uint32_t ch_b, uint32_t ch_c, float gain_a, float gain_b, float gain_c)
{
    Sense_Set_2Ch(sense, ch_a, ch_b, gain_a, gain_b);
    sense->ch_c = ch_c;
    sense->gain_c = gain_c;
    sense->ch_num = 3;
}

void Sense_Start(Sense_t *sense)
{
    sense->state = 1;
    HAL_TIM_PWM_Start(sense->adc_trggier_handle, sense->trggier_channel);
    HAL_ADC_Start_DMA(sense->adc_handle, __pADC_Data, sense->ch_num);
}

void Sense_Stop(Sense_t *sense)
{
    sense->state = 0;
    HAL_TIM_PWM_Stop(sense->adc_trggier_handle, sense->trggier_channel);
    HAL_ADC_Stop_DMA(sense->adc_handle);
}

void Sense_Config(Sense_t *sense,uint32_t pluse)
{
    __HAL_TIM_SetCompare(sense->adc_trggier_handle, sense->trggier_channel, pluse - 1);
}

void Sense_Calibrate_Offsets(Sense_t *sense)
{
    if (sense->state == 1)
        return;

    if (sense->skip_offset == 1)
    {
        sense->offset_a = sense->v_ref / 2;
        sense->offset_b = sense->v_ref / 2;
        sense->offset_c = sense->v_ref / 2;
    }
    else
    {
        HAL_ADC_Start_DMA(sense->adc_handle, __pADC_Data, sense->ch_num);
        int32_t nums = 1000;
        float sum_a = 0;
        float sum_b = 0;
        float sum_c = 0;
        while (nums > 0)
        {
            Sense_Update(sense);
            sum_a += sense->val_a;
            sum_b += sense->val_b;
            sum_c += sense->val_c;
            nums--;
        }
        HAL_ADC_Stop_DMA(sense->adc_handle);
        __pADC_Data[0] = __pADC_Data[1] = __pADC_Data[2] = 0;
        sense->offset_a = sum_a / 1000.0f;
        sense->offset_b = sum_b / 1000.0f;
        sense->offset_c = sum_c / 1000.0f;
    }
}

void Sense_Update(Sense_t *sense)
{
    sense->val_a = (float)__pADC_Data[0] / 4096.0f * sense->v_ref;
    sense->val_b = (float)__pADC_Data[1] / 4096.0f * sense->v_ref;
    sense->val_c = (float)__pADC_Data[2] / 4096.0f * sense->v_ref;

    if (sense->swift)
    {
        sense->i_a = (sense->val_a - sense->offset_a) / sense->gain_a * 1000.0f;
        sense->i_b = (sense->val_b - sense->offset_b) / sense->gain_b * 1000.0f;
        if (sense->ch_num == 2)
        {
            sense->i_c = 0.0f - sense->i_a - sense->i_b;
        }
        else
        {
            sense->i_c = (sense->val_c - sense->offset_c) / sense->gain_c * 1000.0f;
        }

        sense->swift = 0;
    }
}