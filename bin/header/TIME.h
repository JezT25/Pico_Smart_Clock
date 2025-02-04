/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef TIME_h
#define TIME_h

#include "setup.hpp"

#define DS3231_ADDR         0x68

#define SECONDS             0
#define MINUTES             1
#define HOURS               2
#define DOW                 3
#define DAY                 4
#define MONTH               5
#define YEAR                6
#define ALARM_POWER         7
#define ALARM_MINUTES       8
#define ALARM_HOURS         9

#define DATA_REG            0x00
#define SECOND_REG          0x01
#define DATE_REG            0x04
#define YEAR_REG            0x06
#define ALARM_REG           0x08
#define ALARM_POWER_REG     0x07
#define CENTURY_BIT         0x1F
#define HOUR_24_FORMAT      0x3F
#define ALARM_MIN_MASK      0x7F
#define ALARM_STATE_MASK    0x01

#define SET_TIME_SIZE       3
#define SET_DATE_SIZE       3
#define SET_YEAR_SIZE       2
#define SET_ALARM_SIZE      3
#define TURN_ALARM_SIZE     2
#define CLOCK_WRITE_SIZE    1
#define CLOCK_DATA_SIZE     10

#define RUNNING             true
#define NOT_RUNNING         false

class TIME_class {
    private:
        uint8_t alarm_state;
        uint8_t alarm_minutes;
        uint8_t alarm_hours;
        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
        uint8_t dow;
        uint8_t day;
        uint8_t month;
        uint8_t year;
        
        inline uint8_t bcd_to_decimal(uint8_t bcd);
        inline uint8_t decimal_to_bcd(uint8_t decimal);

    public:
        bool stopwatch_isRunning = false;

        void stopwatch(IDATA *IData);
        void getTime(ISYSTEM *ISystem, IDATA *IData);
        void turnAlarm(ISYSTEM *ISystem);
        void setTime(IDATA *IData);
        void setDate(IDATA *IData);
        void setYear(IDATA *IData);
        void setAlarm(IDATA *IData);
};

#endif
