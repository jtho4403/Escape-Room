/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ptu_definitions.h"
#include "ptu_i2c.h"
#include "serial.h"

#include "math.h"

#include "timer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}


// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}


typedef union {
	uint8_t all_leds;
	struct {
		uint8_t led_pair_1 : 2;
		uint8_t led_pair_2 : 2;
		uint8_t led_set_of_4 : 4;
	} led_groups;
} LedRegister;



uint16_t last_capture = 0;
uint16_t diff = 0;

uint16_t rise_time = 0;
uint16_t last_period = 0;

float x = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint8_t buffer[32];
	if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		uint16_t IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value

		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1)
			rise_time = IC_Val1;
		else
			last_period = IC_Val1 - rise_time;

		diff = IC_Val1 - last_capture;
		last_capture = IC_Val1;
	}
}
volatile uint8_t string_to_send[500] = "This is a string !\r\n";
volatile uint8_t test_string[64] = "h!\r\n";
volatile uint8_t rxBuffer[64];

//// Function to read data from Arduino Uno via UART3
//void readFromArduinoUno(void)
//{
//  // Buffer to store received data
//  uint8_t rxBuffer[3];
//
//  // Configure UART3
//  UART_HandleTypeDef huart3;
//  huart3.Instance = USART3;
//  huart3.Init.BaudRate = 115200;
//  huart3.Init.WordLength = UART_WORDLENGTH_8B;
//  huart3.Init.StopBits = UART_STOPBITS_1;
//  huart3.Init.Parity = UART_PARITY_NONE;
//  huart3.Init.Mode = UART_MODE_RX;
//  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
//
//  // Enable UART3 clock
//  __HAL_RCC_USART3_CLK_ENABLE();
//
//  // Configure GPIO pins
//  GPIO_InitTypeDef GPIO_InitStruct;
//  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//  if (HAL_UART_Init(&huart3) != HAL_OK)
//  {
//    // Initialization error handling
//    // Add your error handling code here
//  }
//
//
//}

//static volatile float dx = 0;
//volatile float dt = 0.01;
//static volatile float ds;
//volatile uint16_t dv;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	enable_clocks();
	initialise_board();

	LedRegister *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	SerialInitialise(BAUD_57600, &USART3_PORT, 0x00);

	SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);

	EnableInterrupt();

	HAL_StatusTypeDef return_value = 0x00;

	volatile uint16_t vertical_PWM = 1000;
	volatile uint16_t horizontal_PWM = 1000;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  //MX_TIM2_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PAN
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // TILT

	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1); // LIDAR

	// TIM 2 is setup with a prescaler that makes 1 count = 1 microsecond
	// Even with HAL, you can still set the values yourself
	TIM2->ARR = 20000; // 20000 = 20ms, which is the desired clock period for servos
	TIM2->CR1 |= TIM_CR1_ARPE; // this makes the timing not change until the next pulse is finished

	// note: for PWM if you continually change the clock period
	// you can get unexpected results. To remove this, set ARPE so that the
	// ARR settings are not activated until the next cycle.


	// Tell the NVIC module that timer2 interrupts should be handled
	// NVIC_EnableIRQ(TIM2_IRQn);

	// literallty set the PTU registers using I2C
	// Stews code to initalise HAL_TYPE_STRUCT used for I2C communications
	// he is exrecting a certain stream of infromation and sets it up like so
	initialise_ptu_i2c(&hi2c2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


	// reset lidar board
	uint8_t reset_value = 0x00;
	return_value = HAL_I2C_Mem_Write(&hi2c2, LIDAR_WR, 0x00, 1, &reset_value, 1, 10);

	uint8_t PWM_direction_clockwise = 1;

	// delay for initialisation of the lidar
	HAL_Delay(100);

	uint16_t time = 100; // 4 seconds = 4000ms // 10ms = 10
	//Idea get a interrupt every 10ms
	up_count_TIM3(time, PTU_callback_function);

	float x_acc = 0.0;
	float y_acc = 0.0;
	float z_acc = 0.0;
	float x_v = 0.0;
	float y_v = 0.0;
	float z_v = 0.0;
	float x_angle = 0.0;
	float y_angle = 0.0;
	float z_angle = 0.0;
	float p_rps = 0.0;
	float q_rps = 0.0;
	float r_rps = 0.0;
	float global_roll = 0.0;
	float global_pitch = 0.0;
	float global_yaw = 0.0;
	while (1)
	{
		if (PWM_direction_clockwise == 1) {
			vertical_PWM += 3;
			horizontal_PWM += 3;
		}
		else {
			vertical_PWM -= 3;
			horizontal_PWM -= 3;
		}

		if (vertical_PWM > 2390) {
			vertical_PWM = 2390;
			PWM_direction_clockwise = 0;
		}
		if (vertical_PWM < 505) {
			vertical_PWM = 505;
			PWM_direction_clockwise = 1;
		}
		if (horizontal_PWM > 2390) {
			horizontal_PWM = 2390;
			//PWM_direction_clockwise = 0;
		}
		if (horizontal_PWM < 505) {
			horizontal_PWM = 505;
			//PWM_direction_clockwise = 1;
		}

		TIM2->CCR1 = vertical_PWM;
		TIM2->CCR2 = horizontal_PWM;

//#define LEFT_Pin GPIO_PIN_13
//#define LEFT_GPIO_Port GPIOD
//#define DOWN_Pin GPIO_PIN_14
//#define DOWN_GPIO_Port GPIOD
//#define UP_Pin GPIO_PIN_15
//#define UP_GPIO_Port GPIOD
//#define RIGHT_Pin GPIO_PIN_9
//#define RIGHT_GPIO_Port GPIOC

		  GPIO_PinState pinState1 = HAL_GPIO_ReadPin(LEFT_Pin, GPIO_PIN_13);
		  if (pinState1 == GPIO_PIN_SET) {
			  //horizontal_PWM -= 3;
			  int c=5;
		  } else {
			  int l=5;

		  }

		 GPIO_PinState pinState2 = HAL_GPIO_ReadPin(RIGHT_Pin, GPIO_PIN_9);
		  if (pinState2 == GPIO_PIN_SET) {
			  //horizontal_PWM -= 3;
			  int c=5;
		  } else {
			  int l=5;

		  }
		  GPIO_PinState pinState3 = HAL_GPIO_ReadPin(DOWN_Pin, GPIO_PIN_14);
		  if (pinState3 == GPIO_PIN_SET) {
			  //vertical_PWM -= 3;
			  int c=5;
		  } else {
			  int l=5;

		  }
		  GPIO_PinState pinState4 = HAL_GPIO_ReadPin(UP_Pin ,GPIO_PIN_15);
		  if (pinState4 == GPIO_PIN_SET) {
			  //vertical_PWM += 3;
			  int c=5;
		  } else {
			  int l=5;

		  }

		  //TIM2->CCR1 = vertical_PWM;
		  //TIM2->CCR2 = horizontal_PWM;

		uint8_t xMSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c2,gyro_rd, 0x29, 1, &xMSB, 1, 10);
		uint8_t xLSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c2,gyro_rd, 0x28, 1, &xLSB, 1, 10);
		int16_t yaw_rate = ((xMSB << 8) | xLSB);

		uint8_t yMSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c2,gyro_rd, 0x2B, 1, &yMSB, 1, 10);
		uint8_t yLSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c2,gyro_rd, 0x2A, 1, &yLSB, 1, 10);
		int16_t pitch_rate = ((yMSB << 8) | yLSB);

		uint8_t zMSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c2,gyro_rd, 0x2D, 1, &zMSB, 1, 10);
		uint8_t zLSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c2,gyro_rd, 0x2C, 1, &zLSB, 1, 10);
		int16_t roll_rate = ((zMSB << 8) | zLSB);

		if (pitch_rate < 0)
			led_register->led_groups.led_pair_1 = 0b01;
		else
			led_register->led_groups.led_pair_1 = 0b10;

		if (yaw_rate < 0)
			led_register->led_groups.led_pair_2 = 1;
		else
			led_register->led_groups.led_pair_2 = 2;


		uint8_t lidar_value = 0x03;
		return_value = HAL_I2C_Mem_Write(&hi2c2, LIDAR_WR, 0x00, 1, &lidar_value, 1, 100);

		lidar_value = 0xff;

		uint8_t lidar_MSBa = 0x00;
		uint8_t lidar_LSBa = 0x00;

		volatile uint16_t lidar_distance = 0xff;

		uint16_t timeout;
// some WERID ERROS happen here
		while ((lidar_value & 0x01) != 0x00) {
			return_value = HAL_I2C_Mem_Read(&hi2c2, LIDAR_RD, 0x01, 1, &lidar_value, 1, 100);

			return_value = HAL_I2C_Mem_Read(&hi2c2, LIDAR_RD, 0x0f, 1, &lidar_MSBa, 1, 100);
			return_value = HAL_I2C_Mem_Read(&hi2c2, LIDAR_RD, 0x10, 1, &lidar_LSBa, 1, 100);

			lidar_distance = ((lidar_MSBa << 8) | lidar_LSBa);
			timeout += 1;
			if (timeout > 0xff)
				break;
		}

		uint8_t lidar_ranges = lidar_distance / (100/4); // 100cm broken into 4 groups
		if (lidar_ranges > 3)
			lidar_ranges = 3;

		uint8_t led_values = pow(2, lidar_ranges);

		led_register->led_groups.led_set_of_4 = led_values;

		volatile int read_values_now = 0;

		if (last_period > 4000)
			last_period = 5000;
		if (lidar_distance > 4000)
			lidar_distance = 5500;
		// Read the magnetometer

		uint8_t mag_setting = 0b11111100;
		return_value = HAL_I2C_Mem_Write(&hi2c2, magnet_wr, 0x02, 1, &mag_setting, 1, 10);

		uint8_t mag_data_buf[6];
		return_value = HAL_I2C_Mem_Read(&hi2c2, magnet_rd, 0x03, 1, mag_data_buf, 6, 10);
		int16_t x_mag = ((mag_data_buf[0] << 8) | mag_data_buf[1]);
		int16_t z_mag = ((mag_data_buf[2] << 8) | mag_data_buf[3]);
		int16_t y_mag = ((mag_data_buf[4] << 8) | mag_data_buf[5]);

		// I'm so confused it seems stw things roll is z yaw is x pitch is y??

		// Read acceleration values from ADXL345 registers
		uint8_t buffer[6];
		HAL_I2C_Mem_Read(&hi2c2, accel_rd, 0x32, 1, buffer, 6, 10);
		int16_t x_acceleration = ((buffer[1] << 8) | buffer[0]);
		int16_t y_acceleration = ((buffer[3] << 8) | buffer[2]);
		int16_t z_acceleration = ((buffer[5] << 8) | buffer[4]);


		// I think these calcs are wrong but wait and see
		int32_t x_input = y_acceleration*y_acceleration + z_acceleration*z_acceleration;
		int32_t y_input = x_acceleration*x_acceleration + z_acceleration*z_acceleration;
		int32_t z_input = x_acceleration*x_acceleration + y_acceleration*y_acceleration;
		float ax_roll = (180/3.14)*atan(y_acceleration/sqrt(y_input));
		float ax_pitch = (180/3.14)*atan(x_acceleration/sqrt(x_input));
		float ax_yaw = (180/3.14)*atan(z_acceleration/sqrt(z_input));
		float fYg = (roll_rate+86)*0.00875;

		//float ay_roll = atan(y/sqrt(y_input));
		//x = 0.95*(x + (roll_rate-57)*4/1000) + 0.05*ax_pitch;
		//dv = roll_rate;
		//sprintf(string_to_send, "%hu,%hu,%hd,%hd,%hd,%hd,%hd,%hd,%lf,%lf,%hd,%hd,%hd\r\n", last_period, lidar_distance*10, roll_rate, pitch_rate, yaw_rate, x_acceleration, y_acceleration, z_acceleration, x, ax_pitch, x_mag, z_mag, y_mag);
		//sprintf(string_to_send, "%lf,%lf,%lf,%hd,%hd,%hd\r\n", x_acceleration*0.0039, y_acceleration*0.0039, z_acceleration*0.0039, x_mag, z_mag, y_mag);

		// When gyro facing up
		// Notes on roll pitch and yaw
		// pitch appears to be along the y axis
		// roll_rate is from the z axis
		// yaw_rate is along the x axis


		//float fXg = (roll_rate+86)*0.00875;
		// Do some math to get distance

		float ALPHA = 0.1;
		//float xg;
		//float dpsx = fXg * ALPHA + (xg * (1.0-ALPHA));
		//xg = dpsx;
		//angleX = 0.95*(angleX + dpsx*0.1) + 0.05*ax_roll;

		// impliment a low pass filter on naked results
		//float roll;
		//roll = (roll_rate+86)*ALPHA + roll*(1-ALPHA);

		//sprintf(string_to_send, "%lf,%hd,%lf,%hd,%lf,%lf,%lf\r\n", dpsx, roll_rate, roll, yaw_rate,x_acceleration*0.0039,ax_yaw,angleX);



		// Okay lets understand each value being presented and use today as the time to figure them out
		/*
		 * 1. find the offset of acceleration to stablise it
		 * 2. see if can combine acceleration and velocity together to get some values for x y and z
		 * 3.
		 *  */
		// filter out information using low pass filter
		x_acc = ((float)x_acceleration) * 0.00390625 + (1.0-ALPHA)*x_acc;
		y_acc = ((float)y_acceleration) * 0.00390625 + (1.0-ALPHA)*y_acc;
		z_acc = ((float)z_acceleration) * 0.00390625 + (1.0-ALPHA)*z_acc;
		float a_roll = (180/3.14)*atan(y_acc/sqrt(x_acc*x_acc+z_acc*z_acc));
		float a_pitch = (180/3.14)*atan(x_acc/sqrt(y_acc*y_acc+z_acc*z_acc));
		float a_yaw = (180/3.14)*atan(z_acc/sqrt(y_acc*y_acc+x_acc*x_acc));
		//sprintf(string_to_send, "%lf,%lf,%lf,%lf,%lf,%lf\r\n", x_acc, y_acc, z_acc, a_roll, a_pitch, a_yaw);
		//sprintf(string_to_send, "%hd,%hd,%hd\r\n", x_acceleration, y_acceleration, z_acceleration);
		// DO cals on all gryo parts..

		x_v = ((pitch_rate-74)*0.00875* ALPHA + x_v * (1.0-ALPHA));
		y_v = (yaw_rate+86)*0.00875* ALPHA + y_v * (1.0-ALPHA);
		z_v = (roll_rate-100)*0.00875* ALPHA + z_v * (1.0-ALPHA);
		//sprintf(string_to_send, "%hd,%hd,%hd,%lf,%lf,%lf\r\n", roll_rate, pitch_rate, yaw_rate, x_v, y_v, z_v);


		// Now do math to combines readings
		x_angle = 0.95*(x_angle + x_v*0.09) + 0.05*a_roll;
		y_angle = 0.95*(y_angle + y_v*0.09) + 0.05*a_pitch;
		z_angle = 0.95*(z_angle + z_v*0.09) + 0.05*a_yaw;
		float angle = (y_angle + x_angle);
		float y = y_angle - x_angle;
		/// THIS one is useable
		//sprintf(string_to_send, "%lf,%lf,%lf,%lf,%lf,%lf\r\n", x_v, y_v, z_v, x_angle, y_angle, z_angle);
		//sprintf(string_to_send, "%lf,%lf,%lf\r\n", x_angle, y_angle, z_angle);
		//sprintf(string_to_send, "%lf,%lf\r\n",angle*100, y*100);

		// Investigating magnetic rates
		float mag_angle = atan(y_mag/x_mag);

		//sprintf(string_to_send,"%hd,%hd,%hd,%\r\n",x_mag, z_mag, y_mag);


		// Global roll pitch can't get yaw
		p_rps = ((yaw_rate+86)*0.00875*ALPHA)*(3.14/180) + p_rps * (1.0-ALPHA);
		q_rps = ((pitch_rate-74)*0.00875*ALPHA)*(3.14/180) + q_rps * (1.0-ALPHA);
		r_rps = ((roll_rate-100)*0.00875*ALPHA)*(3.14/180) + r_rps * (1.0-ALPHA);

		// trial of global pitch, roll using accelerometer
		float global_acc_roll = atanf(y_acc/sqrt(x_acc*x_acc+z_acc*z_acc));
		float global_acc_pitch = atanf(x_acc/sqrt(y_acc*y_acc+z_acc*z_acc));

		float phi_dot = p_rps + tanf(global_acc_pitch)*(sinf(global_acc_roll)*q_rps + cosf(global_acc_roll)*r_rps);
		float theta_dot = cosf(global_acc_roll)*q_rps - sinf(global_acc_roll)*r_rps;

		global_roll = 0.95*(global_roll + 0.1*phi_dot) + 0.05*global_acc_roll;
		global_pitch = 0.95*(global_pitch + 0.1*theta_dot) + 0.05*global_acc_pitch;

		// try this tomorrow!
		// idea to get si
		//float si_dot = sinf(global_acc_roll)*q_rps/cosf(global_acc_pitch) + cosf(global_acc_roll)*r_rps/cosf(global_acc_pitch);
		// So to find global yaw
		//global_yaw = global_yaw + 0.1*(si_dot);

		//sprintf(string_to_send, "%lf,%lf,\r\n", global_acc_roll, global_acc_pitch);
		//sprintf(string_to_send, "%lf,%lf,%lf,%lf,%lf,%lf\r\n", global_acc_roll, global_acc_pitch, phi_dot, theta_dot, global_roll, global_pitch);
		sprintf(string_to_send, "%lf,%lf,%lf,%lf,\r\n", global_acc_roll, global_acc_pitch, global_roll, global_pitch);
		// Idea to get XYZ using magnetometer


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
	// code the IQR handler here


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x2000090E;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 47;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 47;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 38400;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin
                           MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin
                          |MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LEFT_Pin DOWN_Pin UP_Pin */
  GPIO_InitStruct.Pin = LEFT_Pin|DOWN_Pin|UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : RIGHT_Pin */
  GPIO_InitStruct.Pin = RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RIGHT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void PTU_callback_function(){
	static uint8_t count = 0;
	if(count == 0){
		//setup_transmission(string_to_send, &USART1_PORT);
		count++;
		return;
	}

	//dx = dx + (dv-57)*0.00875*dt;
	//volatile uint16_t dv;
	//sprintf(test_string, "%lf,\r\n");

//
//	if(count == 1){
//		SerialOutputString(string_to_send, &USART1_PORT);
//		count++;
//		return;
//	}

//	LedRegister *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
//	led_register->led_groups.led_pair_1 ^= 0b10;
	setup_transmission(string_to_send, &USART1_PORT);
	//setup_transmission(test_string, &USART1_PORT);
	// static uint8_t counter = 1;
	// string_to_send[0] = counter++;
	//SerialOutputString(string_to_send, &USART1_PORT);

	// usart1_tx_push(string_to_send);
	// when called enable interrupts
	//enable_USART_interrupts(&USART1_PORT);
	// uint8_t *test = "new wtf hty\r\n";

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
