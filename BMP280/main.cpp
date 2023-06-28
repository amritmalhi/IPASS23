#include "hwlib.hpp"
#include "BMP280.h"

int main( void ){	
    
    namespace target = hwlib::target;
    
    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );
    
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    
    auto bmp = BMP280( i2c_bus, 0x76 );
    
    uint8_t id;

    id = bmp.readRegister(0xD0);

    for(;;){
        hwlib::cout << "Data from register:" << hwlib::endl;
        hwlib::cout << "Hexadecimal: 0x" << hwlib::hex << id << hwlib::endl;
        hwlib::cout << "Decimal: " << hwlib::dec << id << hwlib::endl << hwlib::endl;
    
        hwlib::wait_ms( 1'000 );
    }


    
}

    