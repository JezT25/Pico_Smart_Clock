/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef LED_h
#define LED_h

#include "../setup.hpp"

#define ON              0
#define ON_SEGMENT      1
#define OFF             1
#define OFF_SEGMENT     0
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
        enum LED_Digit : uint8_t
        {
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
            A,              // 13
            F,              // 14
            H,              // 15
            P,              // 16
            n,              // 17
            o,              // 18
            LED_CHAR_COUNT  // Total count
        };

        volatile bool alarmState        = ALARM_OFF;
        volatile bool dotState          = ON;
        volatile bool sectionState      = ON_SEGMENT;
        volatile uint8_t currentSegment = SEG_1;
        const uint8_t LED_digitValues[LED_CHAR_COUNT] =
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
            0b0001000, // A
            0b0001110, // F
            0b0001001, // H
            0b0001100, // P
            0b0101011, // n
            0b0100011  // o
        };
        volatile uint8_t segmentBuffer[ALL_MODE_COUNT][LED_PARTITION_COUNT] =
        {
            // Normal Modes
            {  A,           DASH,      o,         F       },  // Alarm
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Clock Time
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Date
            {  TWO,         ZERO,      ZERO,      ZERO    },  // Year
            {  DIGIT_OFF,   ZERO,      ZERO,      DEGREE  },  // Temperature
            {  ZERO,        ZERO,      ZERO,      H       },  // Humidity
            {  DIGIT_OFF,   ZERO,      ZERO,      P       },  // Pressure

            // Special Modes
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Alarm Adjust
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Clock Adjust
            {  ZERO,        ZERO,      ZERO,      ZERO    },  // Date Adjust
            {  TWO,         ZERO,      ZERO,      ZERO    }   // Year Adjust
        };

        inline uint8_t getDecimal(double number);
        inline uint8_t getOnes(uint8_t number);
        inline uint8_t getTens(uint8_t number, bool blank = DISABLE_BLANK);
        inline uint8_t getHundred(uint8_t number, bool blank = DISABLE_BLANK);

    public:
        enum BLINK : uint8_t{
            NO_BLINK,
            RIGHT_BLINK,
            LEFT_BLINK,
            ALL_BLINK
        };
    
        volatile uint8_t currentSection_blink = NO_BLINK;

        void toggleDot(ISYSTEM ISystem);
        void LED_Cleaner(ISYSTEM ISystem, bool alarm);
        void displayDigits(ISYSTEM ISystem);
        void updateBuffer(IDATA IData, ISYSTEM ISystem);
};

#endif
