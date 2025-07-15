#include <Signal/switchBase.h>

void SwitchBase::Tick(uint32_t _timeElapseMillis)
{
    timer += _timeElapseMillis;
    bool pinIO = ReadSwitchPinIO(Id);

    if (lasrPinIo != pinIO) {
        if (pinIO) {
            OnEventFunc(UP);
            if (timer - pressTime > longPressTime)
                OnEventFunc(LONG_PRESS);
            else
                OnEventFunc(CLICK);
        } else {
            OnEventFunc(DOWN);
            pressTime = timer;
        }

        lasrPinIo = pinIO;
    }
}

void SwitchBase::setOnEventListener(void (*_callback)(Event))
{
    lasrPinIo = ReadSwitchPinIO(Id);
    OnEventFunc = _callback;
}