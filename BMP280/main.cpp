#include "hwlib.hpp"
#include "bmp280.hpp"

int main( void ){	
    
    namespace target = hwlib::target;
    
    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );
    
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    
    auto bmp = bmp280( i2c_bus );
    
    uint8_t id;
    uint8_t reset;
    
    id = bmp.read8(BMP280_CHIP_ID_REG); // Must be 0x58 when read
    reset = bmp.read8(BMP280_SOFT_RESET_REG); // Must be 0x00 when read

    for(;;){
        hwlib::cout << "Data from registers:" << hwlib::endl;
    
        // Read and print the ID register
        bmp.printIDRegister();

        // Read and print the reset register
        bmp.printResetRegister();
        
        // Read and print the calibration data
        bmp.printCalibrationData();
        
        hwlib::wait_ms( 5'000 );
    } 
}

    