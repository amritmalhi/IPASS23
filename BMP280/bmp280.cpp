#include "bmp280.hpp"

// Constructor
// The i2c address is 0x76 if the 'SDO' pin is connected to GND (ground)
// Refer Chapter 5.2 and 6 of the datasheet for more information
bmp280::bmp280(hwlib::i2c_bus & bus, uint8_t i2c_address ) : bus(bus), i2c_address(i2c_address) {
    loadCalibration();
}

// Read 8 bits from a register
uint8_t bmp280::read8 ( uint8_t reg ) {
    bus.write(i2c_address).write(reg);
    uint8_t result;
    bus.read(i2c_address).read(&result, 1);
    return result;
}

// Read 16 bits and return them as a signed 16 bit integer
int16_t bmp280::read16s( uint8_t reg ){
  bus.write( i2c_address ).write( reg );
  auto transaction = bus.read( i2c_address );
  auto byte1 = transaction.read_byte();
  auto byte2 = transaction.read_byte();
  return static_cast<int16_t>((byte1 << 8) | byte2);
}

// Read 16 bits and return them as an unsigned 16 bit integer
uint16_t bmp280::read16u( uint8_t reg ){
  bus.write( i2c_address ).write( reg );
  auto transaction = bus.read( i2c_address );
  auto byte1 = transaction.read_byte();
  auto byte2 = transaction.read_byte();
  return static_cast<uint16_t>((byte1 << 8) | byte2);
}

// Write to a register
void bmp280::write (uint8_t reg, uint8_t data) {
    uint8_t buffer[2] = {reg, data};
    bus.write(i2c_address).write(buffer, 2);
}

// Read and store calibration data
void bmp280::loadCalibration() {
    calibration_data.dig_T1 = read16u(BMP280_DIG_T1_REG);
    calibration_data.dig_T2 = read16s(BMP280_DIG_T2_REG);
    calibration_data.dig_T3 = read16s(BMP280_DIG_T3_REG);

    calibration_data.dig_P1 = read16u(BMP280_DIG_P1_REG);
    calibration_data.dig_P2 = read16s(BMP280_DIG_P2_REG);
    calibration_data.dig_P3 = read16s(BMP280_DIG_P3_REG);
    calibration_data.dig_P4 = read16s(BMP280_DIG_P4_REG);
    calibration_data.dig_P5 = read16s(BMP280_DIG_P5_REG);
    calibration_data.dig_P6 = read16s(BMP280_DIG_P6_REG);
    calibration_data.dig_P7 = read16s(BMP280_DIG_P7_REG);
    calibration_data.dig_P8 = read16s(BMP280_DIG_P8_REG);
    calibration_data.dig_P9 = read16s(BMP280_DIG_P9_REG);
}

void bmp280::printCalibrationData() {
    hwlib::cout << "Calibration Data:" << hwlib::endl;
    hwlib::cout << "dig_T1: " << calibration_data.dig_T1 << hwlib::endl;
    hwlib::cout << "dig_T2: " << calibration_data.dig_T2 << hwlib::endl;
    hwlib::cout << "dig_T3: " << calibration_data.dig_T3 << hwlib::endl;
    hwlib::cout << "dig_P1: " << calibration_data.dig_P1 << hwlib::endl;
    hwlib::cout << "dig_P2: " << calibration_data.dig_P2 << hwlib::endl;
    hwlib::cout << "dig_P3: " << calibration_data.dig_P3 << hwlib::endl;
    hwlib::cout << "dig_P4: " << calibration_data.dig_P4 << hwlib::endl;
    hwlib::cout << "dig_P5: " << calibration_data.dig_P5 << hwlib::endl;
    hwlib::cout << "dig_P6: " << calibration_data.dig_P6 << hwlib::endl;
    hwlib::cout << "dig_P7: " << calibration_data.dig_P7 << hwlib::endl;
    hwlib::cout << "dig_P8: " << calibration_data.dig_P8 << hwlib::endl;
    hwlib::cout << "dig_P9: " << calibration_data.dig_P9 << hwlib::endl << hwlib::endl;
}

void bmp280::printIDRegister() {
    uint8_t id = read8(BMP280_CHIP_ID_REG);
    
    hwlib::cout << "ID Register:" << hwlib::endl;
    hwlib::cout << "Hexadecimal: 0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << id << hwlib::endl;
    hwlib::cout << "Decimal: " << hwlib::dec << id << hwlib::endl << hwlib::endl;
}

void bmp280::printResetRegister() {
    uint8_t reset = read8(BMP280_SOFT_RESET_REG);
    
    hwlib::cout << "Reset Register:" << hwlib::endl;
    hwlib::cout << "Hexadecimal: 0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << reset << hwlib::endl;
    hwlib::cout << "Decimal: " << hwlib::dec << reset << hwlib::endl << hwlib::endl;
}
