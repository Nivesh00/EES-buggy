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

#define SPEED 150


void Speed(int Left, int Right);


void Backward();


void BackwardLeft();


void BackwardRight();


void Forward();


void ForwardLeft();


void ForwardRight();


void Brake();


void Release();


void CurveRight();


void CurveLeft();