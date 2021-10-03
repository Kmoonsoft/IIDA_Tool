#pragma once

/*!
@file       Settings.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief		Settings Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QString>
#include <QObject>

#define ScenePath "ScenePath"
#define LastSavePath "SavePath"
#define ObjectClassPath "ObjectClassPath"

/*!
@brief Settings class
*/
class Settings : public QObject
{
	Q_OBJECT
	//Q_PROPERTY(QString WorkingPath READ  getWorkingPath WRITE setWorkingPath)

public:

	static void		initialize(QObject* parent, const QString& iniFile);
	
	/*!
	@brief Resent File Path
	*/
	static void		setFilePath(const QString& key, const QString& path);
	static QString	getFilePath(const QString& key);

	/*!
	@brief GUI Style Sheet
	*/
	static QString	getStyleSheet(const QString& fileName);

	// Server Address/Port
	//static void		setGISServerAddr(const QString& address);
	//static QString	getGISServerAddr();

	//static void		setPort(quint16 port);
	//static quint16	getPort();

};
