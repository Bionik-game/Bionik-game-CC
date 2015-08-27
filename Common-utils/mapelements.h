#ifndef ROBOTPOSITION_H
#define ROBOTPOSITION_H


struct RobotPosition
{
    const unsigned long xCentimeters;
    const unsigned long yCentimeters;
    const float rotationRadians;
    RobotPosition(unsigned long xCentimeters, unsigned long yCentimeters, float rotationRadians);
};

struct ColorBoxPosition
{
    const enum Color {RED, GREEN, BLUE} color;
    const unsigned long xCentimeters;
    const unsigned long yCentimeters;
    ColorBoxPosition(unsigned long xCentimeters, unsigned long yCentimeters, Color color);
};

struct RobotCommands
{
    const double xCentimetersPerSecond;
    const double yCentimetersPerSecond;
    const double zRadiansPerSecond;
    RobotCommands(double xCentimetersPerSecond, double yCentimetersPerSecond, double zRadiansPerSecond);
};

#endif // ROBOTPOSITION_H
