#include "mainklocki.h"

#include <algorithm>
#include <cmath>

MainKlocki::MainKlocki(unsigned robotId, const std::set<ColorBox::Color>& boxColorSet)
    : maxLength(50.0), robotId(robotId),boxColorSet(boxColorSet)
{
}

void MainKlocki::getCommands(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec)
{
    auto it = std::find(robotVec.begin(), robotVec.end(), robotId);
    if (it == robotVec.end())
    {
        RobotCommands robotCommands = {robotId, 0, 0, 0};
        emit robotCommandUpdate(robotCommands);
        return;
    }
    Robot& robotDevice = *it;

    std::vector<ColorBox> colorBoxVecFiltered;
    for (ColorBox& colorBox : colorBoxVec)
    {
        if (boxColorSet.find(colorBox.color) != boxColorSet.end())
            colorBoxVecFiltered.push_back(colorBox);
    }

    vect2d moveVec;
    for(ColorBox& colorBox : colorBoxVecFiltered)
    {
        vect2d robot;
        robot.x = robotDevice.xCentimeters;
        robot.y = robotDevice.yCentimeters;
        vect2d box;
        box.x = colorBox.xCentimeters;
        box.y = colorBox.yCentimeters;

        vect2d normalVec;

        normalVec.x = robot.x-box.x;
        normalVec.y = robot.y-box.y;
        double length = sqrt(normalVec.x*normalVec.x+normalVec.y*normalVec.y);

        if(length<maxLength)
        {
            double lengthRatio = (maxLength - length) / maxLength; //values between 0 and 1
            double areaMultiplicator = colorBox.area; //easier to modify later

            moveVec.x += normalVec.x * lengthRatio * areaMultiplicator;
            moveVec.y += normalVec.y * lengthRatio * areaMultiplicator;
        }
    }

    RobotCommands robotCommands = {robotId, moveVec.x, moveVec.y, 0};

    emit robotCommandUpdate(robotCommands);
}
