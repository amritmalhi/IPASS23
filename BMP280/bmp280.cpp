#include "bmp280.hpp"

// Constructor
// The i2c address is 0x76 if the 'SDO' pin is connected to GND (ground)
// Refer to Chapter 5.2 and 6 of the datasheet for more information
bmp280::bmp280(hwlib::i2c_bus & bus, uint8_t i2c_address ) : bus(bus), i2c_address(i2c_address) {
    loadCalibration();
    setup();
}

/**
*   The following two methods were written by Bas van den Bergh, a Computer Engineering student at HU.
*   Source: https://github.com/BasvandenBergh/IPASS_jaar1_BAS
**/
void bmp280::write(uint8_t data) {
    hwlib::i2c_write_transaction wtrans = ((hwlib::i2c_bus*)(&bus))->write(i2c_address);
    wtrans.write(data);
} 

void bmp280::read(uint8_t *data, int data_size){
    hwlib::i2c_read_transaction rtrans = ((hwlib::i2c_bus*)(&bus))->read(i2c_address);
    rtrans.read(data, data_size);
}

/* I was not able to get the following methods to work correctly.
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
}*/

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

uint32_t bmp280::read20u(uint8_t reg) {
    // Read the MSB and LSB from the register
    uint32_t msb = read16u(reg);
    uint32_t lsb = read8(reg + 2);

    // Combine MSB and the top 4 bits of LSB to form the raw 20-bit data
    uint32_t raw = (msb << 4) | (lsb >> 4);
    return raw;
}

// This configuration is for a weather monitoring system. See Chapter 3.4 for other use cases.
void bmp280::setup() {
    // Set power mode to forced
    setPowerMode(FORCED_MODE);

    // Set oversampling to ultra low power (x1 for both temperature and pressure)
    setOversampling(SAMPLING_X1, SAMPLING_X1);

    // Set the IIR filter coefficient to off
    // This requires a new method to be added to the bmp280 class
    setFilter(FILTER_OFF);
}

/**
*   The following three methods were written by Bas van den Bergh, a Computer Engineering student at HU.
*   Source: https://github.com/BasvandenBergh/IPASS_jaar1_BAS
* 
*   The algorithms for calculating the actual temp and pressure were created by Bosch and implemented by wovo.
**/

// Compensates the raw temperature reading
double bmp280::getTemperature() {
    uint32_t raw_temp = readTemperatureRaw();

    double var1, var2, T;
    var1 = (((double)raw_temp) / 16384.0 - ((double)calibration_data.dig_T1) / 1024.0) * ((double)calibration_data.dig_T2);
    var2 = ((((double)raw_temp) / 131072.0 - ((double)calibration_data.dig_T1) / 8192.0) * (((double)raw_temp) / 131072.0 - ((double)calibration_data.dig_T1) / 8192.0)) * ((double)calibration_data.dig_T3);
    calibration_data.t_fine = (int32_t)(var1 + var2);
    T = (var1 + var2) / 5120.0;
    return T;
}

// Compensates the raw pressure reading
double bmp280::getPressure() {
    uint32_t raw_press = readPressureRaw();

    double var1, var2, p;
    var1 = ((double)calibration_data.t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)calibration_data.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)calibration_data.dig_P5) * 2.0;
    var2 = (var2/4.0) + (((double)calibration_data.dig_P4) * 65536.0);
    var1 = (((double)calibration_data.dig_P3) * var1 * var1 / 524288.0 + ((double)calibration_data.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)calibration_data.dig_P1);
    if (var1 == 0) {
        return 0;  // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)raw_press;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)calibration_data.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)calibration_data.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)calibration_data.dig_P7)) / 16.0;
    return p;
}

// Read and store calibration data
void bmp280::loadCalibration() {
    write(0x88);
    read(digtemp, 6);
    write(0x8E);
    read(digpress, 18);
    calibration_data.dig_T1 = digtemp[0] | (digtemp[1] << 8);
    calibration_data.dig_T2 = (int16_t)(digtemp[2] | (digtemp[3] << 8));
    calibration_data.dig_T3 = (int16_t)(digtemp[4] | (digtemp[5] << 8));
    calibration_data.dig_P1 = digpress[0] | (digpress[1] << 8);
    calibration_data.dig_P2 = (int16_t)(digpress[2] | (digpress[3] << 8));
    calibration_data.dig_P3 = (int16_t)(digpress[4] | (digpress[5] << 8));
    calibration_data.dig_P4 = (int16_t)(digpress[6] | (digpress[7] << 8));
    calibration_data.dig_P5 = (int16_t)(digpress[8] | (digpress[9] << 8));
    calibration_data.dig_P6 = (int16_t)(digpress[10] | (digpress[11] << 8));
    calibration_data.dig_P7 = (int16_t)(digpress[12] | (digpress[13] << 8));
    calibration_data.dig_P8 = (int16_t)(digpress[14] | (digpress[15] << 8));
    calibration_data.dig_P9 = (int16_t)(digpress[16] | (digpress[17] << 8));
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
    write(BMP280_CTRL_REG);
    write(ctrl_meas);
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
    write(BMP280_CTRL_REG);
    write(ctrl_meas);
}

void bmp280::setFilter(filter_config filter) {
    // Read the current value of the configuration register
    uint8_t config = read8(BMP280_CONFIG_REG);

    // Clear the filter bits (bits 4:2)
    config &= ~(0b111 << 2);

    // Set the new filter configuration
    config |= static_cast<uint8_t>(filter) << 2;

    // Write the updated value back to the configuration register
    write(BMP280_CONFIG_REG);
    write(config);
}

// Reads the raw temperature data
uint32_t bmp280::readTemperatureRaw() {
    int32_t totaaltemp = 0x00;
    write(0xFA);
    read(resultstemp, 3);
    int32_t newresulttemp = resultstemp[2] >> 4;
    totaaltemp = resultstemp[0] << 8;
    totaaltemp = (totaaltemp | resultstemp[1]) << 4;
    return totaaltemp = totaaltemp | newresulttemp;
}

// Reads the raw pressure data
uint32_t bmp280::readPressureRaw() {
    int32_t totaalpress = 0x00;
    write(0xF7);
    read(resultspress, 3);
    int32_t newresultpressure = resultspress[2] >> 4;
    totaalpress = resultspress[0] << 8;
    totaalpress = (totaalpress | resultspress[1]) << 4;
    return totaalpress = totaalpress | newresultpressure;
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
    hwlib::cout << "dig_P9: " << calibration_data.dig_P9 << hwlib::endl;
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

// Prints the current filter settings.
void bmp280::printFilterSettings() {
    // Read the current value of the configuration register
    uint8_t config = read8(BMP280_CONFIG_REG);

    // Extract the filter bits (bits 4:2)
    uint8_t filter = (config >> 2) & 0b111;

    // Print the filter settings
    hwlib::cout << "Filter Settings:" << hwlib::endl;
    hwlib::cout << "0x" << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << static_cast<filter_config>(filter) << hwlib::endl << hwlib::endl;
}

void bmp280::printRawData() {
    uint32_t raw_temp = readTemperatureRaw();
    uint32_t raw_press = readPressureRaw();

    hwlib::cout << "Raw Temperature Data: " << hwlib::dec << raw_temp << hwlib::endl;
    hwlib::cout << "Raw Pressure Data: " << hwlib::dec << raw_press << hwlib::endl << hwlib::endl;
}

void bmp280::printCompensatedData() {
    double temp = getTemperature();
    double press = getPressure();

    hwlib::cout << "Compensated Temperature: " << static_cast<int>(temp) << " _C" << hwlib::endl;
    hwlib::cout << "Compensated Pressure: " << static_cast<int>(press) << " Pa" << hwlib::endl;
}

void bmp280::printDebug() {
    hwlib::cout << "DEBUG INFO" << hwlib::endl;
    
    hwlib::cout << "-----" << hwlib::endl << hwlib::endl;
    
    printIDRegister();
    
    printResetRegister();
    
    printPowerMode();
    
    printOversamplingSettings();
    
    printFilterSettings();
    
    printCalibrationData();
    
    hwlib::cout << hwlib::endl;
}