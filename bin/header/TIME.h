/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef TIME_h
#define TIME_h

#include "../setup.hpp"

#define DS3231_ADDR         0x68

#define SECONDS             0
#define MINUTES             1
#define HOURS               2
#define DOW                 3
#define DAY                 4
#define MONTH               5
#define YEAR                6

#define DATA_REG            0x00
#define SECOND_REG          0x01
#define DATE_REG            0x04
#define YEAR_REG            0x06
#define CENTURY_BIT         0x1F
#define HOUR_24_FORMAT      0x3F

#define SET_TIME_SIZE       3
#define SET_DATE_SIZE       3
#define SET_YEAR_SIZE       2
#define CLOCK_WRITE_SIZE    1
#define CLOCK_DATA_SIZE     7

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
        inline uint8_t decimal_to_bcd(uint8_t decimal);

    public:
        void getTime(IDATA *IData);
        void setTime(IDATA *IData);
        void setDate(IDATA *IData);
        void setYear(IDATA *IData);
};

#endif
