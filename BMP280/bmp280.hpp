#ifndef BMP280_HPP
#define BMP280_HPP

#include "hwlib.hpp"

// Register addresses containing calibration data
constexpr uint8_t BMP280_DIG_T1_REG = 0x88;
constexpr uint8_t BMP280_DIG_T2_REG = 0x8A;
constexpr uint8_t BMP280_DIG_T3_REG = 0x8C;
constexpr uint8_t BMP280_DIG_P1_REG = 0x8E;
constexpr uint8_t BMP280_DIG_P2_REG = 0x90;
constexpr uint8_t BMP280_DIG_P3_REG = 0x92;
constexpr uint8_t BMP280_DIG_P4_REG = 0x94;
constexpr uint8_t BMP280_DIG_P5_REG = 0x96;
constexpr uint8_t BMP280_DIG_P6_REG = 0x98;
constexpr uint8_t BMP280_DIG_P7_REG = 0x9A;
constexpr uint8_t BMP280_DIG_P8_REG = 0x9C;
constexpr uint8_t BMP280_DIG_P9_REG = 0x9E;

// Register addresses containing configurations and data
constexpr uint8_t BMP280_CHIP_ID_REG = 0xD0;
constexpr uint8_t BMP280_SOFT_RESET_REG = 0xE0;
constexpr uint8_t BMP280_STATUS_REG = 0xF3;
constexpr uint8_t BMP280_CTRL_MEAS_REG = 0xF4;
constexpr uint8_t BMP280_CONFIG_REG = 0xF5;
constexpr uint8_t BMP280_PRESS_DATA_REG = 0xF7;
constexpr uint8_t BMP280_TEMP_DATA_REG = 0xFA;

// Struct for hold calibration data
typedef struct {
    uint16_t dig_T1; 
    int16_t dig_T2;  
    int16_t dig_T3;  
    uint16_t dig_P1; 
    int16_t dig_P2;  
    int16_t dig_P3;  
    int16_t dig_P4;  
    int16_t dig_P5;  
    int16_t dig_P6;  
    int16_t dig_P7;  
    int16_t dig_P8;  
    int16_t dig_P9;
} bmp280_calibration_data;

class bmp280 {

private:
    hwlib::i2c_bus& bus; // hwlib i2c bus for communication with the sensor
    uint8_t i2c_address; // Stores the BMP280's i2c slave address
    bmp280_calibration_data calibration_data; // Struct to hold calibration data

    int32_t compensateTemperature();
    uint32_t compensatePressure();

public:
    // Constructor
    bmp280(hwlib::i2c_bus& bus, uint8_t i2c_address = 0x76);
    
    // Private methods, currently public for debug purposes
    uint8_t read8(uint8_t reg);
    int16_t read16s(uint8_t reg);
    uint16_t read16u(uint8_t reg);
    void write(uint8_t reg, uint8_t data);

    // Methods used to configure the sensor
    void loadCalibration();
    void setPowerMode(uint8_t mode);
    void setOversampling(uint8_t osrs_t, uint8_t osrs_p);

    // Methods to read the temperature either raw or compensated
    float readTemperatureRaw();
    float readTemperature();

    // Methods to read the pressure either raw or compensated
    float readPressureRaw();
    float readPressure();

    // Methods to print data from various registers
    void printCalibrationData();
    void printIDRegister();
    void printResetRegister();

};

#endif // BMP280_HPP
