/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef IDEVICE_h
#define IDEVICE_h

#define MODE_COUNT 6

class IDATA
{
    public:
        volatile uint8_t CLOCK_HOUR     =   0;
        
        volatile uint8_t CLOCK_MINUTE   =   0;

        volatile uint8_t CLOCK_SECOND   =   0;
        
        volatile uint8_t CLOCK_DAY      =   0;

        volatile uint8_t CLOCK_MONTH    =   0;
        
        volatile uint8_t CLOCK_YEAR     =   0;

        volatile double SENSOR_TEMP     =   0;
        
        volatile double SENSOR_HUMIDITY =   0;

        volatile double SENSOR_PRESSURE =   0;
};

class ISYSTEM
{
    public:
        enum MODES : uint8_t
        {
            CLOCK_MODE,
            DATE_MODE,
            YEAR_MODE,
            TEMP_MODE,
            HUMI_MODE,
            PRES_MODE
        };

        volatile uint8_t SYSTEM_MODE    =   CLOCK_MODE;
};

#endif
