/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "setup.hpp"

SystemComponents class_lib;
SYSTEM_class Smart_Clock(class_lib);

int main()
{
    stdio_init_all();
    Smart_Clock.Initialize();

    multicore_launch_core1(core1_rtos_process);
    Smart_Clock.Run();

    return 0;
}

void core1_rtos_process()
{
    Smart_Clock.RunRTOS(); 
}
