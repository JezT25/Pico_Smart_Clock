/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "../setup.hpp"

void SENSOR_class::Initialize()
{
    // Do a software reset
    uint8_t buf[2] = { REG_RESET, CMD_RESET };
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // Get ID
    uint8_t id;
    buf[0] = REG_ID;
    i2c_write_blocking(i2c_default, ADDR, buf, 1, true);
    i2c_read_blocking(i2c_default, ADDR, &id, 1, false);

    // Setting Configs for Sampling Rate, IIR Filter and SPI
    const uint8_t t_sb      = T_SB << 5;
    const uint8_t filter    = IIR_FILTER << 2;
    const uint8_t spi3w_en  = SPI3W_EN;
    uint8_t config_cmd = t_sb | filter | spi3w_en;
    buf[0] = REG_CONFIG;
    buf[1] = config_cmd;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // Set Oversampling Rates for Humidity
    buf[0] = REG_CTRL_H;
    buf[1] = OSRS_H;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // Set Oversampling Rates for Temperature and Pressure then set to Normal mode
    const uint8_t osrs_t = OSRS_T << 5;
    const uint8_t osrs_p = OSRS_P << 2;
    uint8_t config_cmd_ctrl = osrs_t | osrs_p | NORMAL_MODE;
    buf[0] = REG_CTRL;
    buf[1] = config_cmd_ctrl;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    sleep_ms(1000); // Sleep to avoid collision

    fetch_compensation_data();
}

void SENSOR_class::getSensorData(IDATA *IData)
{
    // Get raw measurements and put them in raw_measurements
    uint8_t raw_buffer[8];
    uint8_t buf[1] = { REG_DATA };                                                            // Read from 0xF7 all the way to 0xFE
    i2c_write_blocking(i2c_default, ADDR, buf, 1, true);
    i2c_read_blocking(i2c_default, ADDR, raw_buffer, 8, false);

    raw_pressure = (raw_buffer[0] << 12) | (raw_buffer[1] << 4) | (raw_buffer[2] >> 4);       // Pressure is MSB LSB XLSB(7:4)
    raw_temperature = (raw_buffer[3] << 12) | (raw_buffer[4] << 4) | (raw_buffer[5] >> 4);    // Temperature is MSB LSB XLSB(7:4)
    raw_humidity = (raw_buffer[6] << 8) | raw_buffer[7];                                      // Humidity is MSB LSB

    IData->SENSOR_TEMP = compensate_temperature(raw_temperature);
    IData->SENSOR_PRESSURE = compensate_pressure(raw_pressure);
    IData->SENSOR_HUMIDITY = compensate_humidity(raw_humidity);

    // Uncomment for debugging
    // printf("Pressure    = %.3f kPa\n", IData->SENSOR_PRESSURE);
    // printf("Temperature = %.2f C\n", IData->SENSOR_TEMP);
    // printf("Humidity    = %.3f %%RH\n\n", IData->SENSOR_HUMIDITY);
}

double SENSOR_class::compensate_temperature(uint32_t raw_temperature)
{
    double var1 = (((double)raw_temperature) / 16384.0 - ((double)_bme_comp_coeffs.dig_T1) / 1024.0);
    var1 = var1 * ((double)_bme_comp_coeffs.dig_T2);
    double var2 = (((double)raw_temperature) / 131072.0 - ((double)_bme_comp_coeffs.dig_T1) / 8192.0);
    var2 = (var2 * var2) * ((double)_bme_comp_coeffs.dig_T3);
    _bme_comp_coeffs.t_fine = (int32_t)(var1 + var2);
    double temperature = (var1 + var2) / 5120.0;
    // if (temperature < -40.0) temperature = -40.0;                                        // Due to limited display area, the negative limit will be -9.9
    if (temperature < -9.9) temperature = -9.9;
    else if (temperature > 85.0) temperature = 85.0;
    return temperature;
}

double SENSOR_class::compensate_pressure(uint32_t raw_pressure)
{
    double var1 = ((double)_bme_comp_coeffs.t_fine / 2.0) - 64000.0;
    double var2 = var1 * var1 * ((double)_bme_comp_coeffs.dig_P6) / 32768.0;
    var2 = var2 * var1 * ((double)_bme_comp_coeffs.dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)_bme_comp_coeffs.dig_P4) * 65536.0);
    double var3 = ((double)_bme_comp_coeffs.dig_P3) * var1 * var1 / 524288.0;
    var1 = (var3 + ((double)_bme_comp_coeffs.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)_bme_comp_coeffs.dig_P1);

    double pressure = 0.0;
    if (var1 > 0.0)
    {
        pressure = 1048576.0 - (double)raw_pressure;
        pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
        var1 = ((double)_bme_comp_coeffs.dig_P9) * pressure * pressure / 2147483648.0;
        var2 = pressure * ((double)_bme_comp_coeffs.dig_P8) / 32768.0;
        pressure = pressure + (var1 + var2 + ((double)_bme_comp_coeffs.dig_P7)) / 16.0;
        if (pressure < 30000.0) pressure = 30000.0;
        else if (pressure > 1100000.0) pressure = 1100000.0;
    }
    else
    {
        pressure = 30000.0;
    }

    return pressure / 1000; // Divide by 1000 to get kPa
}

double SENSOR_class::compensate_humidity(uint32_t raw_humidity)
{
    double var1 = ((double)_bme_comp_coeffs.t_fine) - 76800.0;
    double var2 = (((double)_bme_comp_coeffs.dig_H4)* 64.0 + (((double)_bme_comp_coeffs.dig_H5) / 16384.0) * var1);
    double var3 = raw_humidity - var2;
    double var4 = ((double)_bme_comp_coeffs.dig_H2) / 65536.0;
    double var5 = (1.0 + (((double)_bme_comp_coeffs.dig_H3) / 67108864.0) * var1);
    double var6 = 1.0 + (((double)_bme_comp_coeffs.dig_H6) / 67108864.0) * var1 * var5;
    var6 = var3 * var4 * var5 * var6;
    double humidity = var6 * (1.0 - ((double)_bme_comp_coeffs.dig_H1) * var6 / 524288.0);

    if (humidity > 100.0) humidity = 100.0;
    else if (humidity < 0.0) humidity = 0.0;
    return humidity;
}

bool SENSOR_class::fetch_compensation_data() {
    uint8_t buffer[33] = {0};

    // Two reads because the calibration/compensation registers are not aligned
    uint8_t compensation_reg_first = 0x88; // Starting address of first bank
    uint8_t compensation_reg_second = 0xE1; // Starting address of second bank

    // Send which register we wish to start reading from, stop if write unsuccessful
    if (i2c_write_timeout_us(i2c_default, ADDR, &compensation_reg_first, 1, false, 1000) != 1) return false;
    // Read 26 bytes from the BME280
    if (i2c_read_timeout_us(i2c_default, ADDR, buffer, 26, false, 10000) != 26) return false;

    // Repeat for second register bank
    if (i2c_write_timeout_us(i2c_default, ADDR, &compensation_reg_second, 1, false, 10000) != 1) return false;
    if (i2c_read_timeout_us(i2c_default, ADDR, buffer + 26, 7, false, 10000) != 7) return false;

    memset(&_bme_comp_coeffs, 0, sizeof(_bme_comp_coeffs));

    _bme_comp_coeffs.dig_T1 = ((uint16_t)buffer[1] << 8) | (uint16_t)buffer[0];
    _bme_comp_coeffs.dig_T2 = uint16_t_to_int16_t(((uint16_t)buffer[3] << 8) | (uint16_t)buffer[2]);
    _bme_comp_coeffs.dig_T3 = uint16_t_to_int16_t(((uint16_t)buffer[5] << 8) | (uint16_t)buffer[4]);
    _bme_comp_coeffs.dig_P1 = ((uint16_t)buffer[7] << 8) | (uint16_t)buffer[6];
    _bme_comp_coeffs.dig_P2 = uint16_t_to_int16_t(((uint16_t)buffer[9] << 8) | (uint16_t)buffer[8]);
    _bme_comp_coeffs.dig_P3 = uint16_t_to_int16_t(((uint16_t)buffer[11] << 8) | (uint16_t)buffer[10]);
    _bme_comp_coeffs.dig_P4 = uint16_t_to_int16_t(((uint16_t)buffer[13] << 8) | (uint16_t)buffer[12]);
    _bme_comp_coeffs.dig_P5 = uint16_t_to_int16_t(((uint16_t)buffer[15] << 8) | (uint16_t)buffer[14]);
    _bme_comp_coeffs.dig_P6 = uint16_t_to_int16_t(((uint16_t)buffer[17] << 8) | (uint16_t)buffer[16]);
    _bme_comp_coeffs.dig_P7 = uint16_t_to_int16_t(((uint16_t)buffer[19] << 8) | (uint16_t)buffer[18]);
    _bme_comp_coeffs.dig_P8 = uint16_t_to_int16_t(((uint16_t)buffer[21] << 8) | (uint16_t)buffer[20]);
    _bme_comp_coeffs.dig_P9 = uint16_t_to_int16_t(((uint16_t)buffer[23] << 8) | (uint16_t)buffer[22]);
    _bme_comp_coeffs.dig_H1 = buffer[25];
    _bme_comp_coeffs.dig_H2 = uint16_t_to_int16_t(((uint16_t)buffer[27] << 8) | (uint16_t)buffer[26]);
    _bme_comp_coeffs.dig_H3 = buffer[28];
    _bme_comp_coeffs.dig_H4 = uint16_t_to_int16_t(((uint16_t)buffer[29] << 4) | (uint16_t)buffer[30] & 0x0F);
    _bme_comp_coeffs.dig_H5 = uint16_t_to_int16_t((((uint16_t)buffer[30] & 0xF0 ) >> 4) | ((uint16_t)buffer[31] << 4));
    _bme_comp_coeffs.dig_H6 = uint8_t_to_int8_t(buffer[32]);

    return true;
}

inline int8_t SENSOR_class::uint8_t_to_int8_t (uint8_t in)
{
    return *(int8_t *)&in;
}

inline int16_t SENSOR_class::uint16_t_to_int16_t (uint16_t in)
{
    return *(int16_t *)&in;
}
