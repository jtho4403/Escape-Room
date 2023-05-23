/*
 * lsm303dlhc_module.h
 *
 *  Created on: May 23, 2023
 *      Author: David
 */

#ifndef INC_LSM303DLHC_MODULE_H_
#define INC_LSM303DLHC_MODULE_H_


#define LSM303DLHC_I2C_ADDRESS 0x19

void lsm303dlhc_initialise(I2C_HandleTypeDef *i2c);
void lsm303dlhc_read_acc(int16_t* accelData, I2C_HandleTypeDef *i2c);
void lsm303dlhc_read_mag(int16_t* accelMaga, I2C_HandleTypeDef *i2c);


#endif /* INC_LSM303DLHC_MODULE_H_ */
