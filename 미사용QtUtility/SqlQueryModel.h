/*! 
    @file       SqlQuery.h
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @brief      The SqlQueryModel class is an extension class of QSqlQueryModel.
*/

#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>
#include "qtutility_global.h"

class SqlQueryModelData;

namespace MOON
{
	class QTUTILITY_EXPORT SqlQueryModel : public QSqlQueryModel
	{
		Q_OBJECT

	public:
		SqlQueryModel(QObject *parent=0);
		~SqlQueryModel();

		using QSqlQueryModel::data;
		QVariant	data( const QModelIndex & item, int role = Qt::DisplayRole ) const;
		QVariant	data(int row, int column, int role = Qt::DisplayRole) const;
		bool		setUserData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
		void		setRowData( int row, const QVariant& value, int role = Qt::EditRole );
		void		clearUserData( int role=-1 );
		void		refresh(const QModelIndex & index);
		void		refreshRow(int row);
		void		setCheckable(int column, bool checkable=true);

		void		sort(int column, Qt::SortOrder order);

		Qt::ItemFlags flags ( const QModelIndex & index ) const;
		bool		setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
		void		setDateTimeColumn(int col);
		void		setCheck(int row, int col, bool check = true);
		void		setCheckAll(int col, bool check = true);

		QList<int>  checkedRow(int col) const;


	signals:
		void		beforeSort(int column, Qt::SortOrder order);
		void		sorted(int column, Qt::SortOrder order);

	private:
		SqlQueryModelData*	_d;
	};
};

#endif // SQLQUERYMODEL_H
