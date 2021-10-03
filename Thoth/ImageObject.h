#pragma once

/*!
@file       LabelObject.h
@date       2021.09.30
@author     Lucky
@version    v0.1
@brief      LableObject Header file
@details	Internal Lable Object
*/

#include <QObject>
#include <QDataStream>
#include <QMap>

class ImageObject : public QObject
{
	Q_OBJECT

public:
	ImageObject(QObject *parent = 0);
	~ImageObject();

	QString name(int imageId) const;
	int id(const QString& imageName) const;
	bool exist(const QString& imageName) const;
	bool contains(int imageId) const;
	void clear();

public slots:

	// return new id or exist id, if contains
	int insert(const QString& imageName);

	// Reset all data
	void setData(QMap<int, QString>& data);

private:
	// add single image
	void add(int id, const QString& name);


private:
	QMap<int, QString> _nameMap;
	QMap<QString, int> _idMap;
	int _nextId = 1;

	friend class LabelObject;

	// For serialization
	friend QDataStream & operator<<(QDataStream &out, const ImageObject &img);
	friend QDataStream & operator>>(QDataStream &in, ImageObject &img);
};
