#include <switch.h>
#include <gpio.h>

bool Switch::ReadSwitchPinIO(uint8_t _id)
{
    switch (_id) {
    case 1:
        return HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PinState::GPIO_PIN_RESET; // set low level is DOWN
        break;
    
    case 2:
        return HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PinState::GPIO_PIN_RESET; // set low level is DOWN
        break;
    
    case 3:
        return HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PinState::GPIO_PIN_RESET; // set low level is DOWN
        break;
    
    case 4:
        return HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == GPIO_PinState::GPIO_PIN_RESET; // set low level is DOWN
        break;
    
    case 5:
        return HAL_GPIO_ReadPin(LA1_GPIO_Port, LA1_Pin) == GPIO_PinState::GPIO_PIN_SET; // set low level is UP
        break;
    default:
        return false;
    }
}
bool Switch::IsPressed()
{
    return !ReadSwitchPinIO(Id);
}
