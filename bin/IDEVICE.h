/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef IDEVICE_h
#define IDEVICE_h

#define MODE_COUNT      8
#define ALL_MODE_COUNT  14

#define ALARM_ON        true
#define ALARM_OFF       false

class IDATA
{
    public:
        // Normal Mode Variables
        volatile uint8_t ALARM_HOUR             =   0;

        volatile uint8_t ALARM_MINUTE           =   0;

        volatile uint8_t CLOCK_HOUR             =   0;
        
        volatile uint8_t CLOCK_MINUTE           =   0;
        
        volatile uint8_t CLOCK_DAY              =   0;

        volatile uint8_t CLOCK_MONTH            =   0;
        
        volatile uint8_t CLOCK_YEAR             =   0;

        volatile double SENSOR_TEMP             =   0;
        
        volatile double SENSOR_HUMIDITY         =   0;

        volatile double SENSOR_PRESSURE         =   0;

        volatile uint8_t STOPWATCH_MS           =   0;

        volatile uint8_t STOPWATCH_HOUR         =   0;

        volatile uint8_t STOPWATCH_MINUTE       =   0;

        volatile uint8_t STOPWATCH_SECOND       =   0;

        volatile uint8_t SPLIT_MS               =   0;

        volatile uint8_t SPLIT_HOUR             =   0;

        volatile uint8_t SPLIT_MINUTE           =   0;

        volatile uint8_t SPLIT_SECOND           =   0;

        // Special Mode Variables
        volatile uint8_t ADJUST_ALARM_HOUR      =   0;

        volatile uint8_t ADJUST_ALARM_MINUTE    =   0;

        volatile uint8_t ADJUST_HOUR            =   0;
        
        volatile uint8_t ADJUST_MINUTE          =   0;

        volatile uint8_t ADJUST_DAY             =   0;

        volatile uint8_t ADJUST_MONTH           =   0;

        volatile uint8_t ADJUST_YEAR            =   0;
};

// todo; auto cycle mode must not show alarm states
// todo, somethimes when we plug in with alarm on it will trigger alarm?
// todo: polish buttons
class ISYSTEM
{
    public:
        // When updating here, update LED.h for buffer updates
        // Update updateBuffer(_IData, _ISystem);
        // Update toggleDot & LED_Cleaner
        enum MODES : uint8_t
        {
            // Normal Modes
            ALARM_MODE,
            CLOCK_MODE,
            DATE_MODE,
            YEAR_MODE,
            TEMP_MODE,
            HUMI_MODE,
            PRES_MODE,
            STOPWATCH_MENU_MODE,

            // Special Modes
            STOPWATCH_MODE,
            SPLIT_MODE,
            ALARM_ADJUST_MODE,
            CLOCK_ADJUST_MODE,
            DATE_ADJUST_MODE,
            YEAR_ADJUST_MODE
        };

        volatile bool ALARM_STATE       =   ALARM_OFF;

        volatile uint8_t SYSTEM_MODE    =   CLOCK_MODE; 
};

#endif
