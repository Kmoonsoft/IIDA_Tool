#include <QPixmap>
#include <QDateTime>
#include <QMap>
#include <QDebug>

#include "SqlQuery.h"
#include "SqlQueryModel.h"

using namespace MOON;

class ModelData
{
public :
	void setData(const QModelIndex index, const QVariant& value, int role)
	{
		if( _d.contains(index) )
		{
			QMap<int, QVariant>& m = _d[index];
			m[role] = value;
		}
		else
		{
			QMap<int, QVariant> m;
			m[role] = value;
			_d[index] = m;
		}
	};

	QVariant data(const QModelIndex index, int role)
	{
		if( !_d.contains(index) ) return QVariant();
		QMap<int, QVariant>& m = _d[index];
		return m.value(role, QVariant());
	};

	void remove(const QModelIndex index, int role)
	{
		if( !_d.contains(index) ) return;
		QMap<int, QVariant>& m = _d[index];
		m.remove(role);
	};

	void clear(const QModelIndex index)
	{
		_d.remove(index);
	};

	void clear(int role = -1)
	{
		if( role == -1 ) _d.clear();

		foreach( const QModelIndex& index, _d.keys() )
		{
			remove(index, role);
		}
	};

private:
	QMap<QModelIndex, QMap<int, QVariant>> _d;
};

class SqlQueryModelData
{
public:
	ModelData	data;
	QMap<QModelIndex, QPixmap> icons;
	QMap<int, bool> checkable;	// column, checkable
	QList<int>	dateTimeColumn;
};

SqlQueryModel::SqlQueryModel(QObject *parent)
	: QSqlQueryModel(parent)
{
	_d = new SqlQueryModelData;
}

SqlQueryModel::~SqlQueryModel()
{
	delete _d;
}

QVariant SqlQueryModel::data( const QModelIndex& index, int role ) const
{
	if (role == Qt::EditRole || role == Qt::DisplayRole)
	{
		QVariant v = QSqlQueryModel::data(index, role);
		if (_d->dateTimeColumn.contains(index.column()))
		{
			QDateTime dt = v.toDateTime();
			//qDebug() << dt;
			return dt.isValid() ? dt.toString("yyyy/MM/dd hh:mm:ss") : v;
			//return index.data().toDateTime().toString("yyyy/MM/dd hh:mm:ss");
		}

		return v;
	}
	return _d->data.data(index, role);
}

QVariant SqlQueryModel::data(int row, int column, int role) const
{
	return index(row, column).data(role);
}

bool SqlQueryModel::setUserData( const QModelIndex & index, const QVariant & value, int role )
{
	if( role == Qt::EditRole || role == Qt::DisplayRole ) return false;
	_d->data.setData(index, value, role);
	emit dataChanged(index, index);
	return true;
}

void SqlQueryModel::setRowData( int row, const QVariant& value, int role )
{
	if( role == Qt::EditRole || role == Qt::DisplayRole ) return;

	int cnt = columnCount();
	for(int i = 0; i < cnt; i++ )
	{
		_d->data.setData(index(row, i), value, role);
	}

	emit dataChanged(index(row, 0), index(row, cnt));
}

void SqlQueryModel::clearUserData( int role )
{
	_d->data.clear(role);
	emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

void SqlQueryModel::refresh(const QModelIndex & index)
{
	emit dataChanged(index, index);
}

void SqlQueryModel::refreshRow(int row)
{
	emit dataChanged(index(row, 0), index(row, columnCount()));
}

void SqlQueryModel::sort(int column, Qt::SortOrder order)
{
	emit beforeSort(column, order);

	SqlQuery q(query());
	QString str = q.lastQuery().simplified();
	if( str.isEmpty() ) return;

	static const QRegExp exp("order\\sby([\\s|,]?[\\s]*[\\\"|\\']?[\\d\\w][\\\"|\\']?[\\s]*(asc|desc|))*", Qt::CaseInsensitive);;
	int insIndex = str.lastIndexOf(exp);
	QString orderby = QString("order by %1 %2").arg(column+1).arg(order == Qt::AscendingOrder ? QLatin1String("asc") : QLatin1String("desc") );

	if( insIndex < 0 ) str +=  " " + orderby;
	else str.replace(exp, orderby);
	
	QMap<QString, QVariant> boundValues = q.boundValues();
	if( boundValues.count() ) q.exec(str, boundValues.values());
	else q.exec(str);
	setQuery(q);

	emit sorted(column, order);
}

void SqlQueryModel::setCheckable(int column, bool checkable)
{
	_d->checkable[column] = checkable;

	if( checkable ) 
	{
		for( int i = 0, cnt=rowCount(); i < cnt; i++ )
		{
			setUserData(index(i, column), Qt::Unchecked, Qt::CheckStateRole);
		}
	}
}

Qt::ItemFlags SqlQueryModel::flags( const QModelIndex & index ) const
{
	if( _d->checkable.isEmpty() || !_d->checkable.contains(index.column()) ) return QSqlQueryModel::flags(index);

	Qt::ItemFlags f;
	_d->checkable[index.column()] ? f = (QSqlQueryModel::flags(index) | Qt::ItemIsUserCheckable)
		: f = (QSqlQueryModel::flags(index) & ~Qt::ItemIsUserCheckable );
	return f;
}

bool SqlQueryModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
	if( role != Qt::CheckStateRole || _d->checkable.isEmpty() || !_d->checkable.value(index.column(), false) ) return false;


	return setUserData(index, value, role);
}

void MOON::SqlQueryModel::setDateTimeColumn(int col)
{
	if (!_d->dateTimeColumn.contains(col)) _d->dateTimeColumn << col;
}

void MOON::SqlQueryModel::setCheck(int row, int col, bool check)
{
	if (_d->checkable.contains(col))
	{
		setData(index(row, col), check ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
	}
}

void MOON::SqlQueryModel::setCheckAll(int col, bool check)
{
	if (_d->checkable.contains(col))
	{
		for (int row = 0; row < rowCount(); row++)
		{
			setData(index(row, col), check ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
		}
	}
}

QList<int> MOON::SqlQueryModel::checkedRow(int col) const
{
	QList<int> list;
	for (int r = 0; r < rowCount(); r++)
	{
		if (data(r, col, Qt::CheckStateRole).toBool())
		{
			list << r;
		}
	}

	return list;
}
