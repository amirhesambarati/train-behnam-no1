#include "out14.h"
#include "main.h"

static GPIO_TypeDef * const k_port[OUT14_COUNT] = {
  digital_out_1_GPIO_Port, digital_out_2_GPIO_Port, digital_out_3_GPIO_Port,
  digital_out_4_GPIO_Port, digital_out_5_GPIO_Port, digital_out_6_GPIO_Port,
  digital_out_7_GPIO_Port, digital_out_8_GPIO_Port, digital_out_9_GPIO_Port,
  digital_out_10_GPIO_Port, digital_out_11_GPIO_Port, digital_out_12_GPIO_Port,
  };
static const uint16_t k_pin[OUT14_COUNT] = {
  digital_out_1_Pin, digital_out_2_Pin, digital_out_3_Pin, digital_out_4_Pin,
  digital_out_5_Pin, digital_out_6_Pin, digital_out_7_Pin, digital_out_8_Pin,
  digital_out_9_Pin, digital_out_10_Pin, digital_out_11_Pin, digital_out_12_Pin,
   };

static uint16_t adc_buf[OUT14_COUNT];      /* پر شدن با DMA */
static uint16_t adc_val[OUT14_COUNT];      /* نمونه‌ی ثبت‌شده هر 1 ثانیه */
static uint32_t t_start, t_read;

static void out(uint8_t idx, uint8_t on)
{
  HAL_GPIO_WritePin(k_port[idx], k_pin[idx], on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Out14_Init(ADC_HandleTypeDef *hadc)
{
  for (uint8_t i = 0; i < OUT14_COUNT; i++) out(i, i < 7);   /* 1..7 روشن */
  HAL_ADC_Start_DMA(hadc, (uint32_t *)adc_buf, OUT14_COUNT);
  t_start = t_read = HAL_GetTick();
}

uint16_t Out14_GetRaw(uint8_t ch)
{
  return (ch >= 1 && ch <= OUT14_COUNT) ? adc_val[ch - 1] : 0;
}

void Out14_Process(void)
{
  uint32_t now = HAL_GetTick();
  if (now - t_read < OUT14_READ_MS) return;
  t_read = now;

  for (uint8_t i = 0; i < OUT14_COUNT; i++) adc_val[i] = adc_buf[i];

  if (now - t_start < OUT14_PHASE2_MS) {
    /* فاز 1: خروجی 1..7 روشن؛ خروجی 8..14 تابع جریان 1..7 */
    for (uint8_t i = 0; i < 7; i++) {
      out(i, 1);
      out(i + 7, adc_val[i] > OUT14_THRESHOLD);
    }
  } else {
    /* فاز 2: خروجی 8..14 همیشه روشن؛ با ADC 8..14 > 100 خروجی متناظر (1..7) روشن، وگرنه خاموش */
    for (uint8_t i = 0; i < 7; i++) {
      out(i + 7, 1);
      out(i, adc_val[i + 7] > OUT14_THRESHOLD);
    }
  }
}
