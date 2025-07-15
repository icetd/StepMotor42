#ifndef __INTERFACE_CAN_H
#define __INTERFACE_CAN_H

#include <can.h>

extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t TxData[8];
extern uint8_t RxData[8];
extern uint32_t TxMailbox;

void can_init(CAN_HandleTypeDef *hcan);
void can_send(CAN_TxHeaderTypeDef *pHeader, uint8_t *data);

#endif