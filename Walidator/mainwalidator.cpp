#include "mainwalidator.h"

#include <iostream>
MainWalidator::MainWalidator()
{
    QObject::connect(&dataCollector, &MyDataCollector::allDataNew, this, &MainWalidator::processData);
    QObject::connect(&dataCollector, &MyDataCollector::dataDropped,
                     [](unsigned int no)
    {
        std::cerr << "Element number " << no << "was replaced before rest of data arrived in MainWalidator" << std::endl;
    });
}

void MainWalidator::processData()
{
    dataCollector.get<std::vector<Robot>>(0);
    dataCollector.get<std::vector<ColorBox>>(1);
    RobotCommands robotCommandsCorrected = dataCollector.get<RobotCommands>(2);
    emit robotCommandUpdateCorrected(robotCommandsCorrected);
}

void MainWalidator::gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec)
{
    dataCollector.set(0, robotVec);
    dataCollector.set(1, colorBoxVec);
}

void MainWalidator::robotCommandUpdateRaw(RobotCommands robotCommands)
{
    dataCollector.set(2, robotCommands);
}

