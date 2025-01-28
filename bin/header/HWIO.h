/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef HWIO_h
#define HWIO_h

#include "../setup.hpp"

#define ALARM_BEEP_PATTERN		3
#define ALARM_SHORT_INTERVAL	150
#define ALARM_LONG_INTERVAL 	800
#define ALARM_TIMEOUT			60000

#define DEBOUNCE_MS				100
#define BUTTON_MIN_PRESS 		30
#define BUTTON_LONG_PRESS 		1250

#define BEEP_SHORT          	50
#define BEEP_MED            	100
#define BEEP_LONG           	300
#define BEEP_EX_LONG        	1000
#define BEEP_EXX_LONG       	3000
#define TONE_LOW            	800
#define TONE_HIGH           	1000

class HWIO_class : private HARDWARE_class {
    private:
		uint8_t beepCount 		= 0;
		uint32_t alarmbeep_lpt	= 0;
		static volatile int buzzer_duration;
		static volatile bool modeButton_ispressed;
		static volatile bool selectButton_ispressed;
		static volatile uint32_t buzzer_lpt;
		static volatile uint32_t modeButton_lpt;
		static volatile uint32_t selectButton_lpt;

		static void button_Function(uint gpio, uint32_t events);

    public:
		enum Buttons : uint8_t
		{
			NO_FLAG,
			MODE_BUTTON,
			SELECT_BUTTON,
			SHORT_COMBO_BUTTON,
			COMBO_BUTTON
		};

		bool alarm_isRinging    = ALARM_OFF;
        uint32_t alarm_timeout  = ALARM_TIMEOUT;
		static volatile uint8_t button_flag;

		void Initialize();
		void alarmHandler(IDATA IData, ISYSTEM *ISystem);
		static void playBuzzer(int frequency, int duration);
		static inline void stopBuzzer();
};

#endif
