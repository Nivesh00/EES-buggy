
#include "buggy.h"

#define Device_Address 0x68	/*Device Address/Identifier for MPU6050*/

/**
 * Gyroscope registers are defined here
 * Not all registers are shown, some are left out because they were
 * not used, e.g. I2C registers (master and alaves are ignored)
*/
#define PWR_MGMT_1   0x6B       /*<! powermode and clocksource                         >*/
#define SMPLRT_DIV   0x19       /*<! sample rate divider - output rate/(1 + SMLRT_DIV) >*/
#define CONFIG       0x1A       /*<! sampling external sigal - normally 0              >*/
#define GYRO_CONFIG  0x1B       /*<! self-test and config - full scale 2000 °/s        >*/
#define ACCEL_CONFIG 0x1c
#define FIFO_EN      0x23       /* Buffer for Gyroscope */
#define INT_ENABLE   0x38       /*<! interrupt enabled - enables interrupt generation  >*/
#define ACCEL_XOUT_H 0x3B       /*<! x-axis accelerometer value                        >*/
#define ACCEL_YOUT_H 0x3D       /*<! y-axis accelerometer value                        >*/
#define ACCEL_ZOUT_H 0x3F       /*<! z-axis accelerometer value                        >*/
#define GYRO_XOUT_H  0x43       /*<! x-axis gyroscope value                            >*/
#define GYRO_YOUT_H  0x45       /*<! y-axis gyroscope value                            >*/
#define GYRO_ZOUT_H  0x47       /*<! z-axis gyroscope value                            >*/


/**
 * Constant values for registers etc...
*/
#define GYRO_CONFIG_VALUE   24       /* no self-test, set fullscale only to 2000 °/s   */
#define ACCEL_CONFIG_VALUE  24       /* no self test, set fullscale to 16g             */
#define FIFO_EN_VALUE       16       /* only Gyroscope-z buffer is enavbled            */
#define ACCEL_sensitivity   16384    /* raw values of ACCEL_OUT must be divided by it */
#define GYRO_sensitivity    131      /* raw values of GYRO_OUT must be divided by it */

extern int fd;
extern float DEVIATION;
extern double DEVIATION_SUM;

//extern long double Gx_offset, Gy_offset, Gz_offset;

void MPU6050_Init();

short read_full_register(int addr);

//void calcOffset();

void read_data();
