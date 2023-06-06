#include "buggy.h"

#define DISTANCE_MIN_TURN_cm 20
#define DISTANCE_MIN_BRAKE_cm 10
#define TIMEOUT_us 10000                //4m reliable distance, i.e. 
#define SoundSpeed_mps 343
#define SoundSpeed_mpms 0.343
#define SoundSpeed_mpus 0.000343
#define TRIGGER_PIN 27
#define ECHO_PIN 22

extern double OBJ_DISTANCE;

void sensorPinSetUp();
void sensorCalcDistance();
