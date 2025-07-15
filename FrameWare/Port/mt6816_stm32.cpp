#include "mt6816_stm32.h"
#include "spi.h"

void MT6816::SpiInit()
{
    MX_SPI2_Init();
}

uint16_t MT6816::SpiTransmitAndRead16Bits(uint16_t _dataTx)
{
    uint16_t dataRx;

    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PinState::GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&_dataTx, (uint8_t *)&dataRx, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PinState::GPIO_PIN_SET);

    return dataRx;
}
