/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef LED_h
#define LED_h

#include "../setup.hpp"

#define SEG_1           0
#define SEG_2           1
#define SEG_3           2
#define SEG_4           3

#define SEG_COUNT       4
#define LED_CHAR_COUNT  14

class LED_class : private HARDWARE_class {
    private:
        volatile uint8_t currentSegment                         = SEG_1;
        volatile uint8_t segmentBuffer[MOUNT_COUNT][SEG_COUNT]  =
        {
            {  0, 0, 0, 0  },  // Clock Time
            {  0, 0, 0, 0  },  // Date
            {  2, 0, 0, 0  },  // Year
            {  0, 0, 0, 12 },  // Temperature
            {  0, 0, 0, 13 },  // Humidity
            {  0, 0, 0, 13 }   // Pressure
        };
        const uint8_t LED_digitValues[LED_CHAR_COUNT]           =
        {
            0b1000000, // 0
            0b1111001, // 1
            0b0100100, // 2
            0b0110000, // 3
            0b0011001, // 4
            0b0010010, // 5
            0b0000010, // 6
            0b1111000, // 7
            0b0000000, // 8
            0b0010000, // 9
            0b1111111, // All Off   -   10
            0b0111111, // -         -   11
            0b0011100, // °         -   12
            0b0001001  // H         -   13
        };

    public:
        void displayDigits(ISYSTEM ISystem);
        void updateBuffer(IDATA IData);
};

#endif
