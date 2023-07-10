#include "bmp280.hpp"

// Constructor
// The i2c address is 0x76 if the 'SDO' pin is connected to GND (ground)
// Refer to Chapter 5.2 and 6 of the datasheet for more information
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

// Set the power mode to the desired mode
// See power_modes enum for available power modes.
void bmp280::setPowerMode(power_modes mode) {
    // Read the current value of the control register
    uint8_t ctrl_meas = read8(BMP280_CTRL_REG);

    // Clear the mode bits (bit 1 and bit 0)
    ctrl_meas &= ~(0b11);

    // Set the new power mode
    ctrl_meas |= static_cast<uint8_t>(mode);

    // Write the updated value back to the control register
    write(BMP280_CTRL_REG, ctrl_meas);
}

void bmp280::setOversampling(sampling_config osrs_t, sampling_config osrs_p) {
    // Read the current value of the control register
    uint8_t ctrl_meas = read8(BMP280_CTRL_REG);

    // Clear the osrs_t and osrs_p bits (bits 7:5 and 4:2)
    ctrl_meas &= ~(0b111 << 5);
    ctrl_meas &= ~(0b111 << 2);

    // Set the new oversampling settings
    ctrl_meas |= static_cast<uint8_t>(osrs_t) << 5;
    ctrl_meas |= static_cast<uint8_t>(osrs_p) << 2;

    // Write the updated value back to the control register
    write(BMP280_CTRL_REG, ctrl_meas);
}


void bmp280::printCalibrationData() {
    hwlib::cout << "Calibration Data:" << hwlib::dec << hwlib::endl;
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

// Must be 0x58 when read
void bmp280::printIDRegister() {
    uint8_t id = read8(BMP280_CHIP_ID_REG); 
    
    hwlib::cout << "ID Register:" << hwlib::endl;
    hwlib::cout << "Hexadecimal: 0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << id << hwlib::endl;
    hwlib::cout << "Decimal: " << hwlib::dec << id << hwlib::endl << hwlib::endl;
}

// Must be 0x00 when read
void bmp280::printResetRegister() {
    uint8_t reset = read8(BMP280_RESET_REG);
    
    hwlib::cout << "Reset Register:" << hwlib::endl;
    hwlib::cout << "0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << reset << hwlib::endl << hwlib::endl;
}

void bmp280::printPowerMode() {
    // Read the current value of the control register
    uint8_t ctrl_meas = read8(BMP280_CTRL_REG);

    // Extract the power mode bits (bit 1 and bit 0)
    uint8_t power_mode = ctrl_meas & 0b11;

    // Print the power mode
    hwlib::cout << "Power Mode: " << hwlib::endl;
    switch (power_mode) {
        case SLEEP_MODE:
            hwlib::cout << "Sleep mode";
            break;
        case FORCED_MODE:
            hwlib::cout << "Forced mode";
            break;
        case NORMAL_MODE:
            hwlib::cout << "Normal mode";
            break;
        default:
            hwlib::cout << "Unknown mode";
            break;
    }
    hwlib::cout << hwlib::endl << hwlib::endl;
}

void bmp280::printOversamplingSettings() {
    // Read the current value of the control register
    uint8_t ctrl_meas = read8(BMP280_CTRL_REG);

    // Extract the osrs_t and osrs_p bits (bits 7:5 and 4:2)
    uint8_t osrs_t = (ctrl_meas >> 5) & 0b111;
    uint8_t osrs_p = (ctrl_meas >> 2) & 0b111;

    // Print the oversampling settings
    hwlib::cout << "Oversampling Settings:" << hwlib::endl;
    hwlib::cout << "Temperature: 0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << static_cast<sampling_config>(osrs_t) << hwlib::endl;
    hwlib::cout << "Pressure: 0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << static_cast<sampling_config>(osrs_p) << hwlib::endl << hwlib::endl;
}

void bmp280::printDebug(){
    printIDRegister();
    printResetRegister();
    printPowerMode();
    printOversamplingSettings();
    printCalibrationData();
}