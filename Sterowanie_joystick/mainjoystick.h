#ifndef MAINJOYSTICK_H
#define MAINJOYSTICK_H

#include <QObject>
#include <QTimer>
#include <Common-utils/mapelements.h>

class MainJoystick : public QObject
{
    Q_OBJECT
private:
    double tempVal;
    unsigned tempCounter;
    unsigned robotId;
    QString device;

    QTimer requestChecker;
    bool padRequest;

public:
    explicit MainJoystick(unsigned robotId, QString device);

signals:
    void gamePadRequest(bool flag);
    void robotCommandUpdate(RobotCommands robotCommands);

public slots:
    RobotCommands getCommands();
    void checkRequest();
};

#endif // MAINJOYSTICK_H
