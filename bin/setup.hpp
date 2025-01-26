/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef setup_hpp_included
#define setup_hpp_included

/** Standard Pico Libraries **/
#include <stdio.h>
#include <cstring>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

/** Pico Smart Clock Libraries **/
#include "IDEVICE.h"

#include "header/HARDWARE.h"
#include "header/LED.h"
#include "header/SENSOR.h"
#include "header/TIME.h"
#include "header/HWIO.h"

#include "cpp/HARDWARE.cpp"
#include "cpp/LED.cpp"
#include "cpp/SENSOR.cpp"
#include "cpp/TIME.cpp"
#include "cpp/HWIO.cpp"

struct SystemComponents {
    // Data Handling Classes
    IDATA          _IData;
    ISYSTEM        _ISystem;

    // Hardware Classes
    HARDWARE_class _HARDWARE;
    LED_class      _LED;
    SENSOR_class   _SENSOR;
    TIME_class     _TIME;
    HWIO_class     _HWIO;
};

/** System Handler Library **/
#include "header/SYSTEM.h"
#include "cpp/SYSTEM.cpp"

#endif
