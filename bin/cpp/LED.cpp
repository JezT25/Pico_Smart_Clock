/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void LED_class::toggleDot(ISYSTEM ISystem, bool stopwatch)
{
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.STOPWATCH_MODE:
            dotState = stopwatch == RUNNING ? !dotState : ON;
            break;
        case ISystem.SPLIT_MODE:
            sectionState = stopwatch == RUNNING ? !sectionState : ON_SEGMENT;
            break;
        case ISystem.CLOCK_MODE:
            dotState = alarmState == ALARM_OFF ? !dotState : ON;
            if (alarmState == ALARM_OFF) break;
        case ISystem.ALARM_ADJUST_MODE:
        case ISystem.CLOCK_ADJUST_MODE:
        case ISystem.DATE_ADJUST_MODE:
        case ISystem.YEAR_ADJUST_MODE:
            sectionState = !sectionState;
            break;
    }
}

void LED_class::LED_Cleaner(ISYSTEM ISystem, bool alarm, bool stopwatch)
{
    // Dot Cleaner
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.STOPWATCH_MODE:
            if (stopwatch == NOT_RUNNING) { dotState = ON; break; }
        case ISystem.CLOCK_MODE:
            alarmState = alarm;
            break;
        case ISystem.AUTO_VIEW_MODE:
        case ISystem.STOPWATCH_MENU_MODE:
        case ISystem.ALARM_MODE:
        case ISystem.YEAR_MODE:
        case ISystem.YEAR_ADJUST_MODE:
        case ISystem.PRES_MODE:
            dotState = OFF;
            break;
        default:
            dotState = ON;
    }

    // Section Blink Cleaner
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.CLOCK_MODE:
            if (alarmState == ALARM_ON) break;
        case ISystem.SPLIT_MODE:
            if (stopwatch == NOT_RUNNING) { sectionState = ON_SEGMENT; break; }
        case ISystem.ALARM_ADJUST_MODE:
        case ISystem.CLOCK_ADJUST_MODE:
        case ISystem.DATE_ADJUST_MODE:
        case ISystem.YEAR_ADJUST_MODE:
            break;
        default:
            sectionState = ON_SEGMENT;
    }

    // Segment Blink Cleaner
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.SPLIT_MODE:
            if (stopwatch == NOT_RUNNING) { currentSection_blink = NO_BLINK; break; }
        case ISystem.CLOCK_MODE:
        case ISystem.ALARM_ADJUST_MODE:
        case ISystem.CLOCK_ADJUST_MODE:
        case ISystem.DATE_ADJUST_MODE:
        case ISystem.YEAR_ADJUST_MODE:
            break;
        default:
            currentSection_blink = NO_BLINK;
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
    bool segmentState;
    for (uint8_t i = 0; i < LED_PARTITION_COUNT; i++)
    {
        switch (sectionState)
        {
            case ON_SEGMENT:
                segmentState = (i == currentSegment);
                break;
            case OFF_SEGMENT:
                switch (currentSection_blink)
                {
                    case RIGHT_BLINK:
                        segmentState = (currentSegment < SEG_3 && i == currentSegment) ? ON_SEGMENT : OFF_SEGMENT;
                        break;
                    case LEFT_BLINK:
                        segmentState = (currentSegment > SEG_2 && i == currentSegment) ? ON_SEGMENT : OFF_SEGMENT;
                        break;
                    case ALL_BLINK:
                    default:
                        segmentState = OFF_SEGMENT;
                }
                break;
        }
        gpio_put(LED_Section[i], segmentState);
    }

    currentSegment = currentSegment == SEG_4 ? SEG_1 : ++currentSegment;
}

void LED_class::updateBuffer(IDATA IData, ISYSTEM ISystem)
{
    switch(ISystem.SYSTEM_MODE)
    {
        case (ISystem.ALARM_MODE):
            segmentBuffer[ISystem.ALARM_MODE][SEG_4] = ISystem.ALARM_STATE == ALARM_OFF ? F : n;
            break;
        case (ISystem.CLOCK_MODE):
            segmentBuffer[ISystem.CLOCK_MODE][SEG_1] = getTens(IData.CLOCK_HOUR);
            segmentBuffer[ISystem.CLOCK_MODE][SEG_2] = getOnes(IData.CLOCK_HOUR);
            segmentBuffer[ISystem.CLOCK_MODE][SEG_3] = getTens(IData.CLOCK_MINUTE);
            segmentBuffer[ISystem.CLOCK_MODE][SEG_4] = getOnes(IData.CLOCK_MINUTE);
            break;
        case (ISystem.DATE_MODE):
            segmentBuffer[ISystem.DATE_MODE][SEG_1] = getTens(IData.CLOCK_DAY, EN_BLANK);
            segmentBuffer[ISystem.DATE_MODE][SEG_2] = getOnes(IData.CLOCK_DAY);
            segmentBuffer[ISystem.DATE_MODE][SEG_3] = getTens(IData.CLOCK_MONTH, EN_BLANK);
            segmentBuffer[ISystem.DATE_MODE][SEG_4] = getOnes(IData.CLOCK_MONTH);
            break;
        case (ISystem.YEAR_MODE):
            segmentBuffer[ISystem.YEAR_MODE][SEG_3] = getTens(IData.CLOCK_YEAR);
            segmentBuffer[ISystem.YEAR_MODE][SEG_4] = getOnes(IData.CLOCK_YEAR);
            break;
        case (ISystem.TEMP_MODE):
            segmentBuffer[ISystem.TEMP_MODE][SEG_1] = (IData.SENSOR_TEMP < 0) ? DASH : getTens(IData.SENSOR_TEMP, EN_BLANK);
            segmentBuffer[ISystem.TEMP_MODE][SEG_2] = getOnes(IData.SENSOR_TEMP);
            segmentBuffer[ISystem.TEMP_MODE][SEG_3] = getDecimal(IData.SENSOR_TEMP);
            break;
        case (ISystem.HUMI_MODE):
            segmentBuffer[ISystem.HUMI_MODE][SEG_1] = getTens(IData.SENSOR_HUMIDITY, EN_BLANK);
            segmentBuffer[ISystem.HUMI_MODE][SEG_2] = getOnes(IData.SENSOR_HUMIDITY);
            segmentBuffer[ISystem.HUMI_MODE][SEG_3] = getDecimal(IData.SENSOR_HUMIDITY);
            break;
        case (ISystem.PRES_MODE):
            segmentBuffer[ISystem.PRES_MODE][SEG_1] = getHundred(IData.SENSOR_PRESSURE, EN_BLANK);
            segmentBuffer[ISystem.PRES_MODE][SEG_2] = getTens(IData.SENSOR_PRESSURE);
            segmentBuffer[ISystem.PRES_MODE][SEG_3] = getOnes(IData.SENSOR_PRESSURE);
            break;
        case (ISystem.ALARM_ADJUST_MODE):
            segmentBuffer[ISystem.ALARM_ADJUST_MODE][SEG_1] = getTens(IData.ADJUST_ALARM_HOUR);
            segmentBuffer[ISystem.ALARM_ADJUST_MODE][SEG_2] = getOnes(IData.ADJUST_ALARM_HOUR);
            segmentBuffer[ISystem.ALARM_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_ALARM_MINUTE);
            segmentBuffer[ISystem.ALARM_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_ALARM_MINUTE);
            break;
        case (ISystem.CLOCK_ADJUST_MODE):
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_1] = getTens(IData.ADJUST_HOUR);
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_2] = getOnes(IData.ADJUST_HOUR);
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_MINUTE);
            segmentBuffer[ISystem.CLOCK_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_MINUTE);
            break;
        case (ISystem.DATE_ADJUST_MODE):
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_1] = getTens(IData.ADJUST_DAY, EN_BLANK);
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_2] = getOnes(IData.ADJUST_DAY);
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_MONTH, EN_BLANK);
            segmentBuffer[ISystem.DATE_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_MONTH);
            break;
        case (ISystem.YEAR_ADJUST_MODE):
            segmentBuffer[ISystem.YEAR_ADJUST_MODE][SEG_3] = getTens(IData.ADJUST_YEAR);
            segmentBuffer[ISystem.YEAR_ADJUST_MODE][SEG_4] = getOnes(IData.ADJUST_YEAR);
            break;
        case (ISystem.STOPWATCH_MODE):
            segmentBuffer[ISystem.STOPWATCH_MODE][SEG_1] = getTens(IData.STOPWATCH_HOUR > 0 ? IData.STOPWATCH_HOUR : (IData.STOPWATCH_MINUTE > 0 ? IData.STOPWATCH_MINUTE : IData.STOPWATCH_SECOND), EN_BLANK);
            segmentBuffer[ISystem.STOPWATCH_MODE][SEG_2] = getOnes(IData.STOPWATCH_HOUR > 0 ? IData.STOPWATCH_HOUR : (IData.STOPWATCH_MINUTE > 0 ? IData.STOPWATCH_MINUTE : IData.STOPWATCH_SECOND));
            segmentBuffer[ISystem.STOPWATCH_MODE][SEG_3] = getTens(IData.STOPWATCH_HOUR > 0 ? IData.STOPWATCH_MINUTE : (IData.STOPWATCH_SECOND > 0 ? IData.STOPWATCH_SECOND : IData.STOPWATCH_MS));
            segmentBuffer[ISystem.STOPWATCH_MODE][SEG_4] = getOnes(IData.STOPWATCH_HOUR > 0 ? IData.STOPWATCH_MINUTE : (IData.STOPWATCH_SECOND > 0 ? IData.STOPWATCH_SECOND : IData.STOPWATCH_MS));
            break;
        case (ISystem.SPLIT_MODE):
            segmentBuffer[ISystem.SPLIT_MODE][SEG_1] = getTens(IData.SPLIT_HOUR > 0 ? IData.SPLIT_HOUR : (IData.SPLIT_MINUTE > 0 ? IData.SPLIT_MINUTE : IData.SPLIT_SECOND), EN_BLANK);
            segmentBuffer[ISystem.SPLIT_MODE][SEG_2] = getOnes(IData.SPLIT_HOUR > 0 ? IData.SPLIT_HOUR : (IData.SPLIT_MINUTE > 0 ? IData.SPLIT_MINUTE : IData.SPLIT_SECOND));
            segmentBuffer[ISystem.SPLIT_MODE][SEG_3] = getTens(IData.SPLIT_HOUR > 0 ? IData.SPLIT_MINUTE : (IData.SPLIT_SECOND > 0 ? IData.SPLIT_SECOND : IData.SPLIT_MS));
            segmentBuffer[ISystem.SPLIT_MODE][SEG_4] = getOnes(IData.SPLIT_HOUR > 0 ? IData.SPLIT_MINUTE : (IData.SPLIT_SECOND > 0 ? IData.SPLIT_SECOND : IData.SPLIT_MS));
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
