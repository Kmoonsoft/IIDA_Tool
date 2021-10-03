#include <QtDebug>
#include <QMap>
#include <QSqlError>

#include "Database.h"

bool MOON::Database::sqliteConnect(const QString & databaseName, const QString & connectionName)
{
	return connect("QSQLITE", databaseName, connectionName);
}

bool MOON::Database::connect(const QString & dbType, const QString & databaseName, const QString & connectionName, const QString & host, const QString & user, const QString & password, int port)
{
	QSqlDatabase db = QSqlDatabase::addDatabase(dbType, connectionName);
	if( !host.isEmpty() ) db.setHostName(host);
	if (!databaseName.isEmpty())db.setDatabaseName(databaseName);
	if (!user.isEmpty())db.setUserName(user);
	if (!password.isEmpty())db.setPassword(password);
	if (port > 0) db.setPort(port);
	if (!db.open())
	{
		qWarning() << QObject::tr("Fail to open the target database!") << databaseName;
		qWarning() << db.lastError();
		return false;
	}

	return true;
}
