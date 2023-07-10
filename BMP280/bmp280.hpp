#ifndef BMP280_HPP
#define BMP280_HPP

#include "hwlib.hpp"
#include "bmp280_defs.hpp"

/**
 * @class bmp280
 * @brief Class for interacting with the BMP280 sensor.
 *
 * This class provides methods to configure the BMP280 sensor, read raw data, compensate data,
 * and print register values.
 * 
 * Took inspiration from:
 * Bosch's BMP280 API:     https://github.com/BoschSensortec/BMP2-Sensor-API/ \
 * wovo's example code:    https://github.com/wovo/hwlib-examples/tree/master/arduino-due/due-%230210-dmp280 \
 * basvandenbergh: https:  https://github.com/BasvandenBergh/IPASS_jaar1_BAS \
 * I was not able to get the calibration of raw temperature/pressure data to work with my own read and write methods, so I had to borrow Bas'
 **/
class bmp280 {

private:
    hwlib::i2c_bus& bus; /**< hwlib i2c bus for communication with the sensor */
    uint8_t i2c_address; /**< Stores the BMP280's i2c slave address */

    bmp280_calibration_data calibration_data; /**< Struct to hold calibration data */

    /**
     * @brief Write data to the sensor.
     * @param data The data to be written.
     */
    void write(uint8_t data);

    /**
     * @brief Read data from the sensor.
     * @param data Pointer to the buffer where the data will be stored.
     * @param data_size The size of the data to be read.
     */
    void read(uint8_t* data, int data_size);

    /**
     * @brief Read an 8-bit value from the sensor.
     * @param reg The register address to read from.
     * @return The 8-bit value read from the register.
     */
    uint8_t read8(uint8_t reg);

    /**
     * @brief Read a signed 16-bit value from the sensor.
     * @param reg The register address to read from.
     * @return The signed 16-bit value read from the register.
     */
    int16_t read16s(uint8_t reg);

    /**
     * @brief Read an unsigned 16-bit value from the sensor.
     * @param reg The register address to read from.
     * @return The unsigned 16-bit value read from the register.
     */
    uint16_t read16u(uint8_t reg);

    /**
     * @brief Read a 20-bit unsigned value from the sensor.
     * @param reg The register address to read from.
     * @return The 20-bit unsigned value read from the register.
     */
    uint32_t read20u(uint8_t reg);

    /**
     * @brief Load calibration data from the sensor.
     */
    void loadCalibration();
    
    /**
     * @brief Read the raw temperature data from the sensor.
     * @return The raw temperature data.
     */
    uint32_t readTemperatureRaw();

    /**
     * @brief Read the raw pressure data from the sensor.
     * @return The raw pressure data.
     */
    uint32_t readPressureRaw();

public:
    /**
     * @brief Constructor for the bmp280 class.
     * @param bus The hwlib i2c bus for communication with the sensor.
     * @param i2c_address The BMP280's i2c slave address. Default is 0x76.
     */
    bmp280(hwlib::i2c_bus& bus, uint8_t i2c_address = 0x76);

    /**
     * @brief Setup the sensor by configuring settings for your use case.
     */
    void setup();

    /**
     * @brief Set the power mode of the sensor.
     * @param mode The power mode to set.
     */
    void setPowerMode(power_modes mode);

    /**
     * @brief Set the oversampling settings for temperature and pressure measurements.
     * @param osrs_t The oversampling setting for temperature.
     * @param osrs_p The oversampling setting for pressure.
     */
    void setOversampling(sampling_config osrs_t, sampling_config osrs_p);

    /**
     * @brief Set the IIR filter coefficient for pressure and temperature measurements.
     * @param filter The filter coefficient to set.
     */
    void setFilter(filter_config filter);

    /**
     * @brief Compensate the temperature data and return the compensated value.
     * @return The compensated temperature value.
     */
    double getTemperature();

    /**
     * @brief Compensate the pressure data and return the compensated value.
     * @return The compensated pressure value.
     */
    double compensatePressure();

    /**
     * @brief Print the ID register value.
     */
    void printIDRegister();

    /**
     * @brief Print the reset register value.
     */
    void printResetRegister();

    /**
     * @brief Print the calibration data.
     */
    void printCalibrationData();

    /**
     * @brief Print the power mode setting.
     */
    void printPowerMode();

    /**
     * @brief Print the oversampling settings for temperature and pressure measurements.
     */
    void printOversamplingSettings();

    /**
     * @brief Print the filter settings.
     */
    void printFilterSettings();

    /**
     * @brief Print the raw temperature and pressure data.
     */
    void printRawData();

    /**
     * @brief Printthe compensated temperature and pressure data.
     */
    void printCompensatedData();

    /**
     * @brief Print debug information.
     */
    void printDebug();

    /**
    *   The following variables were written by Bas van den Bergh, a Computer Engineering student at HU.
    *   Source: https://github.com/BasvandenBergh/IPASS_jaar1_BAS
    **/
    uint8_t resultstemp[3];   /**< Buffer to store temperature results */
    uint8_t resultspress[3];  /**< Buffer to store pressure results */
    uint8_t digtemp[6];       /**< Buffer to store temperature calibration data */
    uint8_t digpress[18];     /**< Buffer to store pressure calibration data */
};

#endif // BMP280_HPP
