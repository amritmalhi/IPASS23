#include "BMP280.h"

// Constructor
BMP280::BMP280(hwlib::i2c_bus & bus, uint8_t address) : bus(bus), address(address) {
    
}

// Read register
uint8_t BMP280::readRegister(uint8_t reg) {
    uint8_t data = reg;
    bus.write(address).write(&data, 1);
    bus.read(address).read(&data, 1);
    return data;
}

// Write register
void BMP280::writeRegister(uint8_t reg, uint8_t data) {
    uint8_t buffer[2] = {reg, data};
    bus.write(address).write(buffer, 2);
}
