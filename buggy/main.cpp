#include "buggy.h"
#include "sensor.h"
#include "gyro.h"


int fd;
double OBJ_DISTANCE, DEVIATION_SUM = 0.0;
float DEVIATION;
bool LAST_TURN = true;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
	std::cout << "Strg-C Programmende" << " --->SUM Deviation: " << DEVIATION_SUM << std::endl;
	// Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
    Release();
    exit(signum);
}

PI_THREAD (readDeviation)
{
    read_data();
    return 0;
}

PI_THREAD (readDistance)
{
    sensorCalcDistance();
    return 0;
}


int main()
{
    wiringPiSetupGpio();
    sensorPinSetUp();

    /*Initializes I2C with device Address*/
    fd = wiringPiI2CSetup(Device_Address);
    
    /* Initializes MPU6050 */
    MPU6050_Init();		         

    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    /* Initialise threads */

    /**
     * Thread for distance, every 50ms, global variable OBJ_DISTANCE is updated
     * Main program then uses this value to make its next decision
    */
    piThreadCreate(readDistance);

    /**
     * Thread for deviation, every 50ms, global variable DEVIATION is updated
     * Main progam then uses this value to make its next decision
    */
    piThreadCreate(readDeviation);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    DEVIATION_SUM = 0;

    while(true)
    {
        Speed(SPEED, SPEED);

        if(OBJ_DISTANCE > DISTANCE_MIN_TURN_cm)
        {
            std::cout << std::endl << "No obstacle found" << std::endl;
            std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
            std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
            Forward();
            while(DEVIATION_SUM < 0)
                ForwardLeft();
            while(DEVIATION_SUM > 0)
                ForwardRight();
        }
        else if(OBJ_DISTANCE < DISTANCE_MIN_TURN_cm 
                                    && OBJ_DISTANCE > DISTANCE_MIN_BRAKE_cm)
        {
            if(LAST_TURN)
            {
                ForwardRight();
                LAST_TURN = false;
            }
            else
            {
                ForwardLeft();
                LAST_TURN = true;
            }
            while(OBJ_DISTANCE < DISTANCE_MIN_TURN_cm)
            {
                std::cout << std::endl << "Obstacle found, turn to avoid" << std::endl;
                std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
                std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
            }
        }
        else
        {
            Brake();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(LAST_TURN)
            {
                ForwardRight();
                LAST_TURN = false;
            }
            else
            {
                ForwardLeft();
                LAST_TURN = true;
            }
            while(OBJ_DISTANCE < DISTANCE_MIN_TURN_cm)
            {
                std::cout << std::endl << "Obstacle found, brake immediately" << std::endl;
                std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
                std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
            }
        }
    }

    Speed(0, 0);
    Brake();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Release();
    
    return 0;
}
