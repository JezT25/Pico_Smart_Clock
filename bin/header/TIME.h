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

class TIME_class : private HARDWARE_class {
    private:
        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
        uint8_t dow;
        uint8_t day;
        uint8_t month;
        uint8_t year;
        
        inline uint8_t bcd_to_decimal(uint8_t bcd);

    public:
        void getTime(IDATA *IData);
        void setTime();
};

#endif
