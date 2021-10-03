#include "ImageObject.h"

ImageObject::ImageObject(QObject *parent)
	: QObject(parent)
{
}

ImageObject::~ImageObject()
{
}

QString ImageObject::name(int imageId) const
{
	return _nameMap.value(imageId);
}

int ImageObject::id(const QString & imageName) const
{
	return _idMap.value(imageName, -1);
}

bool ImageObject::exist(const QString & imageName) const
{
	return _idMap.contains(imageName);
}

bool ImageObject::contains(int imageId) const
{
	return _nameMap.contains(imageId);
}

void ImageObject::add(int id, const QString & name)
{
	_nameMap[id] = name;
	_idMap[name] = id;
}

void ImageObject::clear()
{
	_nameMap.clear();
	_idMap.clear();
}

int ImageObject::insert(const QString & imageName)
{
	if( _idMap.contains(imageName) ) return id(imageName);
	int newId = _nextId++;

	_nameMap[newId] = imageName;
	_idMap[imageName] = newId;
	return newId;
}

void ImageObject::setData(QMap<int, QString>& data)
{
	_nameMap = data;
	_idMap.clear();
	for (int imageId : _nameMap.keys())
	{
		_idMap[_nameMap[imageId]] = imageId;
	}
}


QDataStream & operator<<(QDataStream &out, const ImageObject &img)
{
	out << img._nameMap;
	out << img._idMap;
	out << img._nextId;
	return out;
}

QDataStream & operator>>(QDataStream &in, ImageObject &img)
{
	in >> img._nameMap;
	in >> img._idMap;
	in >> img._nextId;

	return in;
}