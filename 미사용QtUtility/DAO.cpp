//Primitive Include
#include <math.h>
#include <float.h>

//Qt Include
#include <QtDebug>
#include <QMetaProperty>
#include <QSqlIndex>
#include <QSqlDatabase>
#include <QSqlError>

//Custom Include
#include "SqlQuery.h"

//Local Include
//#include "DatabaseManager.h"
#include "DAO.h"

using namespace MOON;

static QSqlDatabase _db;

DAO::DAO(const DAO& other)
{
	for (int i = 1; i < metaObject()->propertyCount(); i++)
	{
		QMetaProperty property = metaObject()->property(i);
		setProperty(property.name(), other.property(property.name()));
	}
}

void DAO::setDatabase(const QSqlDatabase & db)
{
	_db = db;
}

QSqlDatabase MOON::DAO::database()
{
	return _db;
}

void DAO::fromSqlQuery(const QSqlQuery & query)
{
	for (int i = 1; i < metaObject()->propertyCount(); i++)
	{
		QMetaProperty property = metaObject()->property(i);
		setProperty(property.name(), query.value(property.name()));
	}
}

void DAO::select(const QStringList& columns, const QVariantList& values ) 
{
	SqlQuery q(_db);

	QString stmt = "select * from " + tableName() + " where " + columns.join("=?");
	if (columns.count() == 1) stmt += "=?"; // if columns.count == 1 join() doen't work.

	if (q.exec(stmt, values) && q.next())
	{
		fromSqlQuery(q);
	}
}

void DAO::select(const QString & whereStmt, const QVariantList & bindingValues)
{
	SqlQuery q(_db);

	QString stmt = "select * from " + tableName() + " where " + whereStmt;
	if (q.exec(stmt, bindingValues))
	{
		if (q.next())
		{
			fromSqlQuery(q);
		}
	}
}

bool DAO::update(QString & errMessage)
{
	QString whereStmt;
	QStringList keys;
	QVariantList keyValues;
	if (!exist(whereStmt, keys, keyValues))
	{
		errMessage = tr("Update failed. No matching data found.");
		return false;
	}

	QString stmt = "update " + tableName() + " set ";

	QStringList fields;
	columns(fields);
	for (const QString& key : keys) fields.removeAll(key);
	
	stmt += fields.join("=?,");
	stmt += "=?";
	stmt += whereStmt;

	SqlQuery q(_db);
	if (q.exec(stmt, QVariantList() << values(keys) << keyValues)) return true;
	errMessage = q.lastError().text();
	return false;
}

bool DAO::insert(QString & errMessage)
{
	QString whereStmt;
	QStringList keys;
	QVariantList keyValues;
	if (exist(whereStmt, keys, keyValues))
	{
		errMessage = tr("Insert failed. Already have matching data.");
		return false;
	}

	QStringList fields;
	columns(fields);
	QStringList placeHolders;
	for (int i = 0; i < fields.count(); i++)
	{
		placeHolders << "?";
	}

	QString stmt = "insert into " + tableName() + "(";
	stmt += fields.join(",");
	stmt += ") values (";
	stmt += placeHolders.join(",");
	stmt += ")";

	SqlQuery q(_db);
	if (q.exec(stmt, QVariantList() << values())) return true;
	errMessage = q.lastError().text();
	return false;
}

bool DAO::deleteAll(const QStringList & columns, const QVariantList & values)
{
	SqlQuery q(_db);

	QString stmt = "delete from " + tableName() + " where " + columns.join("=?");
	if (columns.count() == 1) stmt += "=?"; // if columns.count == 1 join() doen't work.

	return q.exec(stmt, values);
}

QString DAO::columnName(int member) const
{
	if (metaObject()->propertyCount() < member) return QString();
	return metaObject()->property(member).name();
}

QVariantList DAO::values(const QStringList& exceptList) const
{
	QVariantList list;
	for (int i = 1; i < metaObject()->propertyCount(); i++)
	{
		if (exceptList.contains(metaObject()->property(i).name())) continue;
		list << property(metaObject()->property(i).name());
	}

	return list;
}

QList<DAO*> DAO::selectAll(const QStringList & columns, const QVariantList & values)
{
	QList<DAO*> list;

	SqlQuery q(_db);

	QString stmt = "select * from " + tableName() + " where " + columns.join("=?");
	if (columns.count() == 1) stmt += "=?"; // if columns.count == 1 join() doen't work.

	if (q.exec(stmt, values) )
	{
		while (q.next())
		{
			fromSqlQuery(q);
			list << this->clone();
		}
	}
	return list;
}

QList<DAO*> DAO::selectAll(const QString & whereStmt, const QVariantList & bindingValues)
{
	QList<DAO*> list;

	SqlQuery q(_db);

	QString stmt = "select * from " + tableName() + " where " +  whereStmt;

	if (q.exec(stmt, bindingValues))
	{
		while (q.next())
		{
			fromSqlQuery(q);
			list << this->clone();
		}
	}
	return list;
}

void DAO::print() const
{
	QStringList values;
	for (int i = 1; i < metaObject()->propertyCount(); i++)
	{
		values << property(metaObject()->property(i).name()).toString();
	}

	qDebug() << values;
}

bool DAO::exist(QString & returnWhereStmt, QStringList& returnKeys, QVariantList& returnKeyValues) const
{
	QSqlDatabase db = _db;
	QString table = tableName();
	QSqlRecord record = db.record(table);
	if (record.isEmpty())
	{
		return false;
	}

	SqlQuery q(db);

	///Key로 데이터가 있는지 여부 체크
	QSqlIndex index = db.primaryIndex(table);
	//QStringList keys;
	QString stmt;
	//QVariantList keyValues;
	for (int i = 0; i < index.count(); i++)
	{
		returnKeys << index.fieldName(i);
		returnKeyValues << property(returnKeys.at(i).toLatin1());
	}

	// Make select statement
	stmt = "select * from " + table;

	if (index.isEmpty() == false)
	{
		returnWhereStmt = " where " + returnKeys.join("=? and ");
		returnWhereStmt += "=?";
	}

	stmt += returnWhereStmt;
	 
	if (q.exec(stmt, returnKeyValues) && q.next() ) return true;
	return false;
}

void DAO::columns(QStringList& columnNames) const
{
	QSqlRecord record = _db.record(tableName());

	for (int i = 0; i < record.count(); i++)
	{
		columnNames << record.fieldName(i);
	}
}
