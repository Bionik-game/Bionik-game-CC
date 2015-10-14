#ifndef MAINKOMUNIKACJA_H
#define MAINKOMUNIKACJA_H

#include <QObject>
#include "Common-utils/mapelements.h"
#include "stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

class MainKomunikacja : public QObject
{
	Q_OBJECT
private:
	QString ipAddress;
	HttpClient httpclient_1;
	HttpClient httpclient_2;
	Qstring ret_code;
public:
	explicit MainKomunikacja(QString ipAddress,QString ipAddress2);

	public slots:
	void robotCommandUpdate(RobotCommands robotCommands);
};

#endif // MAINKOMUNIKACJA_H
