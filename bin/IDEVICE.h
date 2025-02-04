/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef IDEVICE_h
#define IDEVICE_h

#define MODE_COUNT      9
#define ALL_MODE_COUNT  15

#define ALARM_ON        true
#define ALARM_OFF       false

class IDATA
{
    public:
        // Normal Mode Variables
        uint8_t ALARM_HOUR             =   0;

        uint8_t ALARM_MINUTE           =   0;

        uint8_t CLOCK_HOUR             =   0;
        
        uint8_t CLOCK_MINUTE           =   0;
        
        uint8_t CLOCK_DAY              =   0;

        uint8_t CLOCK_MONTH            =   0;
        
        uint8_t CLOCK_YEAR             =   0;

        double SENSOR_TEMP             =   0;
        
        double SENSOR_HUMIDITY         =   0;

        double SENSOR_PRESSURE         =   0;

        uint8_t STOPWATCH_MS           =   0;

        uint8_t STOPWATCH_HOUR         =   0;

        uint8_t STOPWATCH_MINUTE       =   0;

        uint8_t STOPWATCH_SECOND       =   0;

        uint8_t SPLIT_MS               =   0;

        uint8_t SPLIT_HOUR             =   0;

        uint8_t SPLIT_MINUTE           =   0;

        uint8_t SPLIT_SECOND           =   0;

        // Special Mode Variables
        uint8_t ADJUST_ALARM_HOUR      =   0;

        uint8_t ADJUST_ALARM_MINUTE    =   0;

        uint8_t ADJUST_HOUR            =   0;
        
        uint8_t ADJUST_MINUTE          =   0;

        uint8_t ADJUST_DAY             =   0;

        uint8_t ADJUST_MONTH           =   0;

        uint8_t ADJUST_YEAR            =   0;
};

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
            AUTO_VIEW_MODE,
            STOPWATCH_MENU_MODE,

            // Special Modes
            STOPWATCH_MODE,
            SPLIT_MODE,
            ALARM_ADJUST_MODE,
            CLOCK_ADJUST_MODE,
            DATE_ADJUST_MODE,
            YEAR_ADJUST_MODE
        };

        bool ALARM_STATE       =   ALARM_OFF;

        uint8_t SYSTEM_MODE    =   CLOCK_MODE; 
};

class SYS_SETTINGS
{
    public:
        bool SYS_WIFI_EN;
        bool SYS_WIFI_CON;
};
SYS_SETTINGS _set;

#endif
