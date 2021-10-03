#pragma once

/*!
@file       LabelObject.h
@date       2021.09.30
@author     Lucky
@version    v0.1
@brief      LableObject Header file
@details	Internal Lable Object
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QObject>

#include <QPointF>
#include <QLineF>
#include <QRectF>
#include <QPolygonF>

#include <QMap>
#include <QDataStream>
#include "ImageObject.h"
#include "CategoryObject.h"
//#include "Defines.h"

struct Annotation
{
	enum Type {NULL_TYPE=-1, UNKNOWN, POINT, LINE, RECT, POLYGON};

	int			type = NULL_TYPE;		// Type
	int			id = 0;					// Annotation ID
	QString		name = "Undefined";		//

	int			category = 0;			// Category ID
	QString		categoryName = "Unknown";
	int			imageID = 0;			// Image ID
	QString		imageName;				// Image Name(file)

	QPointF		point;	// Point(type = POINT)
	QLineF		line;	// Line (type = LINE)
	QRectF		rectangle;	// Rectangle (type = RECT)
	QPolygonF	polygon;	// Polygon (type = POLYGON)

	QRectF		boundingRect; //BBOX

	float		rotate = 0.0f;	// Rotation Angle(Degrees)

	bool isNull() const {
		return (type == NULL_TYPE);
	};

	QString sizeText() const;

	// Assign operator
	Annotation& operator=(const Annotation& o);

	// For serialization
	friend QDataStream & operator<<(QDataStream &out, const Annotation &ann);
	friend QDataStream & operator>>(QDataStream &in, Annotation &ann);
};

/*!
@brief LableObject class has all properties to manage lables
*/
class LabelObject : public QObject
{
	Q_OBJECT

public:
	LabelObject(QObject *parent = 0);
	~LabelObject();

	void setCategoryObject(CategoryObject* cat);

	// clear labels but file is still assigned
	void clear();

	// clear all
	void createNew();

	// oepn lable file
	bool open(const QString& labelPath);

	// save label file
	bool save(const QString& labelPath = QString::null);

	// get annotation
	Annotation annotation(int id) const;
	Category category(int categoryID) const;

	// Get annotation of the current image/scene
	QList<Annotation> annotations(const QString& imageName);

	/*!
	@brief update/append annotation
	*/
	void setAnnotation(const Annotation& anno);
	void removeAnnotation(int id);

	/*!
	@brief create new annotation with id(automatic)
	*/
	Annotation newAnnotation();

	bool isChanged() const;
	bool isNew() const;

	int insertImage(const QString& imageName);

	const ImageObject& imageObject() const;

signals:
	void changed(Annotation);
	void reset();

protected:
	QMap<int, Annotation> _labels;
	QString _labelPath;
	bool _changed = false;
	int _nextId  = 1;
	QMultiMap<int, int> _imageAnnoMap; //[imageID : annotation id]

	ImageObject _img;
	CategoryObject* _cat = 0;

private:
	/*!
	@brief create new id and append annotation
	@retrun new id
	*/
	int addAnnotation(const Annotation& anno);
private:
	double _fileVersion; // Loaded File Version

public:
	// For serialization
	friend QDataStream & operator<<(QDataStream &out, const LabelObject &obj);
	friend QDataStream & operator>>(QDataStream &in, LabelObject &obj);
};
