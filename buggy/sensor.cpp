#include "sensor.h"


/**
 * Sets echo and trigger pin 
*/
void sensorPinSetUp()
{
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(ECHO_PIN, 0);
    digitalWrite(TRIGGER_PIN, 0);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


void sensorCalcDistance()
{
    std::chrono::steady_clock::time_point CHECK;
    std::chrono::steady_clock::time_point START;
    std::chrono::steady_clock::time_point STOP;
    double TIME, DISTANCE;
    bool TIMEOUT;

    while(true)
    {
        TIMEOUT = false;
        digitalWrite(TRIGGER_PIN, 1);
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        digitalWrite(TRIGGER_PIN, 0);

        CHECK = std::chrono::steady_clock::now();
        /**
         * start stopwatch as soon as ECHO_PIN is 1
         * If time to recieve signal is too big, resent signal
        */
        while (!digitalRead(ECHO_PIN))
        {
            START = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::microseconds> (START - CHECK).count() > TIMEOUT_us)
            {
                TIMEOUT = true;
                break;
            }
        }

        if(TIMEOUT) continue;

        /** 
         * Stop stopwatch as soon as incoming signal becomes 0 again
         * OR if time taken is less than TIMEOUT_us
        */
        while(digitalRead(ECHO_PIN))
            STOP = std::chrono::steady_clock::now();

        /**
         * Calculate timetaken
        */
        TIME = std::chrono::duration_cast<std::chrono::microseconds> (STOP - START).count();

        /**
         * Calculate distance and convert to centimeters
        */
        DISTANCE = ((TIME * SoundSpeed_mpus)/2) * 100;

        OBJ_DISTANCE = DISTANCE;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //return DISTANCE;

}