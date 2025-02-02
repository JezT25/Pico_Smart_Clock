/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef GPIO_h
#define GPIO_h

#include "setup.hpp"

#define HIGH                1
#define LOW                 0

#define SEG_A               13
#define SEG_B               12
#define SEG_C               8
#define SEG_D               10
#define SEG_E               11
#define SEG_F               14
#define SEG_G               15
#define SEG_DOT             9

#define SEG_CTRL_1          6
#define SEG_CTRL_2          7
#define SEG_CTRL_3          2
#define SEG_CTRL_4          3

#define BUTTON_MODE         18
#define BUTTON_SELECT       17

#define BUZZER              16

#define LED_PARTITION_COUNT 4
#define LED_SEGMENT_COUNT   7
#define BUTTON_COUNT        2

class GPIO_class {
    protected:
        const uint8_t LED_Dot                           =   SEG_DOT;
        const uint8_t LED_Section[LED_PARTITION_COUNT]  = { SEG_CTRL_1, SEG_CTRL_2, SEG_CTRL_3, SEG_CTRL_4 };
        const uint8_t LED_Segment7[LED_SEGMENT_COUNT]   = { SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G };
        const uint8_t Buttons[BUTTON_COUNT]             = { BUTTON_MODE, BUTTON_SELECT };

    private:
        void I2C_Initialize();
        void LED_Initialize();
        void BUTTON_Initialize();
        void BUZZER_Initialize();

    public:
        void Initialize();
};

#endif
