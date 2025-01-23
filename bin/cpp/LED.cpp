/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void LED_class::displayDigits(ISYSTEM ISystem)
{
    // toggle_dots();
    // gpio_put(LED_Dot, dotBuffer[currentSegment]);

    for (uint8_t i = 0; i < LED_SEGMENT_COUNT; i++)
    {
        gpio_put(LED_Segment7[i], (LED_digitValues[segmentBuffer[ISystem.SYSTEM_MODE][currentSegment]] >> i) & 1);
    }

    for (uint8_t i = 0; i < LED_PARTITION_COUNT; i++)
    {
        gpio_put(LED_Section[i], i == currentSegment);
    }

    currentSegment = currentSegment == SEG_4 ? SEG_1 : ++currentSegment;
}

void LED_class::updateBuffer(IDATA IData)
{
    // Get Time
    segmentBuffer[CLOCK_MODE][0] = IData.CLOCK_HOUR / 10;
    segmentBuffer[CLOCK_MODE][1] = IData.CLOCK_HOUR % 10;
    segmentBuffer[CLOCK_MODE][2] = IData.CLOCK_MINUTE / 10;
    segmentBuffer[CLOCK_MODE][3] = IData.CLOCK_MINUTE % 10;

    // Get Date
    segmentBuffer[DATE_MODE][0] = (IData.CLOCK_DAY / 10) ?: 10;
    segmentBuffer[DATE_MODE][1] = IData.CLOCK_DAY % 10;
    segmentBuffer[DATE_MODE][2] = (IData.CLOCK_MONTH / 10) ?: 10;
    segmentBuffer[DATE_MODE][3] = IData.CLOCK_MONTH % 10;

    // Get Year
    segmentBuffer[YEAR_MODE][2] = IData.CLOCK_YEAR / 10;
    segmentBuffer[YEAR_MODE][3] = IData.CLOCK_YEAR % 10;
}
