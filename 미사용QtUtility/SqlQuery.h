/*! 
    @file       SqlQuery.h
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @brief      The SqlQuery class is an extension class of QSqlQuery.
*/

#ifndef SQLQUERYEX_H
#define SQLQUERYEX_H

#include <QSqlQuery>
#include <QVariantList>

#include "qtutility_global.h"

namespace MOON
{
	class QTUTILITY_EXPORT SqlQuery : public QSqlQuery
	{

	public:
		SqlQuery ( QSqlResult * result ) : QSqlQuery(result){};
		SqlQuery ( const QString & query = QString(), QSqlDatabase db = QSqlDatabase() ) : QSqlQuery(query,db){};
		SqlQuery ( QSqlDatabase db ) : QSqlQuery(db){};
		SqlQuery ( const QSqlQuery & other ) : QSqlQuery(other){};

		~SqlQuery();

		bool exec(const QString& qryString);
		bool exec(const QStringList& qryStringList);
		bool exec(const QString& qryString, const QVariant& bindValue1, const QVariant& bindValue2=QVariant(), const QVariant& bindValue3=QVariant(), const QVariant& bindValue4=QVariant());
		bool exec(const QString& qryString, const QVariantList& bindValues);
		bool execBatch(const QString& qryString, const QList<QVariantList>& batchBindValues);
	};
};

#endif // SQLQUERYEX_H
