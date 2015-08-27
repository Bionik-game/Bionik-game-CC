#ifndef MAINKLOCKI_H
#define MAINKLOCKI_H

#include <QThread>

#include "Common-utils/mapelements.h"
#include "Common-utils/datacollector.h"

class MainKlocki : public QThread
{
    Q_OBJECT

private:
    typedef DataCollector<RobotPosition, std::vector<ColorBoxPosition>> MyDataCollector;
    MyDataCollector dataCollector;

    double tempVal;

public:
    MainKlocki();

signals:
    void robotCommandUpdate(RobotCommands robotCommands);

private slots:
    void processData();

public slots:
    void updateRobotPosition(RobotPosition robotPosition);
    void updateBoxesPositions(std::vector<ColorBoxPosition> boxesPositionVector);
};

#endif // MAINKLOCKI_H
