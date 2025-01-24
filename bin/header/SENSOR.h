/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef SENSOR_h
#define SENSOR_h

#include "../setup.hpp"

#define ADDR        0x77

#define T_SB        0x05          // 1000ms sampling rate
#define IIR_FILTER  0x04          // IIR filter max
#define SPI3W_EN    0x00          // SPI off
#define OSRS_H      0x04          // Humidity Oversampling x 8
#define OSRS_T      0x04          // Temperature Oversampling x 8
#define OSRS_P      0x04          // Pressure Oversampling x 8
#define NORMAL_MODE 0x03          // Normal Mode

// Hardware Values
#define DEVICE_ID   0x60
#define CMD_RESET   0xB6

// Hardware Registers
#define REG_RESET   0xE0
#define REG_ID      0xD0
#define REG_CONFIG  0xF5
#define REG_CTRL    0xF4
#define REG_CTRL_H  0xF2
#define REG_DATA    0xF7

class SENSOR_class : private HARDWARE_class {
    private:
        struct
        {
            uint16_t    dig_T1, dig_P1;
            int16_t     dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, dig_H2, dig_H4, dig_H5;
            uint8_t     dig_H1, dig_H3;
            int8_t      dig_H6; 
            int32_t     t_fine; 
        } _bme_comp_coeffs;

        uint32_t raw_temperature;
        uint32_t raw_pressure;
        uint32_t raw_humidity;

        inline int8_t uint8_t_to_int8_t (uint8_t in);
        inline int16_t uint16_t_to_int16_t(uint16_t in);
        bool fetch_compensation_data();
        double compensate_temperature(uint32_t raw_temperature);
        double compensate_pressure(uint32_t raw_pressure);
        double compensate_humidity(uint32_t raw_humidity);

    public:
        void Initialize();
        void getSensorData(IDATA *IData);
};

#endif
