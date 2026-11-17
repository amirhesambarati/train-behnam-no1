/**
 * @file can_link.h
 * ارتباط CAN1/CAN2 بین برد فرستنده (14 خروجی) و گیرنده (12 خروجی)
 * نقش برد با تعریف CAN_LINK_ROLE_TX (فرستنده) یا CAN_LINK_ROLE_RX (گیرنده) در Project Settings/ main.h انتخاب می‌شود.
 */
#ifndef CAN_LINK_H
#define CAN_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>

#if !defined(CAN_LINK_ROLE_TX) && !defined(CAN_LINK_ROLE_RX)
#error "CAN_LINK_ROLE_TX یا CAN_LINK_ROLE_RX را تعریف کنید"
#endif

#define CAN_LINK_BYTES        12U
#define CAN_LINK_ID_A         0x100U   /* بایت 0..7  */
#define CAN_LINK_ID_B         0x101U   /* بایت 8..11 */
#define CAN_LINK_SEND_MS      1000U
#define CAN_LINK_READ_MS      1000U

/** شروع فیلتر، CAN1/CAN2 و فعال‌سازی اینتراپت‌ها (TX و RX) */
void CanLink_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2);

/** در while(1) صدا زده شود */
void CanLink_Process(void);

#ifdef CAN_LINK_ROLE_RX
/** فراخوانی هنگام دریافت هر بسته کامل (کاربر override می‌کند). data[i]: 0 خاموش، 1 روشن */
void CanLink_OnData(const uint8_t data[CAN_LINK_BYTES]);
#endif

#ifdef __cplusplus
}
#endif
#endif
