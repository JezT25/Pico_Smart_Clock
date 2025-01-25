/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef HARDWARE_h
#define HARDWARE_h

#include "../setup.hpp"

#define HIGH                1
#define LOW                 0

#define SEG_A               15
#define SEG_B               13
#define SEG_C               9
#define SEG_D               11
#define SEG_E               12
#define SEG_F               14
#define SEG_G               8
#define SEG_DOT             10

#define SEG_CTRL_1          16
#define SEG_CTRL_2          17
#define SEG_CTRL_3          18
#define SEG_CTRL_4          19

#define BUTTON_MODE         20
#define BUTTON_SELECT       21

#define BUZZER              7

#define LED_PARTITION_COUNT 4
#define LED_SEGMENT_COUNT   7
#define BUTTON_COUNT        2

class HARDWARE_class {
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
