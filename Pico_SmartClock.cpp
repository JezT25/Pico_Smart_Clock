/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "bin/setup.hpp"

int main()
{
    stdio_init_all();

    SystemComponents class_lib;
    SYSTEM_class Smart_Clock(class_lib);

    Smart_Clock.Initialize();

    while (true)
    {
        Smart_Clock.Run();
    }

    return 0;
}


//TODO: USE MULTICORE !!!!!!!! core 1 rtos core 2 bare metal
// todo: handle and show if wifi not connected?
// button polishing

// set time and mode via wifi
// time interval every minute

//movelwipopts somewhere?