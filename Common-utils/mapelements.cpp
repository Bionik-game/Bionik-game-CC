#include "mapelements.h"

#include <iostream>
RobotPosition::RobotPosition(unsigned long xCentimeters, unsigned long yCentimeters, float rotationRadians)
    : xCentimeters(xCentimeters), yCentimeters(yCentimeters), rotationRadians(rotationRadians)
{
}


ColorBoxPosition::ColorBoxPosition(unsigned long xCentimeters, unsigned long yCentimeters, ColorBoxPosition::Color color)
    : xCentimeters(xCentimeters), yCentimeters(yCentimeters), color(color)
{
}


RobotCommands::RobotCommands(double xCentimetersPerSecond, double yCentimetersPerSecond, double zRadiansPerSecond)
    : xCentimetersPerSecond(xCentimetersPerSecond), yCentimetersPerSecond(yCentimetersPerSecond), zRadiansPerSecond(zRadiansPerSecond)
{
}
