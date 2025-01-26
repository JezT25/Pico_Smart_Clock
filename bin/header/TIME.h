/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef TIME_h
#define TIME_h

#include "../setup.hpp"

#define DS3231_ADDR     0x68

#define DATA_REG        0x00
#define CENTURY_BIT     0x1F
#define HOUR_24_FORMAT  0x3F

class TIME_class {
    private:
        volatile uint8_t seconds;
        volatile uint8_t minutes;
        volatile uint8_t hours;
        volatile uint8_t dow;
        volatile uint8_t day;
        volatile uint8_t month;
        volatile uint8_t year;
        
        inline uint8_t bcd_to_decimal(uint8_t bcd);

    public:
        void getTime(IDATA *IData);
        void setTime();
};

#endif
