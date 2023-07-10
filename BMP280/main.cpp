#include "hwlib.hpp"
#include "bmp280.hpp"

int main( void ){	
    
    namespace target = hwlib::target;
    
    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );
    
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    
    auto bmp = bmp280( i2c_bus );
    
    for(;;){
        hwlib::cout << "Data from registers:" << hwlib::endl;
        
        hwlib::cout << "---" << hwlib::endl << hwlib::endl;
    
        bmp.printDebug();
        
        hwlib::wait_ms( 10000 );
    } 
}

    