#ifndef __SWITCH_H
#define __SWITCH_H

#include <Signal/switchBase.h>

class Switch : public SwitchBase
{
public:
    explicit Switch(uint8_t _id) :
        SwitchBase(_id)
    {}

    Switch(uint8_t _id, uint32_t _longPressTime) :
        SwitchBase(_id, _longPressTime)
    {}

    bool IsPressed();
    bool ReadSwitchPinIO(uint8_t _id) override;
};

#endif