#ifndef MAINKLOCKI_H
#define MAINKLOCKI_H

#include <QObject>
#include <set>
#include <iostream>

#include "Common-utils/mapelements.h"
#include "Common-utils/datacollector.h"

class MainKlocki : public QObject
{
    Q_OBJECT

private:
    struct vect2d
    {
        double x;
        double y;
    };

    const double maxLength;
    unsigned robotId;
    std::set<ColorBox::Color> boxColorSet;

public:
    MainKlocki(unsigned robotId, const std::set<ColorBox::Color> &boxColorSet);

signals:
    void robotCommandUpdate(RobotCommands robotCommands);

public slots:
    void getCommands(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec);
};

#endif // MAINKLOCKI_H
