/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void LED_class::toggleDot(ISYSTEM ISystem)
{
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.CLOCK_MODE:
            dotState = !dotState;
            break;
        case ISystem.CLOCK_ADJUST_MODE:
        case ISystem.DATE_ADJUST_MODE:
        case ISystem.YEAR_ADJUST_MODE:
            sectionState = !sectionState;
            break;
    }
}

void LED_class::toggleDot_Cleaner(ISYSTEM ISystem)
{
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.CLOCK_MODE:
            break;
        case ISystem.YEAR_MODE:
        case ISystem.YEAR_ADJUST_MODE:
        case ISystem.PRES_MODE:
            dotState = OFF;
            break;
        default:
            dotState = ON;
    }
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.CLOCK_ADJUST_MODE:
        case ISystem.DATE_ADJUST_MODE:
        case ISystem.YEAR_ADJUST_MODE:
            break;
        default:
            sectionState = ON_SEGMENT;
    }
}

void LED_class::displayDigits(ISYSTEM ISystem)
{
    // Dot
    gpio_put(LED_Dot, currentSegment == SEG_2 ? dotState : OFF);

    // 7 Segments
    for (uint8_t i = 0; i < LED_SEGMENT_COUNT; i++)
    {
        gpio_put(LED_Segment7[i], (LED_digitValues[segmentBuffer[ISystem.SYSTEM_MODE][currentSegment]] >> i) & 1);
    }

    // Partitions
    for (uint8_t i = 0; i < LED_PARTITION_COUNT; i++)
    {
        gpio_put(LED_Section[i], sectionState ? (i == currentSegment) : 
            ((currentSection_blink == RIGHT_BLINK && currentSegment < SEG_3 && i == currentSegment) || 
            (currentSection_blink == LEFT_BLINK && currentSegment > SEG_2 && i == currentSegment) ? ON_SEGMENT : OFF_SEGMENT));
    }

    currentSegment = currentSegment == SEG_4 ? SEG_1 : ++currentSegment;
}

void LED_class::updateBuffer(IDATA IData, ISYSTEM ISystem)
{
    switch(ISystem.SYSTEM_MODE)
    {
        case (ISystem.CLOCK_MODE):
            // Get Time
            segmentBuffer[ISystem.CLOCK_MODE][SEG_1] = getTens(IData.CLOCK_HOUR);
            segmentBuffer[ISystem.CLOCK_MODE][SEG_2] = getOnes(IData.CLOCK_HOUR);
            segmentBuffer[ISystem.CLOCK_MODE][SEG_3] = getTens(IData.CLOCK_MINUTE);
            segmentBuffer[ISystem.CLOCK_MODE][SEG_4] = getOnes(IData.CLOCK_MINUTE);
            break;
        case (ISystem.DATE_MODE):
            // Get Date
            segmentBuffer[ISystem.DATE_MODE][SEG_1] = getTens(IData.CLOCK_DAY, EN_BLANK);
            segmentBuffer[ISystem.DATE_MODE][SEG_2] = getOnes(IData.CLOCK_DAY);
            segmentBuffer[ISystem.DATE_MODE][SEG_3] = getTens(IData.CLOCK_MONTH, EN_BLANK);
            segmentBuffer[ISystem.DATE_MODE][SEG_4] = getOnes(IData.CLOCK_MONTH);
            break;
        case (ISystem.YEAR_MODE):
            // Get Year
            segmentBuffer[ISystem.YEAR_MODE][SEG_3] = getTens(IData.CLOCK_YEAR);
            segmentBuffer[ISystem.YEAR_MODE][SEG_4] = getOnes(IData.CLOCK_YEAR);
            break;
        case (ISystem.TEMP_MODE):
            // Get Temperature
            segmentBuffer[ISystem.TEMP_MODE][SEG_1] = (IData.SENSOR_TEMP < 0) ? DASH : getTens(IData.SENSOR_TEMP, EN_BLANK);
            segmentBuffer[ISystem.TEMP_MODE][SEG_2] = getOnes(IData.SENSOR_TEMP);
            segmentBuffer[ISystem.TEMP_MODE][SEG_3] = getDecimal(IData.SENSOR_TEMP);
            break;
        case (ISystem.HUMI_MODE):
            // Get Humidity
            segmentBuffer[ISystem.HUMI_MODE][SEG_1] = getTens(IData.SENSOR_HUMIDITY, EN_BLANK);
            segmentBuffer[ISystem.HUMI_MODE][SEG_2] = getOnes(IData.SENSOR_HUMIDITY);
            segmentBuffer[ISystem.HUMI_MODE][SEG_3] = getDecimal(IData.SENSOR_HUMIDITY);
            break;
        case (ISystem.PRES_MODE):
            // Get Pressure
            segmentBuffer[ISystem.PRES_MODE][SEG_1] = getHundred(IData.SENSOR_PRESSURE, EN_BLANK);
            segmentBuffer[ISystem.PRES_MODE][SEG_2] = getTens(IData.SENSOR_PRESSURE);
            segmentBuffer[ISystem.PRES_MODE][SEG_3] = getOnes(IData.SENSOR_PRESSURE);
            break;
        case (ISystem.CLOCK_ADJUST_MODE):
            // Clock Adjust
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_1] = getTens(IData.ADJUST_HOUR);
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_2] = getOnes(IData.ADJUST_HOUR);
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_MINUTE);
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_MINUTE);
            break;
        case (ISystem.DATE_ADJUST_MODE):
            // Date Adjust
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_1] = getTens(IData.ADJUST_DAY, EN_BLANK);
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_2] = getOnes(IData.ADJUST_DAY);
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_MONTH, EN_BLANK);
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_MONTH);
            break;
        case (ISystem.YEAR_ADJUST_MODE):
            // Year Adjust
            segmentBuffer[ISystem.YEAR_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_YEAR);
            segmentBuffer[ISystem.YEAR_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_YEAR);
            break;
    }  
}

inline uint8_t LED_class::getDecimal(double number)
{   
    return ((number < 0 ? -number : number) - (int)number) * 10;
}

inline uint8_t LED_class::getOnes(uint8_t number)
{
    return (number < 0 ? -number : number) % 10;
}

inline uint8_t LED_class::getTens(uint8_t number, bool blank)
{
    return (number > 99 ? (number / 10) % 10 : number / 10) ?: (!blank ? ZERO : DIGIT_OFF);
}

inline uint8_t LED_class::getHundred(uint8_t number, bool blank)
{
    return (number / 100) ?: (!blank ? ZERO : DIGIT_OFF);
}
