#ifndef __SWITCH_BASE_H
#define __SWITCH_BASE_H

#include <stdint-gcc.h>

class SwitchBase
{
public:
    enum Event
    {
        UP = 0,
        DOWN,
        LONG_PRESS,
        CLICK
    };

    explicit SwitchBase(uint8_t _id) :
        Id(_id)
    {}

    SwitchBase(uint8_t _id, uint32_t _longPressTime) :
        Id(_id),
        longPressTime(_longPressTime)
    {}

    void Tick(uint32_t _timeElapseMillis);
    void setOnEventListener(void (*_callback)(Event));

protected:
    uint8_t Id;
    bool lasrPinIo{};
    uint32_t timer = 0;
    uint32_t pressTime{};
    uint32_t longPressTime = 2000;

    void (*OnEventFunc)(Event){};

    virtual bool ReadSwitchPinIO(uint8_t _id) = 0;
};

#endif