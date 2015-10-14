#include "mainjoystick.h"
#include <iostream>

MainJoystick::MainJoystick(unsigned robotId, QString device)
    : tempVal(0.0), robotId(robotId), device(device),requestChecker(this), tempCounter(0), padRequest(false)
{
    QObject::connect(&requestChecker, &QTimer::timeout, this, &MainJoystick::checkRequest);
    requestChecker.start(200);
    joy.setJoystick(0);
}

void MainJoystick::getCommands()
{

    joy.getdata();


       /* Tutaj zawiera sie legenda, ktora komorka axis i buttons odnosi sie do ktorej osi.
        * Bardzo prosze nie ruszac. To moze byc przydatne przy dalszych zmianach.

        joystick.at(current_joystick)->axis[0]=joy->axis[0];
        joystick.at(current_joystick)->axis[1]=joy->axis[1];
        //joystick.at(current_joystick)->axis[2]=joy->axis[2];
        joystick.at(current_joystick)->axis[3]=joy->axis[3];
        joystick.at(current_joystick)->axis[4]=joy->axis[4];
        //joystick.at(current_joystick)->axis[5]=joy->axis[5];

        joystick.at(current_joystick)->button[0] = joy->buttons[0]; // A
        joystick.at(current_joystick)->button[1] = joy->buttons[1]; // B
        joystick.at(current_joystick)->button[2] = joy->buttons[2]; // X
        joystick.at(current_joystick)->button[3] = joy->buttons[3]; // Y
        joystick.at(current_joystick)->button[4] = joy->buttons[4]; //LB
        joystick.at(current_joystick)->button[5] = joy->buttons[5]; //RB
        joystick.at(current_joystick)->button[6] = joy->buttons[6]; //back
        joystick.at(current_joystick)->button[7] = joy->buttons[7]; //start */

    RobotCommands robotCommands = {robotId, joy.axis[4]*0.0305241 , joy.axis[3] *0.0305241, joy.axis[0]*0.0305241};
     //  stala 0.000915 powoduje ze MAX wartosc w strukturze jest mniejsza niz 30
    emit robotCommandUpdate(robotCommands);

}

void MainJoystick::checkRequest()
{
    joy.getdata();
//     Jako guzika przejscia uzylem przycisku "A"
    if(joy.buttons[0] == 1)
        emit gamePadRequest(true);
    else
        emit gamePadRequest(false);
}

