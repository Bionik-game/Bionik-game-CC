#include "mainpriorytetyzator.h"

MainPriorytetyzator::MainPriorytetyzator()
    : gamePad(false)
{
}

#include <iostream>
void MainPriorytetyzator::gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec)
{
    if (gamePad)
        emit getCommandsJoystick();
    else
        emit getCommandsColors(robotVec, colorBoxVec);
}

void MainPriorytetyzator::gamePadRequest(bool flag)
{
    gamePad = flag;
}

