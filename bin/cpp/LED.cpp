/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void LED_class::toggleDot()
{
    dotState = !dotState;
}

void LED_class::toggleDot(ISYSTEM ISystem)
{
    switch(ISystem.SYSTEM_MODE)
    {   
        case ISystem.CLOCK_MODE:
            break;
        case ISystem.YEAR_MODE:
            dotState = OFF;
            break;
        default:
            dotState = ON;
            break;
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
        gpio_put(LED_Section[i], i == currentSegment);
    }

    currentSegment = currentSegment == SEG_4 ? SEG_1 : ++currentSegment;
}

void LED_class::updateBuffer(IDATA IData, ISYSTEM ISystem)
{
    // Get Time
    segmentBuffer[ISystem.CLOCK_MODE][0] = IData.CLOCK_HOUR / 10;
    segmentBuffer[ISystem.CLOCK_MODE][1] = IData.CLOCK_HOUR % 10;
    segmentBuffer[ISystem.CLOCK_MODE][2] = IData.CLOCK_MINUTE / 10;
    segmentBuffer[ISystem.CLOCK_MODE][3] = IData.CLOCK_MINUTE % 10;

    // Get Date
    segmentBuffer[ISystem.DATE_MODE][0] = (IData.CLOCK_DAY / 10) ?: 10;
    segmentBuffer[ISystem.DATE_MODE][1] = IData.CLOCK_DAY % 10;
    segmentBuffer[ISystem.DATE_MODE][2] = (IData.CLOCK_MONTH / 10) ?: 10;
    segmentBuffer[ISystem.DATE_MODE][3] = IData.CLOCK_MONTH % 10;

    // Get Year
    segmentBuffer[ISystem.YEAR_MODE][2] = IData.CLOCK_YEAR / 10;
    segmentBuffer[ISystem.YEAR_MODE][3] = IData.CLOCK_YEAR % 10;
}
