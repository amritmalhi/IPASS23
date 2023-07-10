#ifndef BMP280_HPP
#define BMP280_HPP

#include "hwlib.hpp"

// Oversampling settings for osrs_p or osrs_t
// See Chapter 3.3.1 and 3.3.2 of the datasheet
enum sampling_config {
    SAMPLING_NONE = 0x00,
    SAMPLING_X1 = 0x01,
    SAMPLING_X2 = 0x02,
    SAMPLING_X4 = 0x03,
    SAMPLING_X8 = 0x04,
    SAMPLING_X16 = 0x05
};

// IIR filter settings
// See Chapter 3.3.3 of the datasheet
enum filter_config {
    FILTER_OFF = 0x00,
    FILTER_X2 = 0x01,
    FILTER_X4 = 0x02,
    FILTER_X8 = 0x03,
    FILTER_X16 = 0x04
};
    
// Available power modes
// See Chapter 3.6 of the datasheet
enum power_modes {
    SLEEP_MODE = 0x00,
    FORCED_MODE = 0x02,
    NORMAL_MODE = 0x03
};
    
// Standby time settings for in normal mode
// See Chapter 3.6.3 of the datasheet
enum standby_config {
    STANDBY_MS_1 = 0x00,
    STANDBY_MS_63 = 0x01,
    STANDBY_MS_125 = 0x02,
    STANDBY_MS_250 = 0x03,
    STANDBY_MS_500 = 0x04,
    STANDBY_MS_1000 = 0x05,
    STANDBY_MS_2000 = 0x06,
    STANDBY_MS_4000 = 0x07
};

// Register addresses containing calibration data
// See Chapter 3.11.2 of the datasheet
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
// See Chapter 4 of the datasheet
constexpr uint8_t BMP280_CHIP_ID_REG = 0xD0;
constexpr uint8_t BMP280_RESET_REG = 0xE0;
constexpr uint8_t BMP280_STATUS_REG = 0xF3;
constexpr uint8_t BMP280_CTRL_REG = 0xF4;
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
        
    // Methods to read and write from/to the sensor
    uint8_t read8(uint8_t reg);
    int16_t read16s(uint8_t reg);
    uint16_t read16u(uint8_t reg);
    void write(uint8_t reg, uint8_t data);

    int32_t compensateTemperature();
    uint32_t compensatePressure();

public:
    
    // Constructor
    bmp280(hwlib::i2c_bus& bus, uint8_t i2c_address = 0x76);

    // Methods used to configure the sensor
    void loadCalibration();
    void setPowerMode(power_modes mode);
    void setOversampling(sampling_config osrs_t, sampling_config osrs_p);

    // Methods to read the temperature either raw or compensated
    float readTemperatureRaw();
    float readTemperature();

    // Methods to read the pressure either raw or compensated
    float readPressureRaw();
    float readPressure();

    // Methods to print data from various registers
    void printIDRegister();
    void printResetRegister();
    void printCalibrationData();
    void printPowerMode();
    void printOversamplingSettings();
    void printDebug();

};

#endif // BMP280_HPP
