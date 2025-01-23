/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef IDEVICE_h
#define IDEVICE_h

#define MOUNT_COUNT 6

#define CLOCK_MODE	0
#define DATE_MODE 	1
#define YEAR_MODE	2
#define TEMP_MODE	3
#define HUMI_MODE	4
#define PRES_MODE	5

class IDATA
{
    public:
        volatile uint8_t CLOCK_HOUR     =   0;
        
        volatile uint8_t CLOCK_MINUTE   =   0;

        volatile uint8_t CLOCK_SECOND   =   0;
        
        volatile uint8_t CLOCK_DAY      =   0;

        volatile uint8_t CLOCK_MONTH    =   0;
        
        volatile int CLOCK_YEAR         =   0;

        volatile float SENSOR_TEMP      =   0;
        
        volatile float SENSOR_HUMIDITY  =   0;

        volatile float SENSOR_PRESSURE  =   0;
};

class ISYSTEM
{
    public:
        volatile uint8_t SYSTEM_MODE    =   CLOCK_MODE;
};

#endif
