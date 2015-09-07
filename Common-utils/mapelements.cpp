#include "mapelements.h"

#include <iostream>
RobotPosition::RobotPosition( long xCentimeters,  long yCentimeters, float rotationRadians)
    : xCentimeters(xCentimeters), yCentimeters(yCentimeters), rotationRadians(rotationRadians)
{
}


ColorBoxPosition::ColorBoxPosition( long xCentimeters,  long yCentimeters, ColorBoxPosition::Color color)
    : xCentimeters(xCentimeters), yCentimeters(yCentimeters), color(color)
{
}


RobotCommands::RobotCommands(double xCentimetersPerSecond, double yCentimetersPerSecond, double zRadiansPerSecond)
    : xCentimetersPerSecond(xCentimetersPerSecond), yCentimetersPerSecond(yCentimetersPerSecond), zRadiansPerSecond(zRadiansPerSecond)
{
}
