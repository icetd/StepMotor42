#ifndef __COMMON_H
#define __COMMON_H

#include <stdint-gcc.h>

#ifdef __cplusplus
extern "C" {
#endif


void Main();
void OnCanCmd(uint8_t _cmd, uint8_t *_data, uint32_t _len);

/* Event Callbacks -----------------------------------------------------------*/
void Tim1Callback100Hz();
void Tim4Callback20kHz();

#ifdef __cplusplus

#include <stdio.h>
#include "Motor/motor.h"
#include "Commuication/interface_can.h"
#include "mt6816_stm32.h"
#include "tb67h450_stm32.h"
#include "encoder_calibrator_stm32.h"
#include "switch.h"
#include <Platform/retarget.h>
#include <usart.h>

}
#endif

#endif