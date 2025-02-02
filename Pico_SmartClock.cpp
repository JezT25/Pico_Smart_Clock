/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "setup.hpp"

SystemComponents class_lib;
SYSTEM_class Smart_Clock(class_lib);

void core1_rtos_process();
int main()
{
    stdio_init_all();
    Smart_Clock.Initialize();

    multicore_launch_core1(core1_rtos_process); // RunBare Metal in core1
    Smart_Clock.RunRTOS();                      // Run FreeRTOS in core0

    return 0;
}

void core1_rtos_process()
{
    while(1) Smart_Clock.Run();
}

// todo: handle and show if wifi not connected?
// button polishing
//------------------------------------------
// set time and mode via wifi