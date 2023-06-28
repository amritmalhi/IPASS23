#ifndef BMP280_HPP
#define BMP280_HPP

#include "hwlib.hpp"

class BMP280 {
public:

    // Constructor
    BMP280(hwlib::i2c_bus & bus, uint8_t address);

    // Methods to read and write registers
    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t data);

private:

    hwlib::i2c_bus & bus;
    uint8_t address;

};

#endif // BMP280_HPP