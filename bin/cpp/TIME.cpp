/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "setup.hpp"

inline uint8_t TIME_class::bcd_to_decimal(uint8_t bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

inline uint8_t TIME_class::decimal_to_bcd(uint8_t decimal)
{
    return ((decimal / 10) << 4) | (decimal % 10);
}

void TIME_class::stopwatch(IDATA *IData)
{
    if (stopwatch_isRunning == RUNNING)
    {
        if (IData->STOPWATCH_MS >= 99)
        {
            IData->STOPWATCH_MS = 0;
            IData->STOPWATCH_MINUTE++;
        }
        else
        {
            IData->STOPWATCH_MS++;
        }

        if (IData->STOPWATCH_SECOND > 59)
        {
            IData->STOPWATCH_SECOND = 0;
            IData->STOPWATCH_MINUTE++;
        }
        if (IData->STOPWATCH_MINUTE > 59)
        {
            IData->STOPWATCH_MINUTE = 0;
            IData->STOPWATCH_HOUR++;
        }
    }
}

void TIME_class::getTime(ISYSTEM *ISystem, IDATA *IData)
{
    uint8_t ds3231_data[CLOCK_DATA_SIZE];

    i2c_write_blocking(i2c_default, DS3231_ADDR, DATA_REG, CLOCK_WRITE_SIZE, true);
    i2c_read_blocking(i2c_default, DS3231_ADDR, ds3231_data, CLOCK_DATA_SIZE, false);

    seconds = bcd_to_decimal(ds3231_data[SECONDS]);
    minutes = bcd_to_decimal(ds3231_data[MINUTES]);
    hours   = bcd_to_decimal(ds3231_data[HOURS] & HOUR_24_FORMAT);  // Mask 24-hour format
    dow     = bcd_to_decimal(ds3231_data[DOW]);
    day     = bcd_to_decimal(ds3231_data[DAY]);
    month   = bcd_to_decimal(ds3231_data[MONTH] & CENTURY_BIT);     // Mask century bit
    year    = bcd_to_decimal(ds3231_data[YEAR]);
    alarm_state = bcd_to_decimal(ds3231_data[ALARM_POWER] & ALARM_STATE_MASK);     // Keep only 1st bit
    alarm_minutes = bcd_to_decimal(ds3231_data[ALARM_MINUTES] & ALARM_MIN_MASK);   // Mask alarm bits
    alarm_hours   = bcd_to_decimal(ds3231_data[ALARM_HOURS] & HOUR_24_FORMAT);     // Mask 24-hour format

    ISystem->ALARM_STATE = alarm_state;
    IData->ALARM_MINUTE = alarm_minutes;
    IData->ALARM_HOUR   = alarm_hours;
    IData->CLOCK_MINUTE = minutes;
    IData->CLOCK_HOUR   = hours;
    IData->CLOCK_DAY    = day;
    IData->CLOCK_MONTH  = month;
    IData->CLOCK_YEAR   = year;
}

void TIME_class::setTime(IDATA *IData)
{
    uint8_t time_data[SET_TIME_SIZE] =
    {
        SECOND_REG,
        decimal_to_bcd(IData->ADJUST_MINUTE),   // Minutes
        decimal_to_bcd(IData->ADJUST_HOUR),     // Hours
    };

    IData->CLOCK_MINUTE = IData->ADJUST_MINUTE;
    IData->CLOCK_HOUR   = IData->ADJUST_HOUR;

    i2c_write_blocking(i2c_default, DS3231_ADDR, time_data, SET_TIME_SIZE, false);
}

void TIME_class::setDate(IDATA *IData)
{
    uint8_t date_data[SET_DATE_SIZE] =
    {
        DATE_REG,
        decimal_to_bcd(IData->ADJUST_DAY),      // Date
        decimal_to_bcd(IData->ADJUST_MONTH),    // Month
    };

    IData->CLOCK_DAY   = IData->ADJUST_DAY;
    IData->CLOCK_MONTH = IData->ADJUST_MONTH;

    i2c_write_blocking(i2c_default, DS3231_ADDR, date_data, SET_DATE_SIZE, false);
}

void TIME_class::setYear(IDATA *IData)
{
    uint8_t year_data[SET_YEAR_SIZE] =
    {
        YEAR_REG,
        decimal_to_bcd(IData->ADJUST_YEAR),    // Year
    };

    IData->CLOCK_YEAR = IData->ADJUST_YEAR;

    i2c_write_blocking(i2c_default, DS3231_ADDR, year_data, SET_YEAR_SIZE, false);
}

void TIME_class::setAlarm(IDATA *IData)
{
    uint8_t alarm_data[SET_ALARM_SIZE] =
    {
        ALARM_REG,
        decimal_to_bcd(IData->ADJUST_ALARM_MINUTE),  // Minutes
        decimal_to_bcd(IData->ADJUST_ALARM_HOUR),    // Hours
    };

    IData->ALARM_HOUR = IData->ADJUST_ALARM_HOUR;
    IData->ALARM_MINUTE = IData->ADJUST_ALARM_MINUTE;

    i2c_write_blocking(i2c_default, DS3231_ADDR, alarm_data, SET_ALARM_SIZE, false);
}

void TIME_class::turnAlarm(ISYSTEM *ISystem)
{
    ISystem->ALARM_STATE = !ISystem->ALARM_STATE;

    uint8_t alarm_data[TURN_ALARM_SIZE] =
    {
        ALARM_POWER_REG,                            // Alarm State
        decimal_to_bcd(ISystem->ALARM_STATE)
    };

    i2c_write_blocking(i2c_default, DS3231_ADDR, alarm_data, TURN_ALARM_SIZE, false);
}
