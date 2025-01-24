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
#define TIME_REFRESH_US     500000

class SYSTEM_class {
    private:
        IDATA&          _IData;
        ISYSTEM&        _ISystem;
        HARDWARE_class& _HARDWARE;
        LED_class&      _LED;
        SENSOR_class&   _SENSOR;
        TIME_class&     _TIME;
        HWIO_class&     _HWIO;

        static long long int DisplayISR(alarm_id_t id, void* user_data);
        static long long int ClockISR(alarm_id_t id, void* user_data);

    public:
        SYSTEM_class(SystemComponents& class_lib) : _IData(class_lib._IData), _ISystem(class_lib._ISystem), _HARDWARE(class_lib._HARDWARE), _LED(class_lib._LED), _SENSOR(class_lib._SENSOR), _TIME(class_lib._TIME), _HWIO(class_lib._HWIO) {}

        void Initialize();
        void Run();
};

#endif
