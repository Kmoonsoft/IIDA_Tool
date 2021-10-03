
/*! 
    @file       SqlQuery.h
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @warning    Nobody can edit this file except the author
    @brief      The SqlQuery class is an extension class of QSqlQuery.
*/

#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QtDebug>

#include "SqlQuery.h"

using namespace MOON;

SqlQuery::~SqlQuery()
{

}

bool SqlQuery::exec(const QString& qryString)
{
	bool ok = QSqlQuery::exec(qryString);
	if( !ok ) 
	{
		qDebug() << lastError().text();
		qDebug() << qryString;
	}
	return ok;
}

bool SqlQuery::exec(const QStringList& qryStringList)
{
	foreach( const QString& qryString, qryStringList )
	{
		if( !exec(qryString) ) return false;
	}

	return true;
}

bool SqlQuery::exec(const QString& qryString, const QVariant& bindValue1, const QVariant& bindValue2, const QVariant& bindValue3, const QVariant& bindValue4)
{
	prepare(qryString);
	addBindValue(bindValue1);
	if( bindValue2.isValid() ) 
	{
		addBindValue(bindValue2);
		if( bindValue3.isValid() ) 
		{
			addBindValue(bindValue3);
		
			if( bindValue4.isValid() ) addBindValue(bindValue4);
		}
	}

	bool ok = QSqlQuery::exec();
	if( !ok ) 
	{
		qDebug() << lastError().text();
		qDebug() << qryString;
		qDebug() << "Values:" << boundValues().values();
	}
	return ok;
}

bool SqlQuery::exec(const QString& qryString, const QVariantList& bindValues)
{
	prepare(qryString);
	foreach( const QVariant& v, bindValues ) addBindValue(v);
	bool ok = QSqlQuery::exec();
	if( !ok ) 
	{
		qDebug() << lastError().text();
		qDebug() << qryString;
		qDebug() << "Values:" << bindValues;
	}
	return ok;
}

bool MOON::SqlQuery::execBatch(const QString & qryString, const QList<QVariantList>& batchBindValues)
{
	prepare(qryString);
	for (const QVariantList& v : batchBindValues)
	{
		addBindValue(v);
	}

	bool ok = QSqlQuery::execBatch();
	if (!ok)
	{
		qDebug() << lastError().text();
		qDebug() << qryString;
	}
	return ok;
}
