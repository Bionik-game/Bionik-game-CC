#ifndef MAINKOMUNIKACJA_H
#define MAINKOMUNIKACJA_H

#include <QObject>
#include <QMap>
#include "Common-utils/mapelements.h"
#include "Komunikacja/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

class MainKomunikacja : public QObject
{
	Q_OBJECT
private:
    struct Connection
    {
        jsonrpc::HttpClient* httpClient; //It is here just because I don't know whether StubClient is wise enough to make a copy of HttpClient
        StubClient* stubClent;
    };

    QMap<unsigned, Connection> ipAddresses;
    QString ret_code;
public:
    explicit MainKomunikacja(QMap<unsigned, QString> ipAddresses);
    ~MainKomunikacja();

	public slots:
	void robotCommandUpdate(RobotCommands robotCommands);
};

#endif // MAINKOMUNIKACJA_H
