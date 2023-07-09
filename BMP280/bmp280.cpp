#include "bmp280.h"

// Constructor
bmp280::bmp280(hwlib::i2c_bus & bus, uint8_t i2c_address) : bus(bus), i2c_address(i2c_address) 
{}

// Read 8 bits from a register
uint8_t bmp280::read8( uint8_t reg ) {
    bus.write(i2c_address).write(reg);
    uint8_t result;
    bus.read(i2c_address).read(&result, 1);
    return result;
}


// Read 16 bits from a register
uint16_t bmp280::read16( uint8_t reg ) {
    bus.write(i2c_address).write(reg);
    auto transaction = bus.read( i2c_address );
    auto byte1 = transaction.read_byte();
    auto byte2 = transaction.read_byte();
    return ( byte1 << 8 ) | ( byte2 );
}

// Write register
void bmp280::write(uint8_t reg, uint8_t data) {
    uint8_t buffer[2] = {reg, data};
    bus.write(i2c_address).write(buffer, 2);
}
