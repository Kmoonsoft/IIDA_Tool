#pragma once
/*!
@file       CategoryTreeModel.h
@date       2021.10.01
@author     Lucky
@version    v0.1
@brief      CategoryTree Item Model Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QAbstractItemModel>
#include <CategoryObject.h>

/*!
@brief Category Tree Item Struct
*/
struct CategoryTreeItem : public Category
{
	QList<Category*> children;
	Category* parent = 0;

	CategoryTreeItem& operator=(const Category& o);
	CategoryTreeItem& operator=(const CategoryTreeItem& o);

};

/*!
@brief CategoryTreeModel class privides a Tree model for category(or class) of label data
*/
class CategoryTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	CategoryTreeModel(const QVariantList& headers = QVariantList(), QObject *parent = 0);
	~CategoryTreeModel();

	void setHeaderData(const QVariantList& headers);

	void addCategory(const CategoryObject* cat);

	QVariant		data(const QModelIndex& index, int role) const;
	bool			setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	Qt::ItemFlags	flags(const QModelIndex& index) const;

	int				rowCount(const QModelIndex& parent = QModelIndex()) const;
	int				columnCount(const QModelIndex& parent = QModelIndex()) const;

	QVariant		headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	QModelIndex		index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex		parent(const QModelIndex& index) const;

private:
	CategoryTreeItem* _root;
	QMap<int, CategoryTreeItem*> _parents;
	QList<CategoryTreeItem*> _items;
	QVariantList	_headers;
};
