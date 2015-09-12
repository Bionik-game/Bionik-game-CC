#ifndef MAINPRIORYTETYZATOR_H
#define MAINPRIORYTETYZATOR_H

#include <QObject>
#include "Common-utils/mapelements.h"

class MainPriorytetyzator : public QObject
{
    Q_OBJECT
private:
    bool gamePad;

public:
    MainPriorytetyzator();

signals:
    void getCommandsColors(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec);
    void getCommandsJoystick();

public slots:
    void gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec);
    void gamePadRequest(bool flag);
};

#endif // MAINPRIORYTETYZATOR_H
