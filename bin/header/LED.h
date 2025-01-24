/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef LED_h
#define LED_h

#include "../setup.hpp"

#define SEG_COUNT       4

#define ON              0
#define OFF             1
#define EN_BLANK        true
#define DISABLE_BLANK   false

class LED_class : private HARDWARE_class {
    private:
        enum Segments : uint8_t 
        {
            SEG_1,
            SEG_2,
            SEG_3,
            SEG_4
        };
        enum LEDDigit : uint8_t {
            ZERO,           // 0
            ONE,            // 1
            TWO,            // 2
            THREE,          // 3
            FOUR,           // 4
            FIVE,           // 5
            SIX,            // 6
            SEVEN,          // 7
            EIGHT,          // 8
            NINE,           // 9
            DIGIT_OFF,      // 10
            DASH,           // 11
            DEGREE,         // 12
            H,              // 13
            P,              // 14
            LED_CHAR_COUNT  // Total count
        };

        volatile bool dotState                                  = ON;
        volatile uint8_t currentSegment                         = SEG_1;
        volatile uint8_t segmentBuffer[MODE_COUNT][SEG_COUNT]   =
        {
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Clock Time
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Date
            {  TWO,         ZERO,      ZERO,      ZERO    },  // Year
            {  DIGIT_OFF,   ZERO,      ZERO,      DEGREE  },  // Temperature
            {  ZERO,        ZERO,      ZERO,      H       },  // Humidity
            {  DIGIT_OFF,   ZERO,      ZERO,      P       }   // Pressure
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
            0b1111111, // Digit Off
            0b0111111, // -
            0b0011100, // °
            0b0001001, // H
            0b0001100  // P
        };

        inline uint8_t getDecimal(double number);
        inline uint8_t getOnes(uint8_t number);
        inline uint8_t getTens(uint8_t number, bool blank = DISABLE_BLANK);
        inline uint8_t getHundred(uint8_t number, bool blank = DISABLE_BLANK);

    public:
        void toggleDot();
        void toggleDot(ISYSTEM ISystem);
        void displayDigits(ISYSTEM ISystem);
        void updateBuffer(IDATA IData, ISYSTEM ISystem);
};

#endif
