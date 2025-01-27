/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef IDEVICE_h
#define IDEVICE_h

#define MODE_COUNT      6
#define ALL_MODE_COUNT  9

class IDATA
{
    public:
        // Normal Mode Variables
        volatile uint8_t CLOCK_HOUR     =   0;
        
        volatile uint8_t CLOCK_MINUTE   =   0;
        
        volatile uint8_t CLOCK_DAY      =   0;

        volatile uint8_t CLOCK_MONTH    =   0;
        
        volatile uint8_t CLOCK_YEAR     =   0;

        volatile double SENSOR_TEMP     =   0;
        
        volatile double SENSOR_HUMIDITY =   0;

        volatile double SENSOR_PRESSURE =   0;

        // Special Mode Variables
        volatile uint8_t ADJUST_HOUR    =   0;
        
        volatile uint8_t ADJUST_MINUTE  =   0;

        volatile uint8_t ADJUST_DAY     =   0;

        volatile uint8_t ADJUST_MONTH   =   0;

        volatile uint8_t ADJUST_YEAR    =   0;
};

class ISYSTEM
{
    public:
        // When updating here, update LED.h for buffer updates
        // Update updateBuffer(_IData, _ISystem);
        // Update toggleDot & toggleDot_Cleaner
        enum MODES : uint8_t
        {
            // Normal Modes
            CLOCK_MODE,
            DATE_MODE,
            YEAR_MODE,
            TEMP_MODE,
            HUMI_MODE,
            PRES_MODE,

            // Special Modes
            CLOCK_ADJUST_MODE,
            DATE_ADJUST_MODE,
            YEAR_ADJUST_MODE
        };

        volatile uint8_t SYSTEM_MODE    =   CLOCK_MODE;
};

#endif
