/**
 * @file    digital_out.h
 * @brief   کتابخانه کنترل 12 خروجی دیجیتال با پایش جریان ADC (STM32F407VGT6)
 */
#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define DO_N_OUT          12U     /* تعداد کل خروجی‌ها */
#define DO_HALF           6U      /* تعداد خروجی در هر گروه */
#define DO_SWITCH_MS      3000U   /* بازه سوییچ خروجی‌ها (ms) */
#define DO_THRESH_A       1.0f    /* آستانه جریان هشدار (A) */
#define DO_ADC_VREF_V     3.3f    /* ولتاژ ورودی ADC (V) */
#define DO_SENSOR_V0A     2.5f    /* ولتاژ جریان‌سنج در 0A (V) */
#define DO_SENSOR_V_PER_A 0.1f    /* افت ولتاژ جریان‌سنج به ازای هر آمپر (V/A) */

/**
 * @brief  راه‌اندازی کتابخانه و شروع ADC با DMA
 * @param  hadc: هندل ADC1 (ساخته شده توسط CubeMX)
 */
void DigitalOut_Init(ADC_HandleTypeDef *hadc);

/**
 * @brief  فراخوانی در while(1) - منطق سوییچ و هشدار جریان
 */
void DigitalOut_Process(void);

/**
 * @brief  خواندن مقدار خام ADC یک کانال (0..11)
 */
uint16_t DigitalOut_GetRaw(uint8_t ch);

/**
 * @brief  خواندن جریان یک کانال بر حسب A
 */
float DigitalOut_GetCurrentA(uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_OUT_H */
