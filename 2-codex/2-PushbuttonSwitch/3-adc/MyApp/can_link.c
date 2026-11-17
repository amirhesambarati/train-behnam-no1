#include "can_link.h"
#include "main.h"
#include <string.h>

static CAN_HandleTypeDef *s_can1, *s_can2;

static void filter_all(CAN_HandleTypeDef *h, uint32_t bank)
{
  CAN_FilterTypeDef f = {0};
  f.FilterBank = bank;
  f.FilterMode = CAN_FILTERMODE_IDMASK;
  f.FilterScale = CAN_FILTERSCALE_32BIT;
  f.FilterIdHigh = 0; f.FilterIdLow = 0;
  f.FilterMaskIdHigh = 0; f.FilterMaskIdLow = 0;
  f.FilterFIFOAssignment = CAN_RX_FIFO0;
  f.FilterActivation = ENABLE;
  f.SlaveStartFilterBank = 14;   /* بانک 0..13 برای CAN1 ، 14..27 برای CAN2 */
  HAL_CAN_ConfigFilter(h, &f);
}

void CanLink_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2)
{
  s_can1 = hcan1; s_can2 = hcan2;
  filter_all(s_can1, 0);
  filter_all(s_can2, 14);
  HAL_CAN_Start(s_can1);
  HAL_CAN_Start(s_can2);
  uint32_t it = CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY;
  HAL_CAN_ActivateNotification(s_can1, it);
  HAL_CAN_ActivateNotification(s_can2, it);
}

/* ---------------- دریافت (گیرنده) ---------------- */
static volatile uint8_t rx_data[CAN_LINK_BYTES];
static volatile uint8_t rx_gotA, rx_gotB;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef h;
  uint8_t d[8];
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &h, d) != HAL_OK) return;
  if (h.IDE != CAN_ID_STD) return;
  if (h.StdId == CAN_LINK_ID_A && h.DLC == 8) {
    for (int i = 0; i < 8; i++) rx_data[i] = d[i] ? 1U : 0U;
    rx_gotA = 1;
  } else if (h.StdId == CAN_LINK_ID_B && h.DLC == 4) {
    for (int i = 0; i < 4; i++) rx_data[8 + i] = d[i] ? 1U : 0U;
    rx_gotB = 1;
  }
}

/* ---------------- ارسال (اینتراپت TX) ---------------- */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *h){ (void)h; }
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *h){ (void)h; }
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *h){ (void)h; }

#ifdef CAN_LINK_ROLE_TX
/* الگوی ارسال: هر 10 ثانیه یک بایت (به ترتیب 0..11) برابر 1 و بقیه 0 */
static uint8_t tx_data[CAN_LINK_BYTES];
static uint8_t tx_index;          /* بایتی که این دفعه یک می‌شود */
static uint32_t t_send;
static uint8_t use_can2;          /* 0: CAN1 ، 1: CAN2 (به نوبت) */

static void send_frames(CAN_HandleTypeDef *h)
{
  CAN_TxHeaderTypeDef t = {0};
  uint32_t mb;
  t.IDE = CAN_ID_STD; t.RTR = CAN_RTR_DATA; t.TransmitGlobalTime = DISABLE;

  t.StdId = CAN_LINK_ID_A; t.DLC = 8;
  HAL_CAN_AddTxMessage(h, &t, &tx_data[0], &mb);
  t.StdId = CAN_LINK_ID_B; t.DLC = 4;
  HAL_CAN_AddTxMessage(h, &t, &tx_data[8], &mb);
}

void CanLink_Process(void)
{
  uint32_t now = HAL_GetTick();

  if (now - t_send >= CAN_LINK_SEND_MS) {
    t_send = now;

    memset(tx_data, 0, sizeof(tx_data));
    tx_data[tx_index] = 1U;                       /* فقط یک بایت یک است */
    tx_index = (uint8_t)((tx_index + 1U) % CAN_LINK_BYTES);

    send_frames(use_can2 ? s_can2 : s_can1);
    use_can2 ^= 1U;
  }
}
#else /* RX */
__weak void CanLink_OnData(const uint8_t data[CAN_LINK_BYTES]) { (void)data; }

void CanLink_Process(void)
{
  if (rx_gotA && rx_gotB) {
    uint8_t c[CAN_LINK_BYTES];
    __disable_irq();
    memcpy(c, (const void *)rx_data, CAN_LINK_BYTES);
    rx_gotA = rx_gotB = 0;
    __enable_irq();
    CanLink_OnData(c);
  }
}
#endif
