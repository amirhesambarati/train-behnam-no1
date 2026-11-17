#ifndef OUT14_H
#define OUT14_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define OUT14_COUNT         14U
#define OUT14_THRESHOLD     100U      /* آستانه مقدار خام ADC (12 بیت) */
#define OUT14_READ_MS       1000U     /* دوره خواندن ADC */
#define OUT14_PHASE2_MS     60000U    /* زمان شروع فاز دوم (1 دقیقه) */

void     Out14_Init(ADC_HandleTypeDef *hadc);   /* شروع DMA و روشن کردن خروجی 1..7 */
void     Out14_Process(void);                   /* در while(1) */
uint16_t Out14_GetRaw(uint8_t ch);              /* ch: 1..14 ، آخرین مقدار خوانده‌شده (هر 1 ثانیه) */

#ifdef __cplusplus
}
#endif
#endif
