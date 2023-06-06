#include "buggy.h"

#define DISTANCE_MIN_TURN_cm 20
#define DISTANCE_MIN_BRAKE_cm 10
#define TIMEOUT_us 10000                //time taken for sound to travel 4 meters in microseconds
#define SoundSpeed_mps 343
#define SoundSpeed_mpms 0.343
#define SoundSpeed_mpus 0.000343
#define TRIGGER_PIN 27
#define ECHO_PIN 22

extern double OBJ_DISTANCE;

/**
 * BCM pin numbering is used
*/
void sensorPinSetUp();
void sensorCalcDistance();
