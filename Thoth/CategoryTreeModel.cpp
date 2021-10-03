/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include "CategoryTreeModel.h"

CategoryTreeItem& CategoryTreeItem::operator=(const Category& o)
{
	id = o.id;
	name = o.name;
	desc = o.desc;
	parentId = o.parentId;
	isHidden = o.isHidden;

	return *this;
}

CategoryTreeItem& CategoryTreeItem::operator=(const CategoryTreeItem& o)
{
	id = o.id;
	name = o.name;
	desc = o.desc;
	parentId = o.parentId;
	isHidden = o.isHidden;

	return *this;
}

CategoryTreeModel::CategoryTreeModel(const QVariantList& headers, QObject *parent)
	: QAbstractItemModel(parent)
{
	setHeaderData(headers);

	_root = new CategoryTreeItem;
	CategoryTreeItem* unclass = new CategoryTreeItem;
	unclass->name = tr("Unclassified");
	unclass->id = -1;
	_items << unclass;
	_parents[unclass->id] = unclass;

	_root->children << unclass;
}

/*!
@brief Category Tree Model
*/
CategoryTreeModel::~CategoryTreeModel()
{
	delete _root;
	qDeleteAll(_items);
}

void CategoryTreeModel::setHeaderData(const QVariantList & headers)
{
	_headers = headers;
}

void CategoryTreeModel::addCategory(const CategoryObject * cat)
{
	QList<Category> list = cat->allCategories();
	for (const Category& cat : list)
	{
		CategoryTreeItem* c = new CategoryTreeItem;
		*c = cat;
		_items << c;

		// Parent ÀÌ¸י
		if (_parents.contains(c->id))
		{
			CategoryTreeItem* parent = _parents.value(c->id);
			*parent = cat;
		}
		else // Child
		{
			// Add child
			if (_parents.contains(c->parentId))
			{
				CategoryTreeItem* parent = _parents[c->parentId];
				parent->children << c;
				c->parent = parent;
			}
			else // New Parent
			{
				CategoryTreeItem* parent = new CategoryTreeItem;
				parent->id = c->parentId;
				parent->children << c;
				c->parent = parent;
				_parents[parent->id] = parent;
			}
		}
	}
}

QVariant CategoryTreeModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.column() != 0) return QVariant();

	CategoryTreeItem *item = static_cast<CategoryTreeItem*>(index.internalPointer());

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		//qDebug() << item->name();
		return item->name;
	}
	//else if (role == Qt::CheckStateRole)
	//{
	//	//qDebug() << item->visibility();
	//	return (item->visibility() ? Qt::Checked : Qt::Unchecked);
	//}

	return QVariant();
}

bool CategoryTreeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (!index.isValid()) return false;

	CategoryTreeItem *childItem = static_cast<CategoryTreeItem*>(index.internalPointer());
	if (childItem == 0) return false;

	if (role == Qt::EditRole)
	{
		childItem->name = value.toString();
		//qDebug() << "set:" << value.toString();
	}
	//else if (role == Qt::CheckStateRole)
	//{
	//	childItem->setVisibility(value.toBool());
	//}

	emit dataChanged(index, index);
	return true;
}

Qt::ItemFlags CategoryTreeModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;// | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

int CategoryTreeModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		CategoryTreeItem* cat = static_cast<CategoryTreeItem*>(parent.internalPointer());
		if (cat) return cat->children.count();
		else return 0;
	}
	else
	{
		//return _parents.count();
		return _root->children.count();
	}
}

int CategoryTreeModel::columnCount(const QModelIndex & parent) const
{
	return _headers.count();
}

QVariant CategoryTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return _headers[section];
	}

	return QVariant();
}

QModelIndex CategoryTreeModel::index(int row, int column, const QModelIndex & parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	CategoryTreeItem *parentItem;

	if (!parent.isValid())
	{
		//return QModelIndex();
		parentItem = _root;
	}
	else
	{
		parentItem = static_cast<CategoryTreeItem*>(parent.internalPointer());
	}

	CategoryTreeItem* childItem = (CategoryTreeItem*)(parentItem->children[row]);
	if (childItem )
	{
		return createIndex(row, column, childItem);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex CategoryTreeModel::parent(const QModelIndex & index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}

	CategoryTreeItem *childItem = static_cast<CategoryTreeItem*>(index.internalPointer());
	CategoryTreeItem *parentItem = (CategoryTreeItem *)(childItem->parent);

	if (parentItem == 0)
	{
		return QModelIndex();
	}

	return createIndex(parentItem->children.indexOf(childItem), 0, parentItem);
}
