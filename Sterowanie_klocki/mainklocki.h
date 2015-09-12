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
    double tempVal;
    unsigned robotId;
    std::set<ColorBox::Color> boxColorSet;

public:
    MainKlocki(unsigned robotId, const std::set<ColorBox::Color> &boxColorSet);

signals:
    void robotCommandUpdate(RobotCommands robotCommands);

public slots:
    RobotCommands getCommands(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec);
};

#endif // MAINKLOCKI_H
