#ifndef MAINKOMUNIKACJA_H
#define MAINKOMUNIKACJA_H

#include <QObject>
#include "Common-utils/mapelements.h"

class MainKomunikacja : public QObject
{
    Q_OBJECT
private:
    QString ipAddress;
public:
    explicit MainKomunikacja(QString ipAddress);

public slots:
    void robotCommandUpdate(RobotCommands robotCommands);
};

#endif // MAINKOMUNIKACJA_H
