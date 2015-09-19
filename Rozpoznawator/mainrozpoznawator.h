#ifndef MAINROZPOZNAWATOR_H
#define MAINROZPOZNAWATOR_H

#include <QVector>
#include <QTimer>

#include "Common-utils/mapelements.h"

class MainRozpoznawator : public QObject
{
    Q_OBJECT

private:
    QTimer rythm;
    unsigned long robotPositionX;

public:
    MainRozpoznawator();

private:
    std::vector<Robot> updateRobotPosition();
    std::vector<ColorBox> updateBoxesPositions();

signals:
    void gameState(std::vector<Robot> robotsinfo, std::vector<ColorBox> colorBoxesInfo);

private slots:
    void mainWork();
};

#endif // MAINROZPOZNAWATOR_H
