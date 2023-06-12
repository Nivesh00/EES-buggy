#include "gyro.h"


void MPU6050_Init()
{
	
	wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0xFF);					/* Write to sample rate register */
	wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x01);					/* Write to power management register, use z-axis as ref */
	wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, GYRO_CONFIG_VALUE);		/* Write to Gyro Configuration register */
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

void read_data()
{
	float Gyro_z;

	while(true)
	{
		Gyro_z = read_full_register(GYRO_ZOUT_H);
		DEVIATION = Gyro_z/GYRO_sensitivity;

		if(abs(DEVIATION) < 3.0)
			DEVIATION = 0.0;

		DEVIATION_SUM += DEVIATION/100;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}