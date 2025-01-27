/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void SYSTEM_class::Initialize()
{
    // Initialize Pico Hardware
    _HARDWARE.Initialize();

    // Initialize BME280 Sensor
    _SENSOR.Initialize();

    // Initialize External Hardware IO
    _HWIO.Initialize();

    // Initialize Clock Interrupts
    add_alarm_in_ms(ISR_START_ON_MS, DisplayISR, this, false);
    add_alarm_in_ms(ISR_START_ON_MS, ClockISR, this, false);
}

void SYSTEM_class::Run()
{
    system_modeHandler();

    _LED.updateBuffer(_IData, _ISystem);
    _LED.toggleDot_Cleaner(_ISystem);
    _HWIO.alarmHandler(_IData, &_ISystem);
    _HWIO.stopBuzzer();
}

// Interrupt Routine [200Hz]
long long int SYSTEM_class::DisplayISR(alarm_id_t id, void* user_data)
{
    SYSTEM_class* system = static_cast<SYSTEM_class*>(user_data);
    system->_LED.displayDigits(system->_ISystem);
    return DISPLAY_REFRESH_US;
}

// Interrupt Routine [2Hz]
long long int SYSTEM_class::ClockISR(alarm_id_t id, void* user_data)
{
    SYSTEM_class* system = static_cast<SYSTEM_class*>(user_data);
    system->_LED.toggleDot(system->_ISystem);
    system->_TIME.getTime(&system->_IData);
    system->_SENSOR.getSensorData(&system->_IData);
    return TIME_REFRESH_US;
}

void SYSTEM_class::system_modeHandler()
{
    // Turn off alarm with any button
    if(_HWIO.button_flag != _HWIO.NO_FLAG && _HWIO.alarm_isRinging == ALARM_ON)
    {
        _HWIO.alarm_isRinging = ALARM_OFF;
        _HWIO.alarm_timeout = to_ms_since_boot(get_absolute_time()) + ALARM_TIMEOUT;
        _HWIO.button_flag = _HWIO.NO_FLAG;
        return;
    }

    switch(_HWIO.button_flag)
    {
        case _HWIO.MODE_BUTTON:
            _HWIO.playBuzzer(TONE_LOW, BEEP_SHORT);
            switch (_ISystem.SYSTEM_MODE)
            {
                case _ISystem.ALARM_ADJUST_MODE:
                    _LED.currentSection_blink == LEFT_BLINK ? (_IData.ADJUST_ALARM_HOUR = _IData.ADJUST_ALARM_HOUR == 0 ? 23 : --_IData.ADJUST_ALARM_HOUR) : (_IData.ADJUST_ALARM_MINUTE = _IData.ADJUST_ALARM_MINUTE == 0 ? 59 : --_IData.ADJUST_ALARM_MINUTE);
                    break;
                case _ISystem.CLOCK_ADJUST_MODE:
                    _LED.currentSection_blink == LEFT_BLINK ? (_IData.ADJUST_HOUR = _IData.ADJUST_HOUR == 0 ? 23 : --_IData.ADJUST_HOUR) : (_IData.ADJUST_MINUTE = _IData.ADJUST_MINUTE == 0 ? 59 : --_IData.ADJUST_MINUTE);
                    break;
                case _ISystem.DATE_ADJUST_MODE:
                    _LED.currentSection_blink == LEFT_BLINK ? (_IData.ADJUST_DAY = _IData.ADJUST_DAY == 1 ? 31 : --_IData.ADJUST_DAY) : (_IData.ADJUST_MONTH = _IData.ADJUST_MONTH == 1 ? 12 : --_IData.ADJUST_MONTH);
                    break;
                case _ISystem.YEAR_ADJUST_MODE:
                    _IData.ADJUST_YEAR = _IData.ADJUST_YEAR == 0 ? 99 : --_IData.ADJUST_YEAR;
                    break;
                default:
                    _ISystem.SYSTEM_MODE = (_ISystem.SYSTEM_MODE == 0) ? MODE_COUNT - 1 : --_ISystem.SYSTEM_MODE;
            }
            break;
        case _HWIO.SELECT_BUTTON:
            _HWIO.playBuzzer(TONE_LOW, BEEP_SHORT);
            switch (_ISystem.SYSTEM_MODE)
            {
                case _ISystem.ALARM_ADJUST_MODE:
                    _LED.currentSection_blink == LEFT_BLINK ? (_IData.ADJUST_ALARM_HOUR = _IData.ADJUST_ALARM_HOUR == 23 ? 0 : ++_IData.ADJUST_ALARM_HOUR) : (_IData.ADJUST_ALARM_MINUTE = _IData.ADJUST_ALARM_MINUTE == 59 ? 0 : ++_IData.ADJUST_ALARM_MINUTE);
                    break;
                case _ISystem.CLOCK_ADJUST_MODE:
                    _LED.currentSection_blink == LEFT_BLINK ? (_IData.ADJUST_HOUR = _IData.ADJUST_HOUR == 23 ? 0 : ++_IData.ADJUST_HOUR) : (_IData.ADJUST_MINUTE = _IData.ADJUST_MINUTE == 59 ? 0 : ++_IData.ADJUST_MINUTE);
                    break;
                case _ISystem.DATE_ADJUST_MODE:
                    _LED.currentSection_blink == LEFT_BLINK ? (_IData.ADJUST_DAY = _IData.ADJUST_DAY == 31 ? 1 : ++_IData.ADJUST_DAY) : (_IData.ADJUST_MONTH = _IData.ADJUST_MONTH == 12 ? 1 : ++_IData.ADJUST_MONTH);
                    break;
                case _ISystem.YEAR_ADJUST_MODE:
                    _IData.ADJUST_YEAR = _IData.ADJUST_YEAR == 99 ? 0 : ++_IData.ADJUST_YEAR;
                    break;
                default:
                    _ISystem.SYSTEM_MODE = (_ISystem.SYSTEM_MODE == MODE_COUNT - 1) ? _ISystem.ALARM_MODE : ++_ISystem.SYSTEM_MODE;
            }
            break;
        case _HWIO.SHORT_COMBO_BUTTON:
            switch (_ISystem.SYSTEM_MODE)
            {
                case _ISystem.ALARM_ADJUST_MODE:
                case _ISystem.CLOCK_ADJUST_MODE:
                case _ISystem.DATE_ADJUST_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_SHORT);
                    _LED.currentSection_blink ^= (RIGHT_BLINK ^ LEFT_BLINK);
                    break;
                case _ISystem.ALARM_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_SHORT);
                    _ISystem.ALARM_STATE = !_ISystem.ALARM_STATE;
                    break;
            }
            break;
        case _HWIO.COMBO_BUTTON:
            switch (_ISystem.SYSTEM_MODE)
            {
                case _ISystem.ALARM_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _IData.ADJUST_ALARM_HOUR = _IData.ALARM_HOUR;
                    _IData.ADJUST_ALARM_MINUTE = _IData.ALARM_MINUTE;
                    _LED.currentSection_blink = RIGHT_BLINK;
                    _ISystem.SYSTEM_MODE = _ISystem.ALARM_ADJUST_MODE;
                    break;
                case _ISystem.ALARM_ADJUST_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _TIME.setAlarm(&_IData);
                    _ISystem.SYSTEM_MODE = _ISystem.ALARM_MODE;
                    break;
                case _ISystem.CLOCK_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _IData.ADJUST_HOUR = _IData.CLOCK_HOUR;
                    _IData.ADJUST_MINUTE = _IData.CLOCK_MINUTE;
                    _LED.currentSection_blink = RIGHT_BLINK;
                    _ISystem.SYSTEM_MODE = _ISystem.CLOCK_ADJUST_MODE;
                    break;
                case _ISystem.CLOCK_ADJUST_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _TIME.setTime(&_IData);
                    _ISystem.SYSTEM_MODE = _ISystem.CLOCK_MODE;
                    break;
                case _ISystem.DATE_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _IData.ADJUST_DAY = _IData.CLOCK_DAY;
                    _IData.ADJUST_MONTH = _IData.CLOCK_MONTH;
                    _LED.currentSection_blink = RIGHT_BLINK;
                    _ISystem.SYSTEM_MODE = _ISystem.DATE_ADJUST_MODE;
                    break;
                case _ISystem.DATE_ADJUST_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _TIME.setDate(&_IData);
                    _ISystem.SYSTEM_MODE = _ISystem.DATE_MODE;
                    break;
                case _ISystem.YEAR_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _IData.ADJUST_YEAR = _IData.CLOCK_YEAR;
                    _LED.currentSection_blink = RIGHT_BLINK;
                    _ISystem.SYSTEM_MODE = _ISystem.YEAR_ADJUST_MODE;
                    break;
                case _ISystem.YEAR_ADJUST_MODE:
                    _HWIO.playBuzzer(TONE_LOW, BEEP_LONG);
                    _TIME.setYear(&_IData);
                    _ISystem.SYSTEM_MODE = _ISystem.YEAR_MODE;
                    break;
            }
            break; 
    }
    _HWIO.button_flag = _HWIO.NO_FLAG;
}