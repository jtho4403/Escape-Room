#include "ptu_i2c.h"

#include "ptu_definitions.h"

ACCELEROMETER_CFG_STRUCT accelerometer_cfg = {ADXL345_POWER_CTL, 0x08, ADXL345_DATA_FORMAT, 0x08};

void initialise_ptu_i2c(I2C_HandleTypeDef *i2c) {


	  // bit0 = X bit1 = Y bit2 = Z bit 3 = PD = power down
	  uint8_t reg_1 = 0b00001111;
	  // Enable x, y, z and turn off power down:
	  HAL_I2C_Mem_Write(i2c, gyro_wr, L3G4200D_CTRL_REG1, 1, &reg_1, 1, 10);


	  uint8_t reg_2 = 0b00000000;
	  // HPF = high_pass filter
	  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
	  HAL_I2C_Mem_Write(i2c, gyro_wr, L3G4200D_CTRL_REG2, 1, &reg_2, 1, 10);


	  // interrupt enabled everyhting else in reg 3 disabled
	  // Configure CTRL_REG3 to generate data ready interrupt on INT2
	  // No interrupts used on INT1, if you'd like to configure INT1
	  // or INT2 otherwise, consult the datasheet:
	  HAL_I2C_Mem_Write(i2c, gyro_wr, L3G4200D_CTRL_REG3, 1, &reg_2, 1, 10);

	  uint8_t reg_4 = 0b00000000;
	  // CTRL_REG4 controls the full-scale range, among other things:
	  HAL_I2C_Mem_Write(i2c, gyro_wr, L3G4200D_CTRL_REG4, 1, &reg_4, 1, 10);

	  // CTRL_REG5 controls high-pass filtering of outputs, use it
	  // if you'd like:
	  uint8_t reg_5 = 0b00000000;
	  HAL_I2C_Mem_Write(i2c, gyro_wr, L3G4200D_CTRL_REG5, 1, &reg_5, 1, 10);

	// Turn on measurements in power register
	uint8_t areg_1 = 0b00001000;
	// Enable x, y, z and turn off power down:
	HAL_I2C_Mem_Write(i2c, accel_wr, ADXL345_POWER_CTL, 1, &areg_1, 1, 10);

	// set the data register to full resolution i.e read every 4mg/LSB?
	uint8_t areg_2 = 0b00001000;
	HAL_I2C_Mem_Write(i2c, accel_wr, ADXL345_DATA_FORMAT, 1, &areg_2, 1, 10);

	// accleration calibration
	// offset found to be 250 -8 18 note: the use of 2 compliment to send a -ve 8
	//
	uint8_t acc_offset_calibration[3] = {0x3, 0x01, 0xFE};

	// Left here in case I want to start setting the data offset registers
	HAL_I2C_Mem_Write(i2c, accel_wr, ADXL345_OFSX, 1, acc_offset_calibration, 3, 10);
	//HAL_I2C_Mem_Write(i2c, accel_wr, ADXL345_OFSY, 1, acc_offset_calibration[1], 1, 10);
	//HAL_I2C_Mem_Write(i2c, accel_wr, ADXL345_OFSZ, 1, acc_offset_calibration[2], 1, 10);

	// control register A values
	uint8_t register_setting_a = 0x78;
	// control register B values
	uint8_t register_setting_b = 0xA0;


	HAL_I2C_Mem_Write(i2c, magnet_wr, 0x00, 1, &register_setting_a, 1, 100);

	HAL_I2C_Mem_Write(i2c, magnet_wr, 0x01, 1, &register_setting_b, 1, 100);

}


