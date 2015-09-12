#include "mainkomunikacja.h"

MainKomunikacja::MainKomunikacja(QString ipAddress)
    : ipAddress(ipAddress)
{
}


void MainKomunikacja::robotCommandUpdate(RobotCommands robotCommands)
{
    //Send commands to provided ip using Json-RPC client
}
