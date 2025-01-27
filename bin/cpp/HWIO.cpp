/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

volatile bool HWIO_class::modeButton_ispressed 		= false;
volatile bool HWIO_class::selectButton_ispressed	= false;
volatile int HWIO_class::buzzer_duration			= BEEP_SHORT;
volatile uint8_t HWIO_class::button_flag			= NO_FLAG;
volatile uint32_t HWIO_class::modeButton_lpt		= 0;
volatile uint32_t HWIO_class::selectButton_lpt		= 0;
volatile uint32_t HWIO_class::buzzer_lpt 			= 0;

void HWIO_class::Initialize()
{
	gpio_set_irq_enabled_with_callback(BUTTON_MODE, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &button_Function);
	gpio_set_irq_enabled_with_callback(BUTTON_SELECT, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &button_Function);
}

// TODO: The buttons are a bit noisy. Maybe adjust them
void HWIO_class::button_Function(uint gpio, uint32_t events)
{
	uint32_t current_time = to_ms_since_boot(get_absolute_time());

	if (events & GPIO_IRQ_EDGE_FALL && !modeButton_ispressed && !selectButton_ispressed)
	{
		if (current_time - (gpio == BUTTON_MODE ? modeButton_lpt : selectButton_lpt) > DEBOUNCE_MS)
		{
			(gpio == BUTTON_MODE ? modeButton_lpt : selectButton_lpt) = current_time;
    		(gpio == BUTTON_MODE ? modeButton_ispressed : selectButton_ispressed) = true;
		}
	}
	else if (events & GPIO_IRQ_EDGE_RISE && (modeButton_ispressed ^ selectButton_ispressed))
	{
		float press_duration = (current_time - (gpio == BUTTON_MODE ? modeButton_lpt : selectButton_lpt));
		if(press_duration > BUTTON_MIN_PRESS)
		{
			(gpio == BUTTON_MODE ? modeButton_ispressed : selectButton_ispressed) = false;
			button_flag = (gpio == BUTTON_MODE ? MODE_BUTTON : SELECT_BUTTON);;
		}
	}
	else if (events & GPIO_IRQ_EDGE_FALL && (modeButton_ispressed ^ selectButton_ispressed))
	{
		modeButton_ispressed = true;
		selectButton_ispressed = true;
	}
	else if (events & GPIO_IRQ_EDGE_RISE && modeButton_ispressed && selectButton_ispressed)
	{
		float press_duration = current_time - fmax(selectButton_lpt, modeButton_lpt);
		if(press_duration > BUTTON_MIN_PRESS && press_duration < BUTTON_LONG_PRESS) button_flag = SHORT_COMBO_BUTTON;
		else if(press_duration >= BUTTON_LONG_PRESS) button_flag = COMBO_BUTTON;
		modeButton_ispressed = false;
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
