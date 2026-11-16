/**
 * @file    digital_out.c
 * @brief   پیاده‌سازی کتابخانه کنترل خروجی‌ها
 *
 * فرض: ADC1 در حالت Scan با 12 کانال، DMA Circular با Half-Word
 * و Rank1..Rank12 به ترتیب مربوط به خروجی 1..12 هستند.
 */
#include "digital_out.h"
#include "main.h"

/* تنها آرایه: مقادیر خام ADC توسط DMA */
static volatile uint16_t adc_buf[DO_N_OUT];

/* آستانه خام معادل 1A: ولتاژ = 2.5 - 1 * 0.1 = 2.4V */
#define DO_THRESH_RAW  ((uint16_t)((DO_SENSOR_V0A - DO_THRESH_A * DO_SENSOR_V_PER_A) * 4095.0f / DO_ADC_VREF_V))

static ADC_HandleTypeDef *s_hadc = NULL;
static uint8_t  s_mode_last = 0xFF;
static uint8_t  s_cur = 0;
static uint32_t s_t_last = 0;

static GPIO_TypeDef * const out_port[DO_N_OUT] = {
  digital_out_1_GPIO_Port,  digital_out_2_GPIO_Port,  digital_out_3_GPIO_Port,
  digital_out_4_GPIO_Port,  digital_out_5_GPIO_Port,  digital_out_6_GPIO_Port,
  digital_out_7_GPIO_Port,  digital_out_8_GPIO_Port,  digital_out_9_GPIO_Port,
  digital_out_10_GPIO_Port, digital_out_11_GPIO_Port, digital_out_12_GPIO_Port
};

static const uint16_t out_pin[DO_N_OUT] = {
  digital_out_1_Pin,  digital_out_2_Pin,  digital_out_3_Pin,
  digital_out_4_Pin,  digital_out_5_Pin,  digital_out_6_Pin,
  digital_out_7_Pin,  digital_out_8_Pin,  digital_out_9_Pin,
  digital_out_10_Pin, digital_out_11_Pin, digital_out_12_Pin
};

static inline void out_set(uint8_t i, GPIO_PinState s)
{
  HAL_GPIO_WritePin(out_port[i], out_pin[i], s);
}

void DigitalOut_Init(ADC_HandleTypeDef *hadc)
{
  s_hadc = hadc;
  s_mode_last = 0xFF;
  s_cur = 0;
  s_t_last = HAL_GetTick();
  HAL_ADC_Start_DMA(s_hadc, (uint32_t *)adc_buf, DO_N_OUT);
}

void DigitalOut_Process(void)
{
  uint8_t mode = (HAL_GPIO_ReadPin(mode_GPIO_Port, mode_Pin) == GPIO_PIN_SET) ? 1U : 0U;

  /* mode=0 -> چرخش 1..6 و هشدار 7..12 ; mode=1 -> چرخش 7..12 و هشدار 1..6 */
  uint8_t main_base = mode ? DO_HALF : 0U;
  uint8_t aux_base  = mode ? 0U : DO_HALF;

  /* تغییر مود: همه خاموش و چرخه از اول */
  if (mode != s_mode_last) {
    for (uint8_t i = 0; i < DO_N_OUT; i++) {
      out_set(i, GPIO_PIN_RESET);
    }
    s_mode_last = mode;
    s_cur = 0;
    s_t_last = HAL_GetTick();
    out_set(main_base + s_cur, GPIO_PIN_SET);
  }

  /* سوییچ هر 3 ثانیه */
  if ((HAL_GetTick() - s_t_last) >= DO_SWITCH_MS) {
    s_t_last += DO_SWITCH_MS;
    out_set(main_base + s_cur, GPIO_PIN_RESET);
    s_cur = (s_cur + 1U) % DO_HALF;
    out_set(main_base + s_cur, GPIO_PIN_SET);
  }

  /* هشدار: فقط خروجی فعال بررسی می‌شود؛ خروجی متناظر در گروه دیگر */
  for (uint8_t i = 0; i < DO_HALF; i++) {
    GPIO_PinState s = GPIO_PIN_RESET;
    /* جریان‌سنج با افزایش جریان ولتاژ کم می‌شود: جریان بالاتر = مقدار خام کمتر */
    if ((i == s_cur) && (adc_buf[main_base + i] < DO_THRESH_RAW)) {
      s = GPIO_PIN_SET;
    }
    out_set(aux_base + i, s);
  }
}

uint16_t DigitalOut_GetRaw(uint8_t ch)
{
  return (ch < DO_N_OUT) ? adc_buf[ch] : 0U;
}

float DigitalOut_GetCurrentA(uint8_t ch)
{
  float v = (float)DigitalOut_GetRaw(ch) * DO_ADC_VREF_V / 4095.0f;
  return (DO_SENSOR_V0A - v) / DO_SENSOR_V_PER_A;
}
