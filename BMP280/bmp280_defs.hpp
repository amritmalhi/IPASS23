/**
 * @file bmp280_defs.hpp
 * @brief Definitions and constants for BMP280 sensor configuration and data.
 */

#ifndef BMP280_DEFS_HPP
#define BMP280_DEFS_HPP

/**
 * @enum sampling_config
 * @brief Oversampling settings for pressure and temperature measurements.
 *
 * This enumeration defines the oversampling configurations for pressure and temperature measurements
 * on the BMP280 sensor. The values correspond to the settings specified in Chapter 3.3.1 and 3.3.2
 * of the datasheet.
 */
enum sampling_config : uint8_t {
    SAMPLING_NONE = 0x00, /**< No oversampling */
    SAMPLING_X1 = 0x01,   /**< Oversampling x1 */
    SAMPLING_X2 = 0x02,   /**< Oversampling x2 */
    SAMPLING_X4 = 0x03,   /**< Oversampling x4 */
    SAMPLING_X8 = 0x04,   /**< Oversampling x8 */
    SAMPLING_X16 = 0x05   /**< Oversampling x16 */
};

/**
 * @enum filter_config
 * @brief IIR filter settings.
 *
 * This enumeration defines the IIR (Infinite Impulse Response) filter configurations
 * for pressure and temperature measurements on the BMP280 sensor. The values correspond
 * to the settings specified in Chapter 3.3.3 of the datasheet.
 */
enum filter_config : uint8_t {
    FILTER_OFF = 0x00, /**< Filter off */
    FILTER_X2 = 0x01,  /**< Filter coefficient 2 */
    FILTER_X4 = 0x02,  /**< Filter coefficient 4 */
    FILTER_X8 = 0x03,  /**< Filter coefficient 8 */
    FILTER_X16 = 0x04  /**< Filter coefficient 16 */
};

/**
 * @enum power_modes
 * @brief Available power modes.
 *
 * This enumeration defines the available power modes for the BMP280 sensor. The values correspond
 * to the settings specified in Chapter 3.6 of the datasheet.
 */
enum power_modes : uint8_t {
    SLEEP_MODE = 0x00,   /**< Sleep mode */
    FORCED_MODE = 0x02,  /**< Forced mode */
    NORMAL_MODE = 0x03   /**< Normal mode */
};

/**
 * @enum standby_config
 * @brief Standby time settings in normal mode.
 *
 * This enumeration defines the standby time settings in normal mode for the BMP280 sensor.
 * The values correspond to the settings specified in Chapter 3.6.3 of the datasheet.
 */
enum standby_config : uint8_t {
    STANDBY_MS_1 = 0x00,     /**< Standby time 1 ms */
    STANDBY_MS_63 = 0x01,    /**< Standby time 63 ms */
    STANDBY_MS_125 = 0x02,   /**< Standby time 125 ms */
    STANDBY_MS_250 = 0x03,   /**< Standby time 250 ms */
    STANDBY_MS_500 = 0x04,   /**< Standby time 500 ms */
    STANDBY_MS_1000 = 0x05,  /**< Standby time 1000 ms */
    STANDBY_MS_2000 = 0x06,  /**< Standby time 2000 ms */
    STANDBY_MS_4000 = 0x07   /**< Standby time 4000 ms */
};

/**
 * @brief Register addresses containing calibration data.
 *
 * This section defines the register addresses that contain calibration data for the BMP280 sensor.
 * The addresses are specified in Chapter 3.11.2 of the datasheet.
 */
constexpr uint8_t BMP280_DIG_T1_REG = 0x88;
constexpr uint8_t BMP280_DIG_T2_REG = 0x8A;
constexpr uint8_t BMP280_DIG_T3_REG = 0x8C;
constexpr uint8_t BMP280_DIG_P1_REG = 0x8E;
constexpr uint8_t BMP280_DIG_P2_REG = 0x90;
constexpr uint8_t BMP280_DIG_P3_REG = 0x92;
constexpr uint8_t BMP280_DIG_P4_REG = 0x94;
constexpr uint8_t BMP280_DIG_P5_REG = 0x96;
constexpr uint8_t BMP280_DIG_P6_REG = 0x98;
constexpr uint8_t BMP280_DIG_P7_REG = 0x9A;
constexpr uint8_t BMP280_DIG_P8_REG = 0x9C;
constexpr uint8_t BMP280_DIG_P9_REG = 0x9E;

/**
 * @brief Register addresses containing configurations and data.
 *
 * This section defines the register addresses that contain configurations and data for the BMP280 sensor.
 * The addresses are specified in Chapter 4 of the datasheet.
 */
constexpr uint8_t BMP280_CHIP_ID_REG = 0xD0;
constexpr uint8_t BMP280_RESET_REG = 0xE0;
constexpr uint8_t BMP280_STATUS_REG = 0xF3;
constexpr uint8_t BMP280_CTRL_REG = 0xF4;
constexpr uint8_t BMP280_CONFIG_REG = 0xF5;
constexpr uint8_t BMP280_PRESS_DATA_REG = 0xF7;
constexpr uint8_t BMP280_TEMP_DATA_REG = 0xFA;

/**
 * @struct bmp280_calibration_data
 * @brief Struct that holds calibration data for the BMP280 sensor.
 *
 * This struct holds the calibration data for the BMP280 sensor. It includes the calibration
 * coefficients for temperature and pressure measurements, as well as the t_fine value used for
 * temperature calculation.
 */
typedef struct {
    uint16_t dig_T1;   /**< Temperature calibration coefficient T1 */
    int16_t dig_T2;    /**< Temperature calibration coefficient T2 */
    int16_t dig_T3;    /**< Temperature calibration coefficient T3 */
    uint16_t dig_P1;   /**< Pressure calibration coefficient P1 */
    int16_t dig_P2;    /**< Pressure calibration coefficient P2 */
    int16_t dig_P3;    /**< Pressure calibration coefficient P3 */
    int16_t dig_P4;    /**< Pressure calibration coefficient P4 */
    int16_t dig_P5;    /**< Pressure calibration coefficient P5 */
    int16_t dig_P6;    /**< Pressure calibration coefficient P6 */
    int16_t dig_P7;    /**< Pressure calibration coefficient P7 */
    int16_t dig_P8;    /**< Pressure calibration coefficient P8 */
    int16_t dig_P9;    /**< Pressure calibration coefficient P9 */
    int32_t t_fine;    /**< Fine temperature value for temperature calculation */
} bmp280_calibration_data;

#endif // BMP280_DEFS_HPP
