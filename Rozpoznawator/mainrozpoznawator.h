#ifndef MAINROZPOZNAWATOR_H
#define MAINROZPOZNAWATOR_H

#include <vector>

#include <QThread>
#include <QTimer>

#include "Common-utils/mapelements.h"

class MainRozpoznawator : public QThread
{
    Q_OBJECT

private:
    QTimer rythm;
    unsigned long robotPositionX;

public:
    MainRozpoznawator();

private:
    void updateRobotPosition();
    void updateBoxesPositions();

signals:
    void robotPositionUpdate(RobotPosition robotPosition);
    void boxesPositionUpdate(std::vector<ColorBoxPosition> boxesPositionVector);

private slots:
    void mainWork();
};

#endif // MAINROZPOZNAWATOR_H
