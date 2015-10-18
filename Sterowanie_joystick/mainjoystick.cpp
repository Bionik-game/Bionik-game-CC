#include "mainjoystick.h"

MainJoystick::MainJoystick(unsigned robotId, QString device)
    : tempVal(0.0), robotId(robotId), device(device),requestChecker(this), tempCounter(0), padRequest(false)
{
    QObject::connect(&requestChecker, &QTimer::timeout, this, &MainJoystick::checkRequest);
    requestChecker.start(200);
}

RobotCommands MainJoystick::getCommands()
{
    RobotCommands robotCommands = {robotId, -tempVal, -tempVal*2, -tempVal/2};
    tempVal += 1.3;
    emit robotCommandUpdate(robotCommands);
}

void MainJoystick::checkRequest()
{
    tempCounter += 1;

    if (tempCounter % 10 == 0)
        padRequest = !padRequest;

    emit gamePadRequest(padRequest);
}

