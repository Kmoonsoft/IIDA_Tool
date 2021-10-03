/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QFile>
#include "CategoryObject.h"
#include "LabelObject.h"

const static double gsLabelVersion = 1.0;

QString Annotation::sizeText() const
{
	QString size;
	switch (type)
	{
	case POINT:
		break;
	case LINE:
		size = QString::number(line.length());
		break;
	case RECT:
		size = QString("%1x%2").arg(rectangle.width()).arg(rectangle.height());
		break;
	case POLYGON:
		size = QString("%1x%2").arg(boundingRect.width()).arg(boundingRect.height());
		break;
	}

	return size;
}

// Assign Operator
Annotation& Annotation::operator=(const Annotation& obj)
{
	type = obj.type;
	id = obj.id;
	name = obj.name;

	category = obj.category;
	categoryName = obj.categoryName;
	imageID = obj.imageID;
	imageName = obj.imageName;

	point = obj.point;
	line = obj.line;
	rectangle = obj.rectangle;
	polygon = obj.polygon;

	boundingRect = obj.boundingRect;
	rotate = obj.rotate;

	return *this;
}

// Serialization
QDataStream & operator<<(QDataStream &out, const Annotation &obj)
{
	out << obj.type;
	out << obj.id;
	out << obj.name;

	out << obj.category;
	out << obj.categoryName;
	out << obj.imageID;
	out << obj.imageName;

	out << obj.point;
	out << obj.line;
	out << obj.rectangle;
	out << obj.polygon;

	out << obj.boundingRect;
	out << obj.rotate;

	return out;
}

// Deserialization
QDataStream & operator>>(QDataStream &in, Annotation &obj)
{
	in >> obj.type;
	in >> obj.id;
	in >> obj.name;

	in >> obj.category;
	in >> obj.categoryName;
	in >> obj.imageID;
	in >> obj.imageName;

	in >> obj.point;
	in >> obj.line;
	in >> obj.rectangle;
	in >> obj.polygon;

	in >> obj.boundingRect;
	in >> obj.rotate;

	return in;
}

/*!
@brief Label Object Class
*/
LabelObject::LabelObject(QObject *parent)
	: QObject(parent)
{
}

LabelObject::~LabelObject()
{
	if (_changed) save();
}

void LabelObject::setCategoryObject(CategoryObject * cat)
{
	_cat = cat;
}

void LabelObject::clear()
{
	_labels.clear();
	_nextId = 1;
	_imageAnnoMap.clear();
	_img.clear();
	_labelPath.clear();

	_changed = false;
	emit reset();
}

void LabelObject::createNew()
{
	clear();
	_labelPath.clear();
	emit reset();
	_changed = false;
}

bool LabelObject::open(const QString & labelPath)
{
	QFile file(labelPath);
	if (!file.open(QIODevice::ReadOnly)) return false;

	QDataStream in(&file);

	clear();

	in >> *this;
	_labelPath = labelPath;
	file.close();

	_changed = false;
	return true;
}

bool LabelObject::save(const QString & labelPath)
{
	QString path = labelPath.isEmpty() ? _labelPath : labelPath;

	if (path.isEmpty()) return false;

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) return false;

	QDataStream out(&file);

	out << *this;

	file.close();
	_labelPath = path;

	_changed = false;

	return true;
}

Annotation LabelObject::annotation(int id) const
{
	return _labels.value(id);
}

Category LabelObject::category(int categoryID) const
{
	if (_cat == 0) return Category();
	return _cat->category(categoryID);
}

QList<Annotation> LabelObject::annotations(const QString & imageName)
{
	QList<Annotation> list;
	
	int imageId = _img.id(imageName);
	QList<int> ids = _imageAnnoMap.values(imageId);
	for (int id : ids)
	{
		list << _labels[id];
	}

	return list;
}

void LabelObject::setAnnotation(const Annotation & anno)
{
	if (_labels.contains(anno.id))
	{
		Annotation& a = _labels[anno.id];
		a = anno;
		if (!_imageAnnoMap.contains(anno.imageID, anno.id))
		{
			_imageAnnoMap.insert(anno.imageID, anno.id);
		}
		emit changed(a);
	}
	else
	{
		_labels[anno.id] = anno;
		if (anno.id > _nextId) _nextId = anno.id + 1;

		_imageAnnoMap.insert(anno.imageID, anno.id);
	}

	_changed = true;
}

void LabelObject::removeAnnotation(int id)
{
	if (!_labels.contains(id)) return;

	const Annotation& anno = _labels[id];
	_imageAnnoMap.remove(anno.imageID, id);
	_labels.remove(id);

	_changed = true;
}

int LabelObject::addAnnotation(const Annotation & anno)
{
	int id = _nextId++;
	_labels[id] = anno;
	Annotation& a = _labels[id];
	a.id = id;
	_changed = true;
	_imageAnnoMap.insert(a.imageID, a.id);
	emit changed(a);

	return id;
}
Annotation LabelObject::newAnnotation()
{
	Annotation a;
	a.id = _nextId++;

	_labels[a.id] = a;
	return a;
}

bool LabelObject::isChanged() const
{
	return _changed;
}
bool LabelObject::isNew() const
{
	return _labelPath.isEmpty();
}

int LabelObject::insertImage(const QString & imageName)
{
	return _img.insert(imageName);
}

const ImageObject & LabelObject::imageObject() const
{
	return _img;
}

// Serialization
QDataStream & operator<<(QDataStream &out, const LabelObject &obj)
{
	out << gsLabelVersion; // Save Current Version
	out << obj._labels;
	out << obj._nextId;
	out << obj._img;
	out << obj._imageAnnoMap;

	// Category는 별도 관리함

	return out;
}

// Deserialization
QDataStream & operator>>(QDataStream &in, LabelObject &obj)
{
	in >> obj._fileVersion; // Get File Version
	in >> obj._labels;
	in >> obj._nextId;
	in >> obj._img;
	in >> obj._imageAnnoMap;

	return in;
}
