#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "Shape.h"
#include "ShapeFileLayer.h"

void swap2(int length, void* pWord)   
{
	int		i;
    unsigned char	temp;

    for( i=0; i < length/2; i++ )
    {
		temp = ((unsigned char *) pWord)[i];
		((unsigned char *) pWord)[i] = ((unsigned char *) pWord)[length-i-1];
		((unsigned char *) pWord)[length-i-1] = temp;
    }
}

ShapeFileLayer::ShapeFileLayer( QGraphicsItem * parent, const QString& name, const QString& id)
	: MapLayer(parent, name, id)
{
	_pen = QPen(Qt::darkGray);
	_visibleArea = 20.0;
}

//ShapeLayer::~ShapeLayer()
//{
//
//}

QRectF	ShapeFileLayer::boundingRect() const
{
	return _bound;
}

bool ShapeFileLayer::open(const QString& shapeFile, int ignoreCount)
{
	QFileInfo fi(shapeFile);
	QString shxFileName = fi.absoluteFilePath();
	shxFileName.replace(fi.suffix(), "shx");

	QFile shpFile(shapeFile);
	QFile shxFile(shxFileName);

	if( !shpFile.open(QIODevice::ReadOnly) )
	{
		qDebug() << QString("File open failed : %1").arg(shapeFile); 
		qDebug()  << shpFile.errorString();
		return false;
	}

	if( !shxFile.open(QIODevice::ReadOnly) )
	{
		qDebug() << QString("File open failed :%1").arg(shxFileName); 
		qDebug()  << shxFile.errorString();
		shpFile.close();
		return false;
	}

	int tmp;
	shxFile.seek(24);
	shxFile.read((char*)&tmp, 4);
	swap2(4, &tmp);	// change byte order
	int fileSize = tmp*2;
	int recordCnt = (fileSize-100)/8;	// number of records

	int type=0;
	shpFile.seek(32);
	shpFile.read((char*)&type, 4);

	if(!(type == SHPT_POINT || type == SHPT_POLYLINE || type == SHPT_POLYGON || type == SHPT_MULTIPOINT)) 
	{
		qDebug("Not Supported shape type!\nOnly support Point, Arc, Polygon & MultiPoint.");
		shpFile.close();
		shxFile.close();
		return false;
	}

	int _type = type;
	Envelop env;
	shpFile.read((char*)&env, sizeof(Envelop));// Get Envelop
	//_envelop = QRectF( env.xmin, env.ymin, env.xmax-env.xmin, env.ymax-env.ymin );

	//_points.clear();
	//_polygons.clear();
	//_envelop = QRectF();
	_bound = (QRectF)env;

	switch( _type )
	{
	//case SHPT_POINT:
	//	//_points.reserve(recordCnt);
	//	_points.resize(recordCnt);
	//	for(int i = 0; i < recordCnt; i++)
	//	{
	//		shxFile.seek(100 + i * 8);
	//		shxFile.read((char*)&tmp, 4);
	//		swap(4, &tmp);
	//		tmp = tmp * 2;

	//		shpFile.seek(tmp + 12);
	//		GeoPoint p;
	//		shpFile.read((char*)&p, 16);
	//		_points.append(p);
	//	}
	//	break;

	case SHPT_POLYLINE :
	case SHPT_POLYGON: // 폴리곤 처리
		//_polygons.reserve(recordCnt);
		//_polygons.resize(recordCnt);

		_areas.clear();
		_polygons.clear();
		_boundingRect.clear();

		_areas.reserve(recordCnt);
		_polygons.reserve(recordCnt);
		_boundingRect.reserve(recordCnt);
		for(int i = 0; i < recordCnt; i++ )
		{
			shxFile.seek(100+i*8);
			shxFile.read((char*)&tmp, 4);
			swap2(4, &tmp);
			tmp = tmp*2;

			shpFile.seek(tmp+12);
			Envelop env;
			int partCnt;
			int pointCnt;
			shpFile.read((char*)&env, 32);	//get Envelop
			shpFile.read((char*)&partCnt, 4);	//get parts number
			shpFile.read((char*)&pointCnt, 4);  //get points number

			if(pointCnt < ignoreCount ) continue;

			shpFile.seek(shpFile.pos() + 4*partCnt);

			GeoPoint pGeoPoint;
			QPolygonF polygon;
			for( int j = 0; j < pointCnt; j++ )
			{
				shpFile.read((char*)&pGeoPoint, 16);
				polygon << pGeoPoint;
			}

			_polygons << polygon;
			QRectF r = polygon.boundingRect();
			_areas.append(qMax(0.1, r.width() * r.height()));
			_boundingRect.append(r);
		}
	//case SHPT_MULTIPOINT:
	//	_points.resize(recordCnt);
	//	for(int i = 0; i < recordCnt; i++)
	//	{
	//		shxFile.seek(100+i*8);
	//		shxFile.read((char*)&tmp, 4);
	//		swap(4, &tmp);
	//		tmp = tmp*2;

	//		shpFile.seek(tmp+12);

	//		Polygon p;
	//		int pointCnt;
	//		shpFile.read((char*)&p.envelop, 32);	//get Envelop
	//		shpFile.read((char*)&pointCnt, 4);  //get points number
	//		p.points.resize(pointCnt);
	//		
	//		GeoPoint* pGeoPoint = p.points.data();
	//		for( int j = 0; j < pointCnt; j++ )
	//		{
	//			shpFile.read((char*)(pGeoPoint + j), 16);
	//		}
	//		_polygons.append(p);
	//	}
	//	break;
	}
	

	shpFile.close();
	shxFile.close();

	update();
	
	//qDebug() << "World shape count : " << scene()->items().count() << "개";
	//setLODRange(2, 10);
	return true;
}

void	ShapeFileLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	painter->save(); 
	
	const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
	_pen.setWidth(1);
	painter->setPen(_pen);

	const float* a = _areas.data();
	const QPolygonF* p = _polygons.data();
	const QRectF* r = _boundingRect.data();
	QRectF vr = visibleRect();

	for( int i = 0; i < _polygons.count(); i++ )
	{
		// 면적이 일정 크기 이상이고 가시 범위에 있을 경우 그린다.
		if( a[i]*lod > _visibleArea && vr.intersects(r[i]) ) painter->drawPolyline(p[i]);
	}

	painter->restore();
}
