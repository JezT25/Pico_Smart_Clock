/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef HWIO_h
#define HWIO_h

#include "../setup.hpp"

#define DEBOUNCE_MS			150
#define BUTTON_MIN_PRESS 	30
#define BUTTON_LONG_PRESS 	2000

class HWIO_class : private HARDWARE_class {
    private:
		static IDATA *_IData;
		static ISYSTEM *_ISystem;
		static bool modeButton_ispressed;
		static uint32_t modeButton_lpt;

		static void modeButton_Function(uint gpio, uint32_t events);

    public:
		void Initialize(IDATA *IData, ISYSTEM *ISystem);
};

#endif
