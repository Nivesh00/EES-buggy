#include "buggy.h"
#include "sensor.h"
#include "gyro.h"

#define LED             25 // Brake light

/* Path which the buggy follows */
#define OBJ_RECOG        0 // recognises objects and turns/brakes to avoid
#define CURVE_90DEG      1 // follows a curved path of 90°
#define STAY_ON_PATH     2 // goes in one direction even if buggy is moved

int PATH = CURVE_90DEG;      // current path to folow

// I2C device address of gyroscope
int fd;

// variables for distance, angle of deviation and deviation per second
double OBJ_DISTANCE, DEVIATION_SUM = 0.0;
float DEVIATION;

// checks whether last turn was right or left turn
bool LAST_TURN = true;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
	std::cout << "Strg-C Programmende" << std::endl << " ---> SUM Deviation: " << DEVIATION_SUM << std::endl;
	// Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
    Release();
    exit(signum);
}


/* Thread functions */

// Thread to read current deviation from gyroscope
PI_THREAD (readDeviation)
{
    read_data();
    return 0;
}

// Thread to read current distsnce from sensor
PI_THREAD (readDistance)
{
    sensorCalcDistance();
    return 0;
}


int main()
{
    wiringPiSetupGpio();
    sensorPinSetUp();
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 0);

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
        switch (PATH)
        {
            case CURVE_90DEG:
            
                if(!LAST_TURN)
                    Speed(SPEED - 25, SPEED);
                else
                    Speed(SPEED, SPEED  - 25);

                while(abs(DEVIATION_SUM) < 85)
                {   
                    //if(OBJ_DISTANCE < DISTANCE_MIN_TURN_cm) break;
                    
                    Forward();
                    std::cout << std::endl << "Deviation: " << DEVIATION << " °/s" << std::endl;
                    std::cout << "Deviation sum: " << DEVIATION_SUM << " °/s" << std::endl;
                }

                PATH = OBJ_RECOG;

            break;
            
            case OBJ_RECOG:

                Speed(SPEED, SPEED);
                
                if(OBJ_DISTANCE > DISTANCE_MIN_TURN_cm)
                {
                    digitalWrite(LED, 0);
                    std::cout << std::endl << "No obstacle found" << std::endl;
                    std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
                    std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
                    Forward();
                }
                if(OBJ_DISTANCE < DISTANCE_MIN_TURN_cm 
                                            && OBJ_DISTANCE > DISTANCE_MIN_BRAKE_cm)
                {
                    digitalWrite(LED, 1);
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
                        if(OBJ_DISTANCE < DISTANCE_MIN_BRAKE_cm) break;
                        std::cout << std::endl << "Obstacle found, turn to avoid" << std::endl;
                        std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
                        std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
                    }
                }
                if(OBJ_DISTANCE < DISTANCE_MIN_BRAKE_cm)
                {
                    digitalWrite(LED, 1);
                    Brake();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    if(LAST_TURN)
                    {
                        BackwardRight();
                        LAST_TURN = false;
                    }
                    else
                    {
                        BackwardLeft();
                        LAST_TURN = true;
                    }
                    while(OBJ_DISTANCE < DISTANCE_MIN_BRAKE_cm)
                    {
                        std::cout << std::endl << "Obstacle found, brake immediately" << std::endl;
                        std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
                        std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
                    }
                }

            break;

            case STAY_ON_PATH:
                
                Speed(SPEED, SPEED);

                while(true)
                {
                    if(DEVIATION_SUM > 1.0) Speed(SPEED + 50, SPEED);
                    else if (DEVIATION_SUM < -1.0) Speed(SPEED, SPEED + 50);
                    else Speed(SPEED, SPEED);

                    Forward();

                    std::cout << "Distance: " << OBJ_DISTANCE << " [cm]" << std::endl;
                    std::cout << "Deviation: " << DEVIATION << " °/s" << std::endl;
                }

            break;
        }
    }

    Speed(0, 0);
    Brake();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Release();
    
    return 0;
}
