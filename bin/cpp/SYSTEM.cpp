/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void SYSTEM_class::Initialize()
{
    // Initialize Pico Hardware
    _HARDWARE.Initialize();

    // Initialize External Hardware IO
    _HWIO.Initialize(&_ISystem);

    // Initialize Clock Interrupts
    add_alarm_in_ms(ISR_START_ON_MS, DisplayISR, this, false);
    add_alarm_in_ms(ISR_START_ON_MS, ClockISR, this, false);
}

void SYSTEM_class::Run()
{
    _LED.updateBuffer(_IData);
}

long long int SYSTEM_class::DisplayISR(alarm_id_t id, void* user_data)
{
    SYSTEM_class* system = static_cast<SYSTEM_class*>(user_data);
    system->_LED.displayDigits(system->_ISystem);
    return DISPLAY_REFRESH_US;
}

long long int SYSTEM_class::ClockISR(alarm_id_t id, void* user_data)
{
    SYSTEM_class* system = static_cast<SYSTEM_class*>(user_data);
    system->_TIME.getTime(&system->_IData);
    return TIME_REFRESH_US;
}
