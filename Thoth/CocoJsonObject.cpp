#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtDebug>

#include "CocoJsonObject.h"

static const QString CocoJsonDateTimeFormat = "yyyy-MM-dd hh:mm:ss";
static const QString gJsonFormatHeader = "{\n";

static const QString gJsonFormatImage =
"    {\n"
"      \"license\": LICENSE,\n"
"      \"file_name\": FILE_NAME,\n"
"      \"coco_url\": COCO_URL,\n"
"      \"height\": HEIGHT,\n"
"      \"width\": WIDTH,\n"
"      \"date_created\": DATE_CREATED,\n"
"      \"id\": ID\n"
"    }\n";

static const QString gJsonFormatAnnotation =
"    {\n"
"      \"segmentation\": SEGMENTATION,\n"
"      \"area\": AREA,\n"
"      \"iscrowd\": ISCROWD,\n"
"      \"image_id\": IMAGE_ID,\n"
"      \"bbox\": BBOX,\n"
"      \"category_id\": CATEGORY_ID,\n"
"      \"id\": ID\n"
"    }\n";

static const QString gJsonFormatTail = "\n    ]\n}";

CocoJsonObject::CocoJsonObject()
{
}

QString CocoJsonObject::toString(const QPointF & point, int decimal)
{
	QString pointString = QString("%1, %2").arg(point.x(), 0, 'f', decimal).arg(point.y(), 0, 'f', decimal);
	return pointString;
}

QString CocoJsonObject::toString(const QRectF & rect, int decimal)
{
	QString rectString = QString("[%1, %2]").arg(toString(rect.topLeft())).arg(toString(rect.bottomRight()));
	return rectString;
}

QString CocoJsonObject::toString(const QPolygonF & poly, int decimal)
{
	if (poly.count() == 1)
	{
		return toString(poly.first(), decimal);
	}

	QString str = "[";
	for (const QPointF& pos : poly)
	{
		str += toString(pos, decimal) + ", ";
	}
	if (str.isEmpty() == false) str.chop(2);

	str += "]";
	return str;
}

void CocoJsonObject::setLables(const QMap<qint32, COCOImage>& images, const QMap<qint32, COCOAnnotation> annotations)
{
	_images = images;
	_annotations = annotations;
}

/*!
@breif Conver to Json text
*/
QString CocoJsonObject::toString() const
{
	QString json = gJsonFormatImage;

	for (const COCOImage coco : _images.values())
	{
		QString str = gJsonFormatImage;

		str.replace("LICENSE", QString::number(coco.licence));
		str.replace("FILE_NAME", coco.file_name);
		str.replace("COCO_URL", coco.coco_url);
		str.replace("HEIGHT", QString::number(coco.height));
		str.replace("WIDTH", QString::number(coco.width));
		str.replace("DATE_CREATED", coco.date_created.toString(CocoJsonDateTimeFormat));
		str.replace("ID", QString::number(coco.id));
		str += ",\n";

		json += str;
	}

	for (const COCOAnnotation coco : _annotations.values())
	{
		QString str = gJsonFormatAnnotation;

		str.replace("SEGMENTATION", toString(coco.segmentation, 4));
		str.replace("AREA", QString::number(coco.area, 'f', 6));
		str.replace("ISCROWD", QString::number(coco.iscrowd));
		str.replace("IMAGE_ID", QString::number(coco.image_id));
		str.replace("BBOX", toString(coco.bbox));
		str.replace("CATEGORY_ID", QString::number(coco.category_id));
		str.replace("ID", QString::number(coco.id));
		str += ",\n";

		json += str;
	}

	if (!json.isEmpty())
	{
		json.chop(2);
		json += "\n";
	}

	// Get Templete Json
	QString tempJson;
	QJsonDocument info(_json.value("info").toObject());
	QJsonDocument licence(_json.value("licence").toObject());
	QJsonDocument categories(_json.value("categories").toObject());

	json = info.toJson() + licence.toJson() + categories.toJson() + json + gJsonFormatTail;
	return json;
}

bool CocoJsonObject::save(const QString & filePath) const
{
	bool ok = false;
	QFile file(filePath);
	if (file.open( QIODevice::Text | QIODevice::Append))
	{
		QString json = toString();
		qint64 size = file.write(json.toLatin1());
		ok = (size > 0);
		file.close();
	}

	return ok;
}

QPolygonF cocoArrayToPolygon(const QJsonArray& arr)
{
	QPolygonF poly;
	int cnt = arr.count() / 2;
	for (int i = 0; i < cnt; i++)
	{
		double x = arr.at(i*2).toDouble();
		double y = arr.at(i*2+1).toDouble();

		QPointF p(x, y);
		poly << p;
	}

	return poly;
}

QRectF cocoArrayToRect(const QJsonArray& arr)
{
	if (arr.count() < 4) return QRectF();

	double left = arr[0].toDouble();
	double top = arr[1].toDouble();
	double width = arr[2].toDouble();
	double height = arr[3].toDouble();

	QRectF r(left, top, width, height);
	return r;
}

/*!
@brief open COCO json 
*/
bool CocoJsonObject::open(const QString & jsonFilePath)
{
	_images.clear();
	_annotations.clear();
	_imageAnnoMap.clear();
	_categoryAnnoMap.clear();
	_categoryMap.clear();
	_categoryNameMap.clear();
	_superCatMap.clear();

	QFile f(jsonFilePath);

	if (!f.open(QFile::ReadOnly | QFile::Text))
	{
		qWarning() << "Fail to open JSON file : " << jsonFilePath;
		return false;
	}

	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(f.readAll(), &err);
	f.close();

	if (json.isNull())
	{
		qWarning() << err.errorString();
		return false;
	}

	QJsonObject  obj = json.object();

	if (!obj.contains("annotations"))
	{
		qWarning() << "No COCO JSON file : " << jsonFilePath;
		return false;
	}

	QJsonArray images = obj.value("images").toArray();

	for (int i = 0; i < images.count(); i++)
	{
		COCOImage img;
		QJsonObject obj = images.at(i).toObject();
		img.licence = obj.value("licence").toInt();
		img.file_name = obj.value("file_name").toString();
		img.coco_url = obj.value("coco_url").toString();
		img.height = obj.value("height").toInt();
		img.width = obj.value("width").toInt();
		img.date_created = QDateTime::fromString(obj.value("date_created").toString(), "yyyy-MM-dd hh:mm:ss");
		img.id = obj.value("id").toInt();

		_images[img.id] = img;
		_imageNameMap[img.file_name] = img.id;
	}

	QJsonArray annotations = obj.value("annotations").toArray();

	for (int i = 0; i < annotations.count(); i++)
	{
		COCOAnnotation anno;
		QJsonObject obj = annotations.at(i).toObject(); 
		//QJsonValue segVal = obj.value("segmentation");
		anno.segmentation = cocoArrayToPolygon(obj.value("segmentation").toArray().first().toArray());
		anno.area = obj.value("area").toDouble();
		anno.iscrowd = obj.value("iscrowd").toBool();
		anno.image_id = obj.value("image_id").toInt();
		anno.bbox = cocoArrayToRect(obj.value("bbox").toArray());
		anno.category_id = obj.value("category_id").toInt();
		anno.id = obj.value("id").toInt();

		_annotations[anno.id] = anno;
		_categoryAnnoMap.insert(anno.category_id, anno.id); // Multi Map
		_imageAnnoMap.insert(anno.image_id, anno.id); // Multi Map
	}

	QJsonArray categories = obj.value("categories").toArray();

	for (int i = 0; i < categories.count(); i++)
	{
		COCOCategory cat;
		QJsonObject obj = categories.at(i).toObject();
		cat.supercategory = obj.value("supercategory").toString();
		cat.id = obj.value("id").toInt();
		cat.name = obj.value("name").toString();

		_categoryMap[cat.id] = cat;
		_categoryNameMap[cat.name] = cat.id;
		_superCatMap.insert(cat.supercategory, cat.id);
	}

	return true;
}

bool CocoJsonObject::loadTemplete(const QString & templeteJsonPath)
{
	QFile file(templeteJsonPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qWarning() << "Fail to open JSON Templete file : " << templeteJsonPath;
		return false;
	}
	QByteArray json = file.readAll();
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(json, &error);
	file.close();

	if (doc.isNull())
	{
		qWarning() << error.errorString();
		return false;
	}
	_json = doc.object();
	return true;
}

COCOImage CocoJsonObject::image(int imageID) const
{
	return _images.value(imageID, COCOImage());
}

COCOImage CocoJsonObject::image(const QString & fileName) const
{
	int id = _imageNameMap.value(fileName, -1);
	return id == -1 ? COCOImage() : image(_imageNameMap[fileName] );
}

QList<COCOAnnotation> CocoJsonObject::annotation(int imageID) const
{
	QList<COCOAnnotation> list;
	QList<int> ids = _imageAnnoMap.values(imageID);
	for (int id : ids)
	{
		list << _annotations[id];
	}

	return list;
}

QList<COCOAnnotation> CocoJsonObject::annotation(int imageID, int categoryID) const
{
	QList<COCOAnnotation> list;
	QList<int> ids = _imageAnnoMap.values(imageID);
	QList<int> ids2 = _categoryAnnoMap.values(categoryID);

	for (int id : ids)
	{
		if (ids2.contains(id))
		{
			list << _annotations[id];
		}
	}

	return list;
}

QList<COCOAnnotation> CocoJsonObject::annotation(const QString & fileName) const
{
	COCOImage img = image(fileName);
	if (!img.isNull()) return annotation(img.id);
	else return QList<COCOAnnotation>();
}

COCOCategory CocoJsonObject::category(int catID) const
{
	return _categoryMap.value(catID, COCOCategory());
}

//const QList<AISJson>& CocoJsonObject::loadedObjects() const
//{
//	return _jsons;
//}

//QJsonObject CocoJsonObject::toJson(const QPolygonF & poly)
//{
//	QJsonObject obj;
//	obj.insert("type", "Polygon");
//	obj.insert("coordinates", toString(poly));
//
//	return obj;
//}
//
//void CocoJsonObject::add(const QString& key, const QJsonValue & child)
//{
//	children.append(JsonItem(key, child));
//}
//
//void CocoJsonObject::addPolygon(const QPolygonF & poly)
//{
//	add("type", "Polygon");
//	add("coordinates", toString(poly));
//}
//
//void CocoJsonObject::addPolygon(const QList<QPolygonF>& list)
//{
//	add("type", "MultiPolygon");
//
//	QString coord;
//	for (const QPolygonF& poly : list)
//	{
//		coord += toString(poly) + ",\n";
//	}
//
//	if (!coord.isEmpty())
//	{
//		coord.chop(2);
//		coord = "[\n" + coord + "]\n";
//		add("coordinates", coord);
//	}
//}

//QString CocoJsonObject::toString(int indent, int level) const
//{
//	QString str = QString(indent*level, ' ') + "{\n";
//	level++;
//	QString indentStr = QString(indent*level, ' ');
//
//	if (children.count() == 1)
//	{
//		JsonItem item = children[0];
//		if (item.value.type() == QJsonValue::String)
//		{
//			str += indentStr + QString("\"%1\": \"%2\"").arg(item.value.toString());
//		}
//		else if (item.value.type() == QJsonValue::Object)
//		{
//			str += indentStr + QString("\"%1\": \n").arg(item.value.toString());
//		}
//	}
//	//str += indentStr + QString("\"%1\"").arg
//	//str += QString(indent*level, ' ') + value.toString();
//	//str += QString(indent*level, ' ') + "\n}";
//	return str;
//}
