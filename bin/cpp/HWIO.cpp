/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

IDATA* HWIO_class::_IData				= nullptr;
ISYSTEM* HWIO_class::_ISystem 			= nullptr;
bool HWIO_class::modeButton_ispressed 	= false;
uint32_t HWIO_class::modeButton_lpt		= 0;

void HWIO_class::Initialize(IDATA *IData, ISYSTEM *ISystem)
{
	_IData = IData;
	_ISystem = ISystem;
	gpio_set_irq_enabled_with_callback(BUTTON_MODE, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &modeButton_Function);
}

void HWIO_class::modeButton_Function(uint gpio, uint32_t events)
{
	uint32_t current_time = to_ms_since_boot(get_absolute_time());

	if (events & GPIO_IRQ_EDGE_FALL && !modeButton_ispressed)
	{
		if (current_time - modeButton_lpt > DEBOUNCE_MS)
		{
			modeButton_lpt = current_time;
			modeButton_ispressed = true;
		}
	}
	else if (events & GPIO_IRQ_EDGE_RISE && modeButton_ispressed)
	{
		float press_duration = (current_time - modeButton_lpt);

		if(press_duration > BUTTON_MIN_PRESS && press_duration < BUTTON_LONG_PRESS)
		{
			_ISystem->SYSTEM_MODE = (_ISystem->SYSTEM_MODE == MODE_COUNT - 1) ? _ISystem->CLOCK_MODE : ++_ISystem->SYSTEM_MODE;
		}
		else if(press_duration >= BUTTON_LONG_PRESS)
		{

		}

		modeButton_ispressed = false;
	}
}
