/*
 * lsm303dlhc_module.c
 *
 *  Created on: May 23, 2023
 *      Author: David
 */
#include "stm32f3xx_hal.h"
#include "lsm303dlhc_module.h"
#include "stdio.h"

//I2C_HandleTypeDef hi2c1;



//void SystemClock_Config(void);
//void I2C1_Init(void);


//int main(void)
//{
//  HAL_Init();
//  SystemClock_Config();
//  I2C1_Init();
//  LSM303DLHC_Init();
//
//  while (1)
//  {
//    LSM303DLHC_ReadAcc();
//    LSM303DLHC_ReadMag();
//
//    // Do something with the accelerometer and magnetometer data
//
//    HAL_Delay(500);
//  }
//}

//void SystemClock_Config(void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//
//  __HAL_RCC_PWR_CLK_ENABLE();
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}

//void I2C1_Init(void)
//{
//  hi2c1.Instance = I2C1;
//  hi2c1.Init.Timing = 0x0010020A;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}

void lsm303dlhc_initialise(I2C_HandleTypeDef *i2c){
  uint8_t ctrl1_reg_a = 0x57; // Normal power mode, all axes enabled, 50Hz data rate
  uint8_t ctrl4_reg_a = 0x00; // Continous conversion mode
  uint8_t ctrl1_reg_m = 0x00; // Temp sensor disabled, 4 Gauss full scale
  uint8_t ctrl5_reg_m = 0x80; // Magnetic field sensor in high-resolution mode

  // Configure accelerometer
  HAL_I2C_Mem_Write(i2c, LSM303DLHC_I2C_ADDRESS, 0x20, 1, &ctrl1_reg_a, 1, HAL_MAX_DELAY);

  // Configure magnetometer
  //HAL_I2C_Mem_Write(i2c, LSM303DLHC_I2C_ADDRESS, 0x24, 1, &ctrl1_reg_m, 1, HAL_MAX_DELAY);
  //HAL_I2C_Mem_Write(i2c, LSM303DLHC_I2C_ADDRESS, 0x25, 1, &ctrl5_reg_m, 1, HAL_MAX_DELAY);

  // Verify the configuration by reading back the registers and checking their values
  //uint8_t read_ctrl1_reg_a;
  //uint8_t read_ctrl1_reg_m;
  //uint8_t read_ctrl5_reg_m;

  //HAL_I2C_Mem_Read(i2c, LSM303DLHC_I2C_ADDRESS, 0x20, 1, &read_ctrl1_reg_a, 1, HAL_MAX_DELAY);
  //HAL_I2C_Mem_Read(i2c, LSM303DLHC_I2C_ADDRESS, 0x24, 1, &read_ctrl1_reg_m, 1, HAL_MAX_DELAY);
  //HAL_I2C_Mem_Read(i2c, LSM303DLHC_I2C_ADDRESS, 0x25, 1, &read_ctrl5_reg_m, 1, HAL_MAX_DELAY);

//  if (read_ctrl1_reg_a != ctrl1_reg_a || read_ctrl1_reg_m != ctrl1_reg_m || read_ctrl5_reg_m != ctrl5_reg_m)
//  {
//    Error_Handler();
//  }
}

void lsm303dlhc_read_acc(int16_t* accelData, I2C_HandleTypeDef *i2c)
{
  uint8_t acc_data[6];

  HAL_I2C_Mem_Read(i2c, LSM303DLHC_I2C_ADDRESS, 0x28, 1, acc_data, 6, HAL_MAX_DELAY);

  accelData[0] = (int16_t)(acc_data[0] | (acc_data[1] << 8));
  accelData[1] = (int16_t)(acc_data[2] | (acc_data[3] << 8));
  accelData[2] = (int16_t)(acc_data[4] | (acc_data[5] << 8));

  // Process accelerometer data
}

void lsm303dlhc_read_mag(int16_t* magData, I2C_HandleTypeDef *i2c)
{
  uint8_t mag_data[6];

  HAL_I2C_Mem_Read(i2c, LSM303DLHC_I2C_ADDRESS, 0x88 | 0x80, 1, mag_data, 6, HAL_MAX_DELAY);

  magData[0] = (int16_t)(mag_data[0] | (mag_data[1] << 8));
  magData[1] = (int16_t)(mag_data[4] | (mag_data[5] << 8));
  magData[2] = (int16_t)(mag_data[2] | (mag_data[3] << 8));

  // Process magnetometer data
}

//LSM303DLHC_LowLevel_Init();

/*
     LSM303DLHC_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
  LSM303DLHC_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
  LSM303DLHC_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
  LSM303DLHC_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
  LSM303DLHC_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
  LSM303DLHC_InitStructure.Endianness=LSM303DLHC_BLE_LSB;
  LSM303DLHC_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;



// Configure MEMS: data rate, power mode, full scale and axes
ctrl1 |= (uint8_t) (LSM303DLHC_NORMAL_MODE | LSM303DLHC_ODR_100_HZ | LSM303DLHC_AXES_ENABLE);

ctrl4 |= (uint8_t) (LSM303DLHC_BlockUpdate_Continous | LSM303DLHC_BLE_LSB | \
		  LSM303DLHC_FULLSCALE_2G | LSM303DLHC_HR_ENABLE); ==

// Write value to ACC MEMS CTRL_REG1 regsister
HAL_I2C_Mem_Write(&hi2c1, ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG1_A, 1, &ctrl1, 1, 100);

// Write value to ACC MEMS CTRL_REG4 regsister
HAL_I2C_Mem_Write(&hi2c1, ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, 1, &ctrl4, 1, 100);
*/

