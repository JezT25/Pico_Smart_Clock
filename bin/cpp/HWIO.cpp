/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

volatile IDATA* HWIO_class::_IData					= nullptr;
volatile ISYSTEM* HWIO_class::_ISystem 				= nullptr;
volatile int HWIO_class::buzzer_duration			= BEEP_SHORT;
volatile bool HWIO_class::modeButton_ispressed 		= false;
volatile bool HWIO_class::selectButton_ispressed	= false;
volatile uint32_t HWIO_class::modeButton_lpt		= 0;
volatile uint32_t HWIO_class::selectButton_lpt		= 0;
volatile uint32_t HWIO_class::buzzer_lpt 			= 0;


void HWIO_class::Initialize(IDATA *IData, ISYSTEM *ISystem)
{
	_IData = IData;
	_ISystem = ISystem;
	gpio_set_irq_enabled_with_callback(BUTTON_MODE, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &button_Function);
	gpio_set_irq_enabled_with_callback(BUTTON_SELECT, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &button_Function);
}

void HWIO_class::button_Function(uint gpio, uint32_t events)
{
	uint32_t current_time = to_ms_since_boot(get_absolute_time());
	switch (gpio)
	{
		case BUTTON_MODE:
			modeFunction(current_time, events);
			break;
		case BUTTON_SELECT:
			selectFunction(current_time, events);
			break;
	}
}

void HWIO_class::modeFunction(uint32_t current_time, uint32_t events)
{
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
			playBuzzer(TONE_LOW, BEEP_SHORT);
			_ISystem->SYSTEM_MODE = (_ISystem->SYSTEM_MODE == MODE_COUNT - 1) ? _ISystem->CLOCK_MODE : ++_ISystem->SYSTEM_MODE;
		}
		else if(press_duration >= BUTTON_LONG_PRESS)
		{
			playBuzzer(TONE_LOW, BEEP_LONG);
		}

		modeButton_ispressed = false;
	}
}

void HWIO_class::selectFunction(uint32_t current_time, uint32_t events)
{
	if (events & GPIO_IRQ_EDGE_FALL && !selectButton_ispressed)
	{
		if (current_time - selectButton_lpt > DEBOUNCE_MS)
		{
			selectButton_lpt = current_time;
			selectButton_ispressed = true;
		}
	}
	else if (events & GPIO_IRQ_EDGE_RISE && selectButton_ispressed)
	{
		float press_duration = (current_time - selectButton_lpt);

		if(press_duration > BUTTON_MIN_PRESS && press_duration < BUTTON_LONG_PRESS)
		{
			playBuzzer(TONE_LOW, BEEP_SHORT);
			_ISystem->SYSTEM_MODE = (_ISystem->SYSTEM_MODE == 0) ? MODE_COUNT - 1 : --_ISystem->SYSTEM_MODE;
		}
		else if(press_duration >= BUTTON_LONG_PRESS)
		{
			playBuzzer(TONE_LOW, BEEP_LONG);
		}

		selectButton_ispressed = false;
	}
}

void HWIO_class::playBuzzer(int frequency, int duration)
{
	buzzer_duration = duration;
	buzzer_lpt = to_ms_since_boot(get_absolute_time());
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    uint32_t clock_divider = 4;
    pwm_set_clkdiv(slice_num, clock_divider);
    uint32_t wrap = 125000000 / (clock_divider * frequency);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER), wrap / 2);
    pwm_set_enabled(slice_num, true);
}

inline void HWIO_class::stopBuzzer()
{
	if (to_ms_since_boot(get_absolute_time()) - buzzer_lpt > buzzer_duration) pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER), false);
}
