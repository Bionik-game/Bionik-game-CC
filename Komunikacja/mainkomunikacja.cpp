#include "mainkomunikacja.h"
using namespace jsonrpc;

MainKomunikacja::MainKomunikacja(QString ipAddress,QString ipAddress2)
    : ipAddress(ipAddress),httpclient_1(ipAddress),httpclient_2(ipAddress2)// zmienna ipAddress (składowa modułu mainKomunikacja ofc) jest tu generalnie zbędna
{
}


void MainKomunikacja::robotCommandUpdate(RobotCommands robotCommands)
{
    //Send commands to provided ip using Json-RPC client
switch(robotCommands.robotId){
case 1:
	StubClient c(httpclient_1);
	  try
	    {
	    	ret_code = c.moveRobot(robotCommands.xCentimetersPerSecond,robotCommands.yCentimetersPerSecond,robotCommands.zRadiansPerSecond);
	        c.notifyServer();
	    }
	  catch (JsonRpcException e)
	    {
	    	std::cerr << e.what() << endl;
	    }
	break;

case 2:
	StubClient c(httpclient_2);
	  try
	    {
	    	ret_code = c.moveRobot(robotCommands.xCentimetersPerSecond,robotCommands.yCentimetersPerSecond,robotCommands.zRadiansPerSecond);
	        c.notifyServer();
	    }
	  catch (JsonRpcException e)
	    {
	    	std::cerr << e.what() << endl;
	    }
	break;

default:
	std::cerr<<"Bad robotId !\n";
}


}
