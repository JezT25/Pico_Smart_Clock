/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef SYSTEM_h
#define SYSTEM_h

#include "../setup.hpp"

#define ISR_START_ON_MS     1000
#define DISPLAY_REFRESH_US  5000
#define STW_REFRESH_US      10000
#define TIME_REFRESH_US     500000

#define AUTOVIEW_ON         true
#define AUTOVIEW_OFF        false
#define AUTOVIEW_INTERVAL   5000

class SYSTEM_class {
    private:
        IDATA&          _IData;
        ISYSTEM&        _ISystem;
        GPIO_class&     _GPIO;
        LED_class&      _LED;
        SENSOR_class&   _SENSOR;
        TIME_class&     _TIME;
        HWIO_class&     _HWIO;
        WIFI_class&     _WIFI;

        bool autoview         = AUTOVIEW_OFF;
        uint32_t autoview_lpt = 0;

        void system_modeHandler();
        void system_autoviewHandler();
        static long long int DisplayISR(alarm_id_t id, void* user_data);
        static long long int ClockISR(alarm_id_t id, void* user_data);
        static long long int StopwatchISR(alarm_id_t id, void* user_data);

    public:
        SYSTEM_class(SystemComponents& class_lib) : _IData(class_lib._IData), _ISystem(class_lib._ISystem), _GPIO(class_lib._GPIO), _LED(class_lib._LED), _SENSOR(class_lib._SENSOR), _TIME(class_lib._TIME), _HWIO(class_lib._HWIO), _WIFI(class_lib._WIFI) {}

        void Initialize();
        void Run();
};

#endif
