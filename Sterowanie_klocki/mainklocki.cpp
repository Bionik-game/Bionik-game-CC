#include "mainklocki.h"

#include <algorithm>

MainKlocki::MainKlocki(unsigned robotId, const std::set<ColorBox::Color>& boxColorSet)
    : tempVal(0.0), robotId(robotId),boxColorSet(boxColorSet)
{
}

RobotCommands MainKlocki::getCommands(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec)
{
    auto it = std::find(robotVec.begin(), robotVec.end(), robotId);
    if (it == robotVec.end())
    {
        RobotCommands robotCommands = {robotId, 0, 0, 0};
        return robotCommands;
    }

    std::vector<ColorBox> colorBoxVecFiltered;
    for (ColorBox& colorBox : colorBoxVec)
    {
        if (boxColorSet.find(colorBox.color) != boxColorSet.end())
            colorBoxVecFiltered.push_back(colorBox);

    }

    // Jakieś tam dziwne obliczenia...

    tempVal += 1.3;
    RobotCommands robotCommands = {robotId, tempVal, tempVal*2, tempVal/2};

    emit robotCommandUpdate(robotCommands);
}
