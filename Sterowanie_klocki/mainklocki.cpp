#include "mainklocki.h"

#include <algorithm>

MainKlocki::MainKlocki(unsigned robotId, const std::set<ColorBox::Color>& boxColorSet)
    : tempVal(0.0), robotId(robotId),boxColorSet(boxColorSet)
{
    QObject::connect(&dataCollector, &MyDataCollector::allDataNew, this, &MainKlocki::processData);
}

void MainKlocki::processData()
{
    Robot robotPosition = dataCollector.get<Robot>(0);
    std::vector<ColorBox> boxes = dataCollector.get<std::vector<ColorBox>>(1);
    // Jakieś tam dziwne obliczenia...

    RobotCommands robotCommands = {robotPosition.robotId, tempVal, tempVal*2, tempVal/2};
    emit robotCommandUpdate(robotCommands);

    tempVal += 1.3;
}

void MainKlocki::gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec)
{
    auto it = std::find(robotVec.begin(), robotVec.end(), robotId);
    dataCollector.set(0, *it);

    std::vector<ColorBox> colorBoxVecFiltered;
    for (ColorBox& colorBox : colorBoxVec)
    {
        if (boxColorSet.find(colorBox.color) != boxColorSet.end())
            colorBoxVecFiltered.push_back(colorBox);

    }
    dataCollector.set(1, colorBoxVecFiltered);
}
