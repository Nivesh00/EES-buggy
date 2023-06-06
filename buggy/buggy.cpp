#include "buggy.h"


int s = 100;


AdafruitMotorHAT hat;


auto wheelLeft { hat.getMotor(1) };
auto wheelRight { hat.getMotor(4) };

void Speed(int Left, int Right)
{
    Left = Left > 15 ? Left : 15;
    wheelLeft->setSpeed(Left - 15);
    wheelRight->setSpeed(Right);
}


void Backward()
{
    wheelLeft->run (AdafruitDCMotor::kForward);
    wheelRight->run (AdafruitDCMotor::kForward);
}


void BackwardLeft()
{
    wheelLeft->run(AdafruitDCMotor::kBrake);
    wheelRight->run(AdafruitDCMotor::kForward);
}


void BackwardRight()
{
    wheelLeft->run(AdafruitDCMotor::kForward);
    wheelRight->run(AdafruitDCMotor::kBrake);
}


void Forward()
{
    wheelLeft->run(AdafruitDCMotor::kBackward);
    wheelRight->run(AdafruitDCMotor::kBackward);
}


void ForwardLeft()
{
    wheelLeft->run(AdafruitDCMotor::kBrake);
    wheelRight->run(AdafruitDCMotor::kBackward);
}


void ForwardRight()
{
    wheelLeft->run(AdafruitDCMotor::kBackward);
    wheelRight->run(AdafruitDCMotor::kBrake);
}


void Brake()
{
    wheelLeft->run(AdafruitDCMotor::kBrake);
    wheelRight->run(AdafruitDCMotor::kBrake);
}

void Release()
{
    wheelLeft->run(AdafruitDCMotor::kRelease);
    wheelRight->run(AdafruitDCMotor::kRelease);
}

void CurveRight()
{
    Speed(SPEED, SPEED/2);
    wheelLeft->run(AdafruitDCMotor::kBackward);
    wheelRight->run(AdafruitDCMotor::kBackward);
}


void CurveLeft()
{
    Speed(SPEED/2, SPEED);
    wheelLeft->run(AdafruitDCMotor::kBackward);
    wheelRight->run(AdafruitDCMotor::kBackward);
}