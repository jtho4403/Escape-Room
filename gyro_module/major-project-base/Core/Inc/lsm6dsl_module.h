/*
 * LSM6DSL_module.h
 *
 *  Created on: 23 May 2023
 *      Author: David
 */

#ifndef LSM6DSL_MODULE_H_
#define LSM6DSL_MODULE_H_

#include "stm32f3xx_hal.h"

// Define things here
#define LSM6DSL_ADDRESS 0xD6

#define OUTX_L_GYRO 0x22 // Gyro output reg

//OUTX_H_G r 23 00100011 output
//OUTY_L_G r 24 00100100 output
//OUTY_H_G r 25 00100101 output
//OUTZ_L_G r 26 00100110 output
//OUTZ_H_G r 27 00100111 output

#define OUTX_L_ADXL 0x28
//OUTX_H_XL r 29 00101001 output
//OUTY_L_XL r 2A 00101010 output
//OUTY_H_XL r 2B 00101011 output
//OUTZ_L_XL r 2C 00101100 output
//OUTZ_H_XL r 2D 00101101 output


void LSM6DSL_initalise(I2C_HandleTypeDef *i2c);
void lsm6dsl_read_accel_data(int16_t* accelData, I2C_HandleTypeDef *i2c);
void lsm6dsl_read_gyro_data(int16_t* gyroData, I2C_HandleTypeDef *i2c);

#endif /* LSM6DSL_MODULE_H_ */
