/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

ISYSTEM* HWIO_class::_ISystem = nullptr;
uint32_t HWIO_class::modeButton_lpt = 0;

void HWIO_class::Initialize(ISYSTEM *isystem)
{
	_ISystem = isystem;
	gpio_set_irq_enabled_with_callback(BUTTON_MODE, GPIO_IRQ_EDGE_FALL, true, &modeButton_Function);
}

void HWIO_class::modeButton_Function(uint gpio, uint32_t events)
{
	uint32_t current_time = to_ms_since_boot(get_absolute_time());
	if (current_time - modeButton_lpt > DEBOUNCE_MS) {
		_ISystem->SYSTEM_MODE = (_ISystem->SYSTEM_MODE == MOUNT_COUNT - 1) ? CLOCK_MODE : _ISystem->SYSTEM_MODE + 1;
        modeButton_lpt = current_time;
    }
}
