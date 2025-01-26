/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef HWIO_h
#define HWIO_h

#include "../setup.hpp"

#define DEBOUNCE_MS			100
#define BUTTON_MIN_PRESS 	30
#define BUTTON_LONG_PRESS 	1500

#define BEEP_SHORT          50
#define BEEP_MED            100
#define BEEP_LONG           300
#define BEEP_EX_LONG        1000
#define BEEP_EXX_LONG       3000
#define TONE_LOW            800
#define TONE_HIGH           1000

class HWIO_class : private HARDWARE_class {
    private:
		static volatile IDATA *_IData;
		static volatile ISYSTEM *_ISystem;
		static volatile int buzzer_duration;
		static volatile bool modeButton_ispressed;
		static volatile bool selectButton_ispressed;
		static volatile uint32_t buzzer_lpt;
		static volatile uint32_t modeButton_lpt;
		static volatile uint32_t selectButton_lpt;

		static void button_Function(uint gpio, uint32_t events);
		static void modeFunction(uint32_t current_time, uint32_t events);
		static void selectFunction(uint32_t current_time, uint32_t events);

    public:
		void Initialize(IDATA *IData, ISYSTEM *ISystem);
		static void playBuzzer(int frequency, int duration);
		static inline void stopBuzzer();
};

#endif
