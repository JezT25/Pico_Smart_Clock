/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void GPIO_class::I2C_Initialize()
{
    i2c_init(i2c_default, 400 * 1000); 
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

void GPIO_class::LED_Initialize()
{
    // Setup Dot
    gpio_init(LED_Dot);
    gpio_set_dir(LED_Dot, GPIO_OUT);
    gpio_put(LED_Dot, HIGH);

    // Setup 7 Segments
    for (uint8_t i = 0; i < LED_SEGMENT_COUNT; i++)
    {
        gpio_init(LED_Segment7[i]);
        gpio_set_dir(LED_Segment7[i], GPIO_OUT);
        gpio_put(LED_Segment7[i], HIGH);
    }

    // Setup Quarter Segments
    for (uint8_t i = 0; i < LED_PARTITION_COUNT; i++)
    {
        gpio_init(LED_Section[i]);
        gpio_set_dir(LED_Section[i], GPIO_OUT);
        gpio_put(LED_Section[i], HIGH);
    }
}

void GPIO_class::BUTTON_Initialize()
{
    // Setup Buttons
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        gpio_init(Buttons[i]);
        gpio_set_dir(Buttons[i], GPIO_IN);
        gpio_pull_up(Buttons[i]);
    }
}

void GPIO_class::BUZZER_Initialize()
{
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
}

void GPIO_class::Initialize()
{
    I2C_Initialize();
    LED_Initialize();
    BUTTON_Initialize();
    BUZZER_Initialize();
}
