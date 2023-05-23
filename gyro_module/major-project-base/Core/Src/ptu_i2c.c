#include "ptu_i2c.h"
#include "ptu_definitions.h"

ACCELEROMETER_CFG_STRUCT accelerometer_cfg = {ADXL345_POWER_CTL, 0x08, ADXL345_DATA_FORMAT, 0x08};

void initialise_ptu_i2c(I2C_HandleTypeDef *i2c) {

	  /* USER CODE BEGIN 1 */
		uint8_t i2cBuf[2];
		uint8_t accdata[6];
		uint16_t x,y,z;
		uint8_t reg1a = 0x00;
		uint8_t reg2a = 0x00;
		uint8_t reg4a = 0x00;

	  uint8_t ctrl1 = 0x00, ctrl4 = 0x00;

	  HAL_StatusTypeDef return_value = 0x00;
	////////  GYRO_CFG_STRUCT gyro_cfg = {L3G4200D_CTRL_REG1, 0x0f};

	  ////////  HAL_I2C_Mem_Read(&hi2c1,gyro_rd,0x20,1,&reg1a,1,10);
	  ////////HAL_I2C_Mem_Read(&hi2c1,gyro_rd,0x21,1,&reg2a,1,10);
	  ////////HAL_I2C_Mem_Read(&hi2c1,gyro_rd,0x23,1,&reg4a,1,10);
	/*
	  //CTRL_REG1_A
	  i2cBuf[0]=0x20;
	  i2cBuf[1]=0x97;
	  HAL_I2C_Master_Transmit(&hi2c1, gyro_wr, i2cBuf, 2, 10);
	  //CTRL_REG4_A
	  i2cBuf[0]=0x23;
	  i2cBuf[1]=0x38;
	  HAL_I2C_Master_Transmit(&hi2c1, gyro_wr, i2cBuf, 2, 10);
	  //CTRL_REG2_A
	  i2cBuf[0]=0x21;
	  i2cBuf[1]=0x38;
	  HAL_I2C_Master_Transmit(&hi2c1, gyro_wr, i2cBuf, 2, 10);
	*/
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

	  //if(scale == 250){
	  //HAL_I2C_Mem_Write(&i2c, gyro_wr, L3G4200D_CTRL_REG4, 1, &reg_2, 1, 10);
	  //}else if(scale == 500){
	  //writeRegister(gyro_Address, gyro_CTRL_REG4, 0b00010000);
	  //}else{
	  //writeRegister(gyro_Address, gyro_CTRL_REG4, 0b00110000);
	  //}

	  // CTRL_REG5 controls high-pass filtering of outputs, use it
	  // if you'd like:
	  uint8_t reg_5 = 0b00000000;
	  HAL_I2C_Mem_Write(i2c, gyro_wr, L3G4200D_CTRL_REG5, 1, &reg_5, 1, 10);



	  //trying to read from the same registers where we wrote in
	  HAL_I2C_Mem_Read(i2c,gyro_rd,0x20,1,&reg1a,1,10);
	  HAL_I2C_Mem_Read(i2c,gyro_rd,0x21,1,&reg2a,1,10);
	  ////////return_value = HAL_I2C_Mem_Read(&hi2c1,gyro_rd,0x23,1,&reg4a,1,10);


	  /* Configure the low level interface ---------------------------------------*/
	  //LSM303DLHC_LowLevel_Init();

	  /*
	   *
	       LSM303DLHC_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
	    LSM303DLHC_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
	    LSM303DLHC_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
	    LSM303DLHC_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
	    LSM303DLHC_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
	    LSM303DLHC_InitStructure.Endianness=LSM303DLHC_BLE_LSB;
	    LSM303DLHC_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;
	   */
	  /*
	  HAL_I2C_Mem_Read(&hi2c1,ACC_I2C_ADDRESS_READ,0x20,1,&reg1a,1,10);
	  HAL_I2C_Mem_Read(&hi2c1,ACC_I2C_ADDRESS_READ,0x21,1,&reg2a,1,10);
	  HAL_I2C_Mem_Read(&hi2c1,ACC_I2C_ADDRESS_READ,0x23,1,&reg4a,1,10);

	  // Configure MEMS: data rate, power mode, full scale and axes
	  ctrl1 |= (uint8_t) (LSM303DLHC_NORMAL_MODE | LSM303DLHC_ODR_100_HZ | LSM303DLHC_AXES_ENABLE);

	  ctrl4 |= (uint8_t) (LSM303DLHC_BlockUpdate_Continous | LSM303DLHC_BLE_LSB | \
			  LSM303DLHC_FULLSCALE_2G | LSM303DLHC_HR_ENABLE);

	  // Write value to ACC MEMS CTRL_REG1 regsister
	  HAL_I2C_Mem_Write(&hi2c1, ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG1_A, 1, &ctrl1, 1, 100);

	  // Write value to ACC MEMS CTRL_REG4 regsister
	  HAL_I2C_Mem_Write(&hi2c1, ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, 1, &ctrl4, 1, 100);
	*/
	  /*
	  //CTRL_REG1_A
	  i2cBuf[0]=0x20;
	  i2cBuf[1]=0x27;
	  HAL_I2C_Master_Transmit(&hi2c1, ACC_I2C_ADDRESS, i2cBuf, 2, 10);
	  //CTRL_REG4_A
	  i2cBuf[0]=0x23;
	  i2cBuf[1]=0x38;
	  HAL_I2C_Master_Transmit(&hi2c1, ACC_I2C_ADDRESS, i2cBuf, 2, 10);
	  //CTRL_REG2_A
	  i2cBuf[0]=0x21;
	  i2cBuf[1]=0x38;
	  HAL_I2C_Master_Transmit(&hi2c1, ACC_I2C_ADDRESS, i2cBuf, 2, 10);

	  //trying to read from the same registers where we wrote in
	  HAL_I2C_Mem_Read(&hi2c1,ACC_I2C_ADDRESS_READ,0x20,1,&reg1a,1,10);
	  HAL_I2C_Mem_Read(&hi2c1,ACC_I2C_ADDRESS_READ,0x21,1,&reg2a,1,10);
	  HAL_I2C_Mem_Read(&hi2c1,ACC_I2C_ADDRESS_READ,0x23,1,&reg4a,1,10);

	*/

	//uint8_t status = 0x00;
	uint8_t ready = 0x00;
	uint8_t return_status = 0x00;

	uint8_t val_1[2];
	uint16_t* val_1p = &val_1[0];
	val_1p = 0x00;

	uint8_t text_buffer[255];

	uint16_t val_2 = 0x00;
	uint16_t val_3 = 0x00;

	uint8_t Text[] = "Hello\r\n";
//	BSP_LED_Toggle(LED_GREEN);
	//BSP_GYRO_Init();
	//BSP_ACCELERO_Init();

	// reset lidar board
	uint8_t reset_value = 0x00;
	return_value = HAL_I2C_Mem_Write(i2c, LIDAR_WR, 0x00, 1, &reset_value, 1, 10);
	//uint8_t lidar_value = 0x03;
	//return_value = HAL_I2C_Mem_Write(&hi2c1, LIDAR_WR, 0x00, 1, &lidar_value, 1, 100);

	//uint8_t lidar_value = 0x03;
	//return_value = HAL_I2C_Mem_Write(&hi2c1, LIDAR_WR, 0x00, 1, &lidar_value, 1, 100);




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


