/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

inline uint8_t TIME_class::bcd_to_decimal(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

void TIME_class::getTime(IDATA *IData)
{
    uint8_t ds3231_data[7];

    i2c_write_blocking(i2c_default, DS3231_ADDR, DATA_REG, 1, true);
    i2c_read_blocking(i2c_default, DS3231_ADDR, ds3231_data, 7, false);

    seconds = bcd_to_decimal(ds3231_data[0]);
    minutes = bcd_to_decimal(ds3231_data[1]);
    hours   = bcd_to_decimal(ds3231_data[2] & HOUR_24_FORMAT);  // Mask 24-hour format
    dow     = bcd_to_decimal(ds3231_data[3]);
    day     = bcd_to_decimal(ds3231_data[4]);
    month   = bcd_to_decimal(ds3231_data[5] & CENTURY_BIT);     // Mask century bit
    year    = bcd_to_decimal(ds3231_data[6]);

    IData->CLOCK_SECOND = seconds;
    IData->CLOCK_MINUTE = minutes;
    IData->CLOCK_HOUR   = hours;
    IData->CLOCK_DAY    = day;
    IData->CLOCK_MONTH  = month;
    IData->CLOCK_YEAR   = year;
}

void TIME_class::setTime()
{
    uint8_t time_data[7] = {
        0x01, // Seconds
        0x34, // Minutes
        0x23, // Hours
        0x00, // Day
        0x20, // Date
        0x01, // Month
        0x25  // Year
    };
    i2c_write_blocking(i2c_default, DS3231_ADDR, time_data, 7, false);
}
