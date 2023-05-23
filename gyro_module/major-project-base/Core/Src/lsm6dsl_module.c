/*
 * LSM6DSL_module.c
 *
 *  Created on: 23 May 2023
 *      Author: David
 */
#include "lsm6dsl_module.h"
#include "stm32f3xx_hal.h"

void LSM6DSL_initalise(I2C_HandleTypeDef *i2c){
	 // Example: Set the accelerometer and gyroscope to normal mode with 104 Hz output data rate
	 uint8_t ctrl1_acc_reg = 0x10; // Control Register 1 - Accelerometer
	 uint8_t ctrl2_gyro_reg = 0x11; // Control Register 2 - Gyroscope
	 uint8_t config_data = 0b01000000; // Normal mode, 104 Hz output data rate

	 HAL_I2C_Mem_Write(i2c, LSM6DSL_ADDRESS, ctrl1_acc_reg, I2C_MEMADD_SIZE_8BIT, &config_data, 1, HAL_MAX_DELAY);
	 HAL_I2C_Mem_Write(i2c, LSM6DSL_ADDRESS, ctrl2_gyro_reg, I2C_MEMADD_SIZE_8BIT, &config_data, 1, HAL_MAX_DELAY);

}

void lsm6dsl_read_accel_data(int16_t* accelData, I2C_HandleTypeDef *i2c){
    uint8_t data[6];
    uint8_t regAddr = 0x28; // OUTX_L_XL register address with MSB set for auto-increment

    // Read the accelerometer data
    HAL_I2C_Mem_Read(i2c, LSM6DSL_ADDRESS, regAddr, 1, data, 6, 10);

    // Convert the raw data to signed 16-bit values
    accelData[0] = (int16_t)((data[1] << 8) | data[0]); // X-axis data
    accelData[1] = (int16_t)((data[3] << 8) | data[2]); // Y-axis data
    accelData[2] = (int16_t)((data[5] << 8) | data[4]); // Z-axis data
}

void lsm6dsl_read_gyro_data(int16_t* gyroData, I2C_HandleTypeDef *i2c){
    uint8_t data[6];
    uint8_t regAddr = 0x22; // OUTX_L_XL register address with MSB set for auto-increment
    // Read the accelerometer data
    HAL_I2C_Mem_Read(i2c, LSM6DSL_ADDRESS, regAddr, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);

    // Convert the raw data to signed 16-bit values
    gyroData[0] = (int16_t)((data[1] << 8) | data[0]); // X-axis data
    gyroData[1] = (int16_t)((data[3] << 8) | data[2]); // Y-axis data
    gyroData[2] = (int16_t)((data[5] << 8) | data[4]); // Z-axis data
}

