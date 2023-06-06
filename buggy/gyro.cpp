#include "gyro.h"


void MPU6050_Init()
{
	
	wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0x07);					/* Write to sample rate register */
	wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x01);					/* Write to power management register, use z-axis as ref */
	//wiringPiI2CWriteReg8 (fd, CONFIG, 0);							/* Write to Configuration register */
	wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, GYRO_CONFIG_VALUE);		/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8 (fd, ACCEL_CONFIG, ACCEL_CONFIG_VALUE);
	//wiringPiI2CWriteReg8 (fd, INT_ENABLE, 0x01);	/*Write to interrupt enable register */
}

/**
 * Reads register and provides raw values
*/
short read_full_register(int addr)
{
	short MSB, LSB, full_value;
	MSB = wiringPiI2CReadReg8(fd, addr);
	LSB = wiringPiI2CReadReg8(fd, addr + 1);
	full_value = (MSB << 8) | LSB;
	return full_value;
}

/*
void calcOffset()
{
	for(int loop = 0; loop < 1000; loop++)
	{
		Gx_offset += read_full_register(GYRO_XOUT_H)/131;
		Gy_offset += read_full_register(GYRO_YOUT_H)/131;
		Gz_offset += read_full_register(GYRO_ZOUT_H)/131;
	}

	Gx_offset = Gx_offset/1000;
	Gy_offset = Gy_offset/1000;
	Gz_offset = Gz_offset/1000;

	std::cout << "Gx_off: " << Gx_offset << "Gy_off: " << Gy_offset<< "Gz_off: " << Gz_offset << std::endl;
}
*/

void read_data()
{
	/*float Acc_x,Acc_y,Acc_z;
	float Gyro_x,Gyro_y,Gyro_z;
	float Ax=0, Ay=0, Az=0;
	float Gx=0, Gy=0, Gz=0;*/
	float Gyro_z;

	/*Read raw value of Accelerometer and gyroscope from MPU6050
	Acc_x = read_full_register(ACCEL_XOUT_H);
	Acc_y = read_full_register(ACCEL_YOUT_H);
	Acc_z = read_full_register(ACCEL_ZOUT_H);
	
	Gyro_x = read_full_register(GYRO_XOUT_H);
	Gyro_y = read_full_register(GYRO_YOUT_H);*/

	while(true)
	{
		Gyro_z = read_full_register(GYRO_ZOUT_H);
		DEVIATION = Gyro_z/GYRO_sensitivity;
		DEVIATION_SUM += DEVIATION/2;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	
	
	/* Divide raw value by sensitivity scale factor 
	Ax = Acc_x/16384.0;
	Ay = Acc_y/16384.0;
	Az = Acc_z/16384.0;
	

	Gx = Gyro_x/131;
	Gy = Gyro_y/131;*/
	

	//printf("\n Gz=%.3f\n", Gz);
	//printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\n",Gx,Gy,Gz,Ax,Ay,Az);
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

		
}