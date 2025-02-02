/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef setup_hpp_included
#define setup_hpp_included

// Settings
#define VERBOSE     0
#define WIFI_EN     0
#define WIFI_CON    0

/** Standard Pico Libraries **/
#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <cstring>
#include <math.h>

/** Pico Smart Clock Libraries **/
#include "IDEVICE.h"

#include "header/GPIO.h"
#include "header/LED.h"
#include "header/SENSOR.h"
#include "header/TIME.h"
#include "header/HWIO.h"
#include "header/WIFI.h"

#include "cpp/GPIO.cpp"
#include "cpp/LED.cpp"
#include "cpp/SENSOR.cpp"
#include "cpp/TIME.cpp"
#include "cpp/HWIO.cpp"
#include "cpp/WIFI.cpp"

struct SystemComponents {
    // Data Handling Classes
    IDATA          _IData;
    ISYSTEM        _ISystem;

    // Hardware Classes
    GPIO_class     _GPIO;
    LED_class      _LED;
    SENSOR_class   _SENSOR;
    TIME_class     _TIME;
    HWIO_class     _HWIO;
    WIFI_class     _WIFI;
};

/** System Handler Library **/
#include "header/SYSTEM.h"
#include "cpp/SYSTEM.cpp"

#endif
