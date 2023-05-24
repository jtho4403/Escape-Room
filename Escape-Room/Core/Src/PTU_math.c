#include "ptu_i2c.h"
#include "ptu_definitions.h"
#include <math.h>
#include <stdio.h>

#define ALPHA 0.1
/*
 * PTU_math.c
 *
 *  Created on: 24 May 2023
 *      Author: David
 */

void calculate_roll_pitch_yaw(int16_t* accel_buff, int16_t* gyro_buff, uint8_t* output_string){
	static float x_acc = 0.0;
	static float y_acc = 0.0;
	static float z_acc = 0.0;
	static float x_v = 0.0;
	static float y_v = 0.0;
	static float z_v = 0.0;
	static float x_angle = 0.0;
	static float y_angle = 0.0;
	static float z_angle = 0.0;


	x_acc = ((float)accel_buff[0]) * 0.00390625 + (1.0-ALPHA)*x_acc;
	y_acc = ((float)accel_buff[1]) * 0.00390625 + (1.0-ALPHA)*y_acc;
	z_acc = ((float)accel_buff[2]) * 0.00390625 + (1.0-ALPHA)*z_acc;
	float a_roll = (180/3.14)*atan(y_acc/sqrt(x_acc*x_acc+z_acc*z_acc));
	float a_pitch = (180/3.14)*atan(x_acc/sqrt(y_acc*y_acc+z_acc*z_acc));
	float a_yaw = (180/3.14)*atan(z_acc/sqrt(y_acc*y_acc+x_acc*x_acc));
	//sprintf(string_to_send, "%lf,%lf,%lf,%lf,%lf,%lf\r\n", x_acc, y_acc, z_acc, a_roll, a_pitch, a_yaw);
	//sprintf(string_to_send, "%hd,%hd,%hd\r\n", x_acceleration, y_acceleration, z_acceleration);


	x_v = (float)((gyro_buff[1]-74)*0.00875* ALPHA + x_v * (1.0-ALPHA));
	y_v = (float)(gyro_buff[2]+86)*0.00875* ALPHA + y_v * (1.0-ALPHA);
	z_v = (float)(gyro_buff[0]-100)*0.00875* ALPHA + z_v * (1.0-ALPHA);
	//sprintf(string_to_send, "%hd,%hd,%hd,%lf,%lf,%lf\r\n", roll_rate, pitch_rate, yaw_rate, x_v, y_v, z_v);


	// Now do math to combines readings
	x_angle = 0.95*(x_angle + x_v*0.09) + 0.05*a_roll;
	y_angle = 0.95*(y_angle + y_v*0.09) + 0.05*a_pitch;
	z_angle = 0.95*(z_angle + z_v*0.09) + 0.05*a_yaw;
	float angle = (y_angle + x_angle);
	float y = y_angle - x_angle;
	/// THIS one is useable
	sprintf(output_string, "%lf,%lf,%lf,%lf,%lf,%lf\r\n", x_v, y_v, z_v, x_angle, y_angle, z_angle);
	//sprintf(string_to_send, "%lf,%lf,%lf\r\n", x_angle, y_angle, z_angle);
	//sprintf(string_to_send, "%lf,%lf\r\n",angle*100, y*100);


}
