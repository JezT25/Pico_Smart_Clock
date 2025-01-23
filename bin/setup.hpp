/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef setup_hpp_included
#define setup_hpp_included

/** Standard Pico Libraries **/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

/** Pico Smart Clock Libraries **/
#include "IDEVICE.h"
#include "header/HARDWARE.h"
#include "cpp/HARDWARE.cpp"
#include "header/LED.h"
#include "cpp/LED.cpp"
#include "header/TIME.h"
#include "cpp/TIME.cpp"
#include "header/HWIO.h"
#include "cpp/HWIO.cpp"

struct SystemComponents {
    // Data Handling Classes
    IDATA          _IData;
    ISYSTEM        _ISystem;

    // Hardware Classes
    HARDWARE_class _HARDWARE;
    LED_class      _LED;
    TIME_class     _TIME;
    HWIO_class     _HWIO;
};

/** System Handler Library **/
#include "header/SYSTEM.h"
#include "cpp/SYSTEM.cpp"

#endif
