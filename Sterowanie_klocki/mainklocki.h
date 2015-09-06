#ifndef MAINKLOCKI_H
#define MAINKLOCKI_H

#include <QObject>
#include <set>

#include "Common-utils/mapelements.h"
#include "Common-utils/datacollector.h"

class MainKlocki : public QObject
{
    Q_OBJECT

private:
    typedef DataCollector<Robot, std::vector<ColorBox>> MyDataCollector;
    MyDataCollector dataCollector;

    double tempVal;
    unsigned robotId;
    std::set<ColorBox::Color> boxColorSet;

public:
    MainKlocki(unsigned robotId, const std::set<ColorBox::Color> &boxColorSet);

signals:
    void robotCommandUpdate(RobotCommands robotCommands);

private slots:
    void processData();

public slots:
    void gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec);
};

#endif // MAINKLOCKI_H
