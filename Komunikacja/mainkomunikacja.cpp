#include "mainkomunikacja.h"
#include <sstream>
using namespace jsonrpc;

MainKomunikacja::MainKomunikacja(QMap<unsigned, QString> ipAddresses)
{
    for(unsigned robotId : ipAddresses.keys())
    {
        Connection connection;
        connection.httpClient = new HttpClient(ipAddresses[robotId].toStdString());
        connection.stubClent = new StubClient(*connection.httpClient);
        this->ipAddresses.insert(robotId, connection);
    }
}

MainKomunikacja::~MainKomunikacja()
{
    for(unsigned robotId : ipAddresses.keys())
    {
        Connection& connection = ipAddresses[robotId];
        delete connection.stubClent;
        delete connection.httpClient;
    }
}

void MainKomunikacja::robotCommandUpdate(RobotCommands robotCommands)
{
    QMap<unsigned, Connection>::Iterator it = ipAddresses.find(robotCommands.robotId);
    if (it == ipAddresses.end())
    {
        std::ostringstream oss;
        oss << "(MainKomunikacja::robotCommandUpdate) Got robotId that was not specified in constructor. Id is: " << robotCommands.robotId << ".";
        throw std::runtime_error(oss.str().c_str());
    }

    StubClient& stubClient = *(it.value().stubClent);
    try
    {
        ret_code = stubClient.moveRobot(robotCommands.xCentimetersPerSecond,robotCommands.yCentimetersPerSecond,robotCommands.zRadiansPerSecond).c_str();
        stubClient.notifyServer();
    }
    catch (JsonRpcException e)
    {
        std::cerr << e.what() << std::endl;
    }
}
