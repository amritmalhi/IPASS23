#include "hwlib.hpp"
#include "bmp280.hpp"

const char* getOutfitRecommendation(float temperature) {
    if (temperature < 0) {
        return "It's freezing outside! Wear a heavy coat, gloves, and a hat.";
    } else if (temperature < 10) {
        return "It's pretty cold. Wear a coat and a hat.";
    } else if (temperature < 20) {
        return "It's a bit chilly. A jacket should suffice.";
    } else if (temperature < 30) {
        return "It's warm. A t-shirt and shorts would be comfortable.";
    } else {
        return "It's very hot. Wear light clothing and stay hydrated.";
    }
}

int main() {
    namespace target = hwlib::target;  // Adjust to your target board

    // Create the i2c bus for the BMP280
    auto scl = target::pin_oc(target::pins::scl);
    auto sda = target::pin_oc(target::pins::sda);
    auto i2c_bus_bmp = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    
    // Create the i2c bus for the OLED
    auto scl1 = target::pin_oc(target::pins::scl1);
    auto sda1 = target::pin_oc(target::pins::sda1);
    auto i2c_bus_oled = hwlib::i2c_bus_bit_banged_scl_sda(scl1, sda1);

    // Create the BMP280 and OLED objects
    bmp280 sensor(i2c_bus_bmp);
    
    auto oled = hwlib::glcd_oled( i2c_bus_oled );
    auto font = hwlib::font_default_8x8();
    auto display = hwlib::terminal_from(oled,font);
    
    sensor.setup();
    
    hwlib::wait_ms(10);

    // Print debug information to check if the sensor is properly set up
    sensor.printDebug();

    // Infinite loop to continuously read from the sensor
    while (true) {

        // Print the compensated data
        float temperature = sensor.getTemperature();
        hwlib::cout << "Temperature: " << (int)temperature << "C\n";
    
        // Print the outfit recommendation
        const char* recommendation = getOutfitRecommendation(temperature);
        hwlib::cout << recommendation << "\n";
        hwlib::cout << hwlib::endl;

        display
            << "\f" << recommendation  
            << "\t0305" << (int) temperature << " C"
            << "\n" 
            << hwlib::flush;   
        
        // Delay for a second
        hwlib::wait_ms(20000);
    }

    return 0;
}
