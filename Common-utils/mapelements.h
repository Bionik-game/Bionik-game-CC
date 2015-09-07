#ifndef ROBOTPOSITION_H
#define ROBOTPOSITION_H


struct RobotPosition
{
    const long xCentimeters;
    const long yCentimeters;
    const float rotationRadians;
    RobotPosition( long xCentimeters, long yCentimeters, float rotationRadians);
};

struct ColorBoxPosition
{
    const enum Color {RED, GREEN, BLUE} color;
    const long xCentimeters;
    const long yCentimeters;
    ColorBoxPosition( long xCentimeters, long yCentimeters, Color color);
};

struct RobotCommands
{
    const double xCentimetersPerSecond;
    const double yCentimetersPerSecond;
    const double zRadiansPerSecond;
    RobotCommands(double xCentimetersPerSecond, double yCentimetersPerSecond, double zRadiansPerSecond);
};

#endif // ROBOTPOSITION_H
