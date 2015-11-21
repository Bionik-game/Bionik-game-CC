#include "mainklocki.h"

#include <algorithm>
#include <cmath>

using namespace std;

MainKlocki::MainKlocki(unsigned robotId, const std::set<ColorBox::Color>& boxColorSet)
    : maxLength(300.0), robotId(robotId),boxColorSet(boxColorSet)
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
    moveVec.x = 0;
    moveVec.y = 0;

    cout << colorBoxVecFiltered.size() << endl;

    for(ColorBox& colorBox : colorBoxVecFiltered)
    {
       // cout << colorBox.color << " " << colorBox.xCentimeters << " " << colorBox.yCentimeters << endl;
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
        normalVec.x = normalVec.x/length;       //Dlugosc normalnego wektora ma byc 1
        normalVec.y = normalVec.y/length;

        //cout << normalVec.x << " " << normalVec.y << endl;

        float robotAngleRad = robotDevice.rotationRadians;
        float cs = cos(robotAngleRad);
        float sn = sin(robotAngleRad);
        float tempX = normalVec.x*cs - normalVec.y*sn;
        float tempY = normalVec.x * sn + normalVec.y * cs;
        normalVec.x = tempX;
        normalVec.y = tempY;


        if(length<maxLength)
        {
            double lengthRatio = (maxLength - length) / maxLength; //values between 0 and 1
            double areaMultiplicator = 1;//colorBox.area; //easier to modify later
         //   cout << lengthRatio << endl;
            moveVec.x += normalVec.x * lengthRatio * areaMultiplicator;
            moveVec.y += normalVec.y * lengthRatio * areaMultiplicator;

        //    cout << moveVec.x << " " << moveVec.y << endl;
        }
    }
    moveVec.x = moveVec.x*1000;
    moveVec.y = moveVec.y*1000;
    RobotCommands robotCommands = {robotId, moveVec.x, moveVec.y, 0};

      //  cout << robotCommands.robotId << " " << robotCommands.xCentimetersPerSecond << " " << robotCommands.yCentimetersPerSecond << endl;

    emit robotCommandUpdate(robotCommands);
}
