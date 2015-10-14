#ifndef MAINKOMUNIKACJA_H
#define MAINKOMUNIKACJA_H

#include <QObject>
#include "Common-utils/mapelements.h"
#include "Komunikacja/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

class MainKomunikacja : public QObject
{
	Q_OBJECT
private:
	QString ipAddress;
    jsonrpc::HttpClient httpclient_1;
    jsonrpc::HttpClient httpclient_2;
    QString ret_code;
public:
	explicit MainKomunikacja(QString ipAddress,QString ipAddress2);

	public slots:
	void robotCommandUpdate(RobotCommands robotCommands);
};

#endif // MAINKOMUNIKACJA_H
