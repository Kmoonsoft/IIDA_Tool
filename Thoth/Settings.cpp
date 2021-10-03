/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QApplication>
#include <QStringList>
#include <QSettings>
#include <QByteArray>
#include <QDir>

#include <time.h>

#include "Settings.h"

static QSettings* gIni = 0;

void Settings::initialize(QObject* parent, const QString& iniFile)
{
	gIni = new QSettings(iniFile, QSettings::IniFormat, parent);
}

QString Settings::getStyleSheet(const QString & fileName)
{
	QString styleSheet;

	QFile styleFile(qApp->applicationDirPath() + "/style/" + fileName);
	if (styleFile.open(QIODevice::Text | QIODevice::ReadOnly))
	{
		styleSheet = styleFile.readAll();
		styleFile.close();
	}

	return styleSheet;
}

void Settings::setFilePath(const QString & key, const QString & path)
{
	gIni->setValue(QString("Path/%1").arg(key), path);
}

QString Settings::getFilePath(const QString & key)
{
	return gIni->value(QString("Path/%1").arg(key)).toString();
}

//void Settings::setGISServerAddr(const QString& address)
//{
//	gIni.setValue("System/OpenStreetIP", address);
//}
//
//QString Settings::getGISServerAddr()
//{
//	return gIni.value("System/OpenStreetIP", "127.0.0.1").toString();
//}
//
//void Settings::setPort(quint16 port)
//{
//	gIni.setValue("System/Port", port);
//}
//
//quint16 Settings::getPort()
//{
//	return gIni.value("System/Port", 3001).toUInt();
//}
//
