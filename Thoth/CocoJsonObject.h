#pragma once

/*!
@file       MapView.h
@date       2021.09.15
@author     Lucky
@version    v0.1
@brief      COCO Json Object Header file
@details
*/
#include <QMap>
#include <QJsonObject>
#include <QDateTime>
#include <QPolygonF>

/*!
@brief COCO Image tag infomation
*/
struct COCOImage
{
	int licence  = 0;
	QString file_name;
	QString coco_url;
	int height;
	int width;
	QDateTime date_created = QDateTime::currentDateTime();
	int id = -1;

	bool isNull() const
	{
		return id < 0;
	}
};

/*!
@brief COCO Annotation tag information
*/
struct COCOAnnotation
{
	QPolygonF segmentation;
	double area;
	bool iscrowd;
	int image_id;
	QRectF bbox;
	int category_id;
	int id = -1;
	
	bool isNull() const
	{
		return id < 0;
	}
};

struct COCOCategory
{
	QString supercategory;
	int id = -1;
	QString name;
};

/*!
@brief COCO Json Lable Object 
*/
class CocoJsonObject
{
public:
	CocoJsonObject();

	static QString toString(const QPointF& point, int decimal = 0);
	static QString toString(const QRectF& rect, int decimal = 0);
	static QString toString(const QPolygonF & poly, int decimal = 0);

	//void append(const AISLableGeometry& ais);
	void setLables(const QMap<qint32, COCOImage>& images, const QMap<qint32, COCOAnnotation> annotations);

	//static QJsonObject toJson(const QPolygonF & poly);

	//void add(const QString& key, const QJsonValue& child);
	//void addPolygon(const QPolygonF & poly);
	//void addPolygon(const QList<QPolygonF> & list);

	//QString toString(int indent = 4, int level = 0) const;

	QString toString() const;
	bool save(const QString& filePath) const;

	bool open(const QString& jsonFilePath);

	bool loadTemplete(const QString& templeteJsonPath);

	COCOImage image(int imageID) const;
	COCOImage image(const QString& fileName) const;
	QList<COCOAnnotation> annotation(int imageID) const;
	QList<COCOAnnotation> annotation(int imageID, int categoryID) const;
	QList<COCOAnnotation> annotation(const QString& fileName) const;
	COCOCategory category(int catID) const;
private:
	QMap<int, COCOImage>		_images;	// [id : COCOImage]
	QMap<int, COCOAnnotation>	_annotations;	// [id : COCOAnnotation]

	QMap<QString, int>			_imageNameMap;	// [image file name : image id]
	QMultiMap<int, int>			_imageAnnoMap;	// [image id : annotation id]
	QMultiMap<int, int>			_categoryAnnoMap;	//[ category id : annotation id ]
	QJsonObject					_json;

	QMap<int, COCOCategory>		_categoryMap;
	QMap<QString, int>			_categoryNameMap;
	QMultiMap<QString, int>		_superCatMap;

	//QList<AISJson> _jsons;
};
