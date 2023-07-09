#ifndef BMP280_HPP
#define BMP280_HPP

#include "hwlib.hpp"

#define BMP280_CHIP_ID 0xD0
#define BMP280_SOFT_RESET 0xE0
#define BMP280_STATUS 0xF3
#define BMP280_CTRL_MEAS 0xF4
#define BMP280_CONFIG 0xF5
#define BMP280_PRESSURE_DATA 0xF7
#define BMP280_TEMP_DATA 0xFA


class bmp280 {

    //Calibration data addresses
    typedef struct {
        uint16_t    dig_T1; 
        int16_t     dig_T2;  
        int16_t     dig_T3;  

        uint16_t    dig_P1; 
        int16_t     dig_P2;  
        int16_t     dig_P3;  
        int16_t     dig_P4;  
        int16_t     dig_P5;  
        int16_t     dig_P6;  
        int16_t     dig_P7;  
        int16_t     dig_P8;  
        int16_t     dig_P9;
    } bmp280_calibration_data;

private:

    hwlib::i2c_bus & bus;
    uint8_t i2c_address;
    

    
    // Methods to compensate the reading according to the calibration data
    int32_t compensateTemperature();
    uint32_t compensatePressure();
    
public:

    // Constructor
    bmp280 ( hwlib::i2c_bus & bus, uint8_t i2c_address );
    
    // Methods to read and write registers
    // Move to private after debug!
    uint8_t  read8    ( uint8_t reg );
    uint16_t read16   ( uint8_t reg );
    void     write    ( uint8_t reg, uint8_t data );
    
    // Methods for configuring the sensor
    void loadCalibration();
    void setPowerMode(uint8_t mode);
    void setOversampling(uint8_t osrs_t, uint8_t osrs_p);
    
    // Methods to read the temperature either raw or compensated
    float readTemperatureRaw();
    float readTemperature();
    
    // Methods to read the pressure either raw or compensated
    float readPressureRaw();
    float readPressure();
    
};
    
#endif // BMP280_HPP