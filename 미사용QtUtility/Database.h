#pragma once
#include <QSqlDatabase>
#include "qtutility_global.h"

namespace MOON
{
	//-----------------------------------------------------------------------------
	//	Database
	//-----------------------------------------------------------------------------
	class QTUTILITY_EXPORT Database
	{
	public:
		static bool sqliteConnect(const QString& databaseName, const QString& connectionName);
		static bool connect(const QString& dbType, const QString& databaseName, const QString& connectionName, const QString& host = QString::null, const QString& user = QString::null, const QString& password = QString::null, int port = -1);
	};
}