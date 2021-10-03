#pragma once
#include <QObject>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "qtutility_global.h"

namespace MOON
{
	class QTUTILITY_EXPORT DAO :public QObject
	{
		Q_OBJECT
	public:
		DAO() = default;
		DAO(const DAO& other);

		static void setDatabase(const QSqlDatabase& db);
		static QSqlDatabase database();

		void select(const QStringList& columns, const QVariantList& values);
		void select(const QString& whereStmt, const QVariantList& bindingValues);
		bool update(QString& errMessage);
		bool insert(QString& errMessage);

		QList<DAO*> selectAll(const QStringList& columns, const QVariantList& values);
		QList<DAO*> selectAll(const QString& whereStmt, const QVariantList& bindingValues);
		bool deleteAll(const QStringList& columns, const QVariantList& values);

		void fromSqlQuery(const QSqlQuery& query);

		/// 칼럼 이름을 반환한다. member는 각 상속 클래스의 Members Enum을 참조해서 호출해야 한다.
		QString columnName(int member) const;
		QVariantList values(const QStringList& exceptList = QStringList()) const;

		///Utility
		void print() const;

	protected:
		virtual QString tableName() const = 0;
		virtual DAO* clone() const = 0;

	private:
		bool	exist(QString& returnWhereStmt, QStringList& returnKeys, QVariantList& returnKeyValues) const;
		void	columns(QStringList& columnNames) const;
	};
}
