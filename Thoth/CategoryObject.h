#pragma once

/*!
@file       CategoryObject.h
@date       2021.09.30
@author     Lucky
@version    v0.1
@brief      CategoryObject Header file
@details	Internal Object Category(Class) manager 
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QMap>
#include <QDataStream>

struct Category
{
	int id = -1;
	//int parentId;
	QString name = "UnKnown";
	QString desc;
	int parentId = -1;
	bool isHidden = false;;

	bool isNull() const
	{
		return (id > 0);
	};

	// For serialization
	friend QDataStream & operator<<(QDataStream &out, const Category &cat);
	friend QDataStream & operator>>(QDataStream &in, Category &cat);
};

#include <QObject>

class CategoryObject : public QObject
{
	Q_OBJECT

public:
	CategoryObject(QObject *parent = 0);
	~CategoryObject();

	void setId(int id, const QString& name = QString());
	int id() const;
	QString name() const;

	bool open(const QString& classJsonPath);
	bool save(const QString& classJsonPath = QString());

	Category category(int id) const;
	
	Category add(const QString& name, const QString& desc);


	const QList<Category> allCategories() const;

private:
	int _id;
	QString _name;
	QMap<int, Category> _catMap;
	int _nextChildId = 1;
	QString _filePath;
	bool _changed = false;
};
