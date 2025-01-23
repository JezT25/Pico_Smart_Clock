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

#define BUTTON_MODE         21
#define BUTTON_UP           21
#define BUTTON_DOWN         21

#define LED_PARTITION_COUNT 4
#define LED_SEGMENT_COUNT   7
#define BUTTON_COUNT        1   // Set to 1 since I only have 1 installed for now

class HARDWARE_class {
    protected:
        const uint8_t LED_Dot           =   SEG_DOT;
        const uint8_t LED_Section[4]    = { SEG_CTRL_1, SEG_CTRL_2, SEG_CTRL_3, SEG_CTRL_4 };
        const uint8_t LED_Segment7[7]   = { SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G };
        const uint8_t Buttons[3]        = { BUTTON_MODE, BUTTON_UP, BUTTON_DOWN };

    private:
        void I2C_Initialize();
        void LED_Initialize();
        void BUTTON_Initialize();

    public:
        void Initialize();
};

#endif
