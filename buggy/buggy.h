#include <thread>
#include <chrono>
#include <signal.h>
#include <iostream>
#include <vector>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include "source/adafruitmotorhat.h"


//default speed of motors: 150 rpm
#define SPEED 150



void Speed(int Left, int Right);

/**
 * Note: due to the way the buggy is assembled, the forward and backward
 * functions are switched, i.e when the motors rotate in forward direction,
 * Buggy goes backwards and vice versa
*/

void Backward();


void BackwardLeft();


void BackwardRight();


void Forward();


void ForwardLeft();


void ForwardRight();


void Brake();


void Release();