#include "mainklocki.h"

MainKlocki::MainKlocki()
    : tempVal(0.0)
{
    QObject::connect(&dataCollector, &MyDataCollector::allDataNew, this, &MainKlocki::processData);
}

void MainKlocki::processData()
{
    RobotPosition robotPosition = dataCollector.get<RobotPosition>(0);
    std::vector<ColorBoxPosition> boxes = dataCollector.get<std::vector<ColorBoxPosition>>(1);
    // Jakieś tam dziwne obliczenia...

    RobotCommands robotCommands = {tempVal, tempVal*2, tempVal/2};
    emit robotCommandUpdate(robotCommands);

    tempVal += 1.3;
}



void MainKlocki::updateRobotPosition(std::vector<RobotPosition> robotPosition)
{
    dataCollector.set(0, robotPosition);
}


void MainKlocki::updateBoxesPositions(std::vector<ColorBoxPosition> boxesPositionVector)
{
    dataCollector.set(1, boxesPositionVector);
}
