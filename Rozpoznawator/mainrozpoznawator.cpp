#include "mainrozpoznawator.h"
#include <QThread>

MainRozpoznawator::MainRozpoznawator()
    : rythm(this), robotPositionX(30)
{
    /**
     * Wystartowanie zegara z czasem 0 powoduje jego natychmiastowe
     * włączanie kiedy tylko nie ma żadnych innych zdarzeń
     * do przetworzenia w pętli.
     */
    QObject::connect(&rythm, &QTimer::timeout, this, &MainRozpoznawator::mainWork);
    rythm.start(0);
}

std::vector<Robot> MainRozpoznawator::updateRobotPosition()
{
    std::vector<Robot> robots;

    Robot robotPosition = {1, robotPositionX, 40, 2.3};
    robotPositionX+= 1;

    robots.push_back(robotPosition);
    return robots;
}

std::vector<ColorBox> MainRozpoznawator::updateBoxesPositions()
{
    std::vector<ColorBox> boxesPositionVector;
    for (int i = 0; i < 10; i++)
    {
        if (i%2)
        {
            ColorBox colorBoxPosition = {ColorBox::BLUE, (const unsigned long) i+10, (const unsigned long)  i*2, 1.5};
            boxesPositionVector.push_back(colorBoxPosition);
        }
        else
        {
            ColorBox colorBoxPosition = {ColorBox::GREEN, (const unsigned long) 2*i+50, (const unsigned long) i+9, 1.0};
            boxesPositionVector.push_back(colorBoxPosition);
        }
    }
    return boxesPositionVector;
}

void MainRozpoznawator::mainWork()
{
    int sleepTime = 100 + qrand()%900;
    QThread::msleep(sleepTime);

    emit gameState(updateRobotPosition(), updateBoxesPositions());
}

