# BMP280 Project

This project is a C++ library for interfacing with the BMP280 temperature and pressure sensor. It provides functions to configure the sensor, read raw sensor data, and calculate compensated temperature and pressure values.

## Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The BMP280 is a digital sensor that measures temperature and pressure. This library provides a convenient interface to interact with the sensor using the I2C communication protocol. It includes functions to set the power mode, configure oversampling settings, read raw sensor data, and calculate compensated temperature and pressure values.

Took inspiration from:
Bosch's BMP280 API:     https://github.com/BoschSensortec/BMP2-Sensor-API/ 
wovo's example code:    https://github.com/wovo/hwlib-examples/tree/master/arduino-due/due-%230210-dmp280 
basvandenbergh: https:  https://github.com/BasvandenBergh/IPASS_jaar1_BAS 

I was not able to get the calibration of raw temperature/pressure data to work with my own read and write methods, so I had to borrow Bas'

## Installation

1. Clone the repository to your local machine:

   ```bash
   https://github.com/amritmalhi/IPASS23.git
   ```

2. Ensure you have the necessary dependencies installed. This project requires the `hwlib` library for I2C communication.

3. Build the project using your preferred C++ build system.

4. Link the built library and include the necessary header files in your project.

## Usage

1. Include the `bmp280.hpp` header file in your C++ project:

   ```cpp
   #include "bmp280.hpp"
   ```

2. Create an instance of the `bmp280` class and specify the I2C bus and device address:

   ```cpp
   // Create an instance of the BMP280 class
   bmp280 sensor(i2c_bus, 0x76);
   ```

3. Initialize the sensor and configure the desired settings:

   ```cpp
   // Initialize the sensor
   sensor.setup();

   // Set the power mode
   sensor.setPowerMode(bmp280::FORCED_MODE);

   // Set the oversampling settings
   sensor.setOversampling(bmp280::SAMPLING_X1, bmp280::SAMPLING_X1);
   ```

4. Read sensor data:

   ```cpp
   // Read the compensated temperature
   double temperature = sensor.compensateTemperature();

   // Read the compensated pressure
   double pressure = sensor.compensatePressure();
   ```

For more detailed examples and usage instructions, please refer to the code documentation.

## License

This project is licensed under the [Boost Software License](LICENSE).

