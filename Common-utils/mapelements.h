#ifndef ROBOTPOSITION_H
#define ROBOTPOSITION_H


struct Robot
{
    unsigned robotId;
    unsigned long xCentimeters;
    unsigned long yCentimeters;
    float rotationRadians;

    bool operator==(unsigned robotId)
    {
        return this->robotId == robotId;
    }
};

struct ColorBox
{
    enum Color {RED, GREEN, BLUE} color;
    unsigned long xCentimeters;
    unsigned long yCentimeters;
    double area;
};

struct RobotCommands
{
    unsigned robotId;
    double xCentimetersPerSecond;
    double yCentimetersPerSecond;
    double zRadiansPerSecond;
};

struct BoardIndicator
{
    double xCentimetersPerSecond;
    double yCentimetersPerSecond;
};

struct RobotGoal
{
    double xCentimetersPerSecond;
    double yCentimetersPerSecond;
    unsigned goalId;
};

/**
 * Aby móc wysyłać sygnałami klasy, które nie są dostarczone przez Qt,
 * należy je zadeklarować poniżej i zarejestrować w funkcji "registerMetaTypes".
 */
#include <QObject>
Q_DECLARE_METATYPE(ColorBox)
Q_DECLARE_METATYPE(Robot)
Q_DECLARE_METATYPE(RobotCommands)
Q_DECLARE_METATYPE(BoardIndicator)
Q_DECLARE_METATYPE(RobotGoal)
Q_DECLARE_METATYPE(std::vector<ColorBox>)
Q_DECLARE_METATYPE(std::vector<Robot>)
Q_DECLARE_METATYPE(std::vector<RobotCommands>)
Q_DECLARE_METATYPE(std::vector<BoardIndicator>)
Q_DECLARE_METATYPE(std::vector<RobotGoal>)

void registerMetaTypes();

#endif // ROBOTPOSITION_H
