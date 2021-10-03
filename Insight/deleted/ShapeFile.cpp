#include <QFileInfo>
#include <QtDebug>
#include <QRectF>
#include <QTime>
#include <qtconcurrentrun.h>

#include "ShapeFile.h"

ShapeFile::ShapeFile(QObject *parent)
	: QObject(parent)
{
}

ShapeFile::~ShapeFile()
{
	close();
}

bool ShapeFile::open(const QString & filePath)
{
	if (!QFile::exists(filePath))
	{
		qWarning() << "No exist the shape file : " + filePath;
		return false;
	}

	_shp = (GDALDataset*)GDALOpenEx(filePath.toLatin1(), GDAL_OF_VECTOR, NULL, NULL, NULL);

	if (_shp == 0)
	{
		qWarning() << "Fail to open the shape file : " + filePath;
		return false;
	}

	qDebug() << _shp->GetProjectionRef();

	//for (int i = 0; i < _shp->GetLayerCount(); i++)
	//{
	//	OGRLayer* layer = _shp->GetLayer(i);
	//	qDebug() << QString("%1, Features: %2, EPSG:").arg(layer->GetName()).arg(layer->GetFeatureCount()).arg(layer->GetSpatialRef()->GetEPSGGeogCS());
	//}

	//Layer Name : GSHHS_f_L1 
	//Feature Count : 179837
	
	//layer->GetLayerDefn()->OGRFeatureDefn::GetGeomFieldDefn(iGeomField)->GetSpatialRef())

	return true;

	//for (int i = 0; i < 10 ; i++)
	//{
	//	OGRFeature* features = layer->GetFeature(i);

	//	OGRGeometry *geometry = features->GetGeometryRef();
	//	OGREnvelope env;
	//	geometry->getEnvelope(&env);
	//	qDebug() << QString("%1, %2, %3, %4").arg(env.MinX).arg(env.MinY).arg(env.MaxX).arg(env.MaxY);

	//	//OGRCurve* curve = geometry->toCurve();
	//	//qDebug() << curve->getNumPoints();
	//	//for (const OGRPoint p : *curve)
	//	//{
	//	//	qDebug() << QString("%1, %2").arg(p.getX()).arg(p.getY());
	//	//}
	//	
	//	OGRPolygon rect;
	//	OGRLinearRing ring;
	//	ring.addPoint(124, 24);
	//	ring.addPoint(124, 38);
	//	ring.addPoint(128, 38);
	//	ring.addPoint(128, 24);
	//	ring.addPoint(124, 24);
	//	rect.addRing(ring.toCurve());
	//	
	//	OGRGeometry *boundary = geometry->Boundary();
	//	OGRGeometry * crop = boundary->Intersection(rect.Boundary());
	//	//OGRCurve* curve = crop->toCurve();
	//	//qDebug() << curve->getNumPoints();

	//	OGRCurve* curve2 = crop->Boundary()->toCurve();
	//	qDebug() << curve2->getNumPoints();

	//	//qDebug() << geometry->getGeometryType() << boundary->getGeometryType();
	//	////if (boundary->getGeometryType() == wkbPolygon)
	//	//{
	//	//	OGRCurve* curve = boundary->toCurve();
	//	//	for (const OGRPoint p : *curve)
	//	//	{
	//	//		qDebug() << QString("%1, %2").arg(p.getX()).arg(p.getY());
	//	//	}

	//	//	//OGRPolygon* polygon = boundary->toPolygon();
	//	//	//for (OGRLinearRing* p : *polygon)
	//	//	//{
	//	//	//	
	//	//	//}
	//	//}
	//}

	return true;
}

void ShapeFile::close()
{
	if (_shp)
	{
		GDALClose(_shp);
		_shp = 0;
	}
}

QList<QPolygonF> inPolygon(OGRPolygon *bound, const OGRPolygon* boundary)
{
	QList<QPolygonF> list;

	OGRGeometry* inters = bound->Intersection(boundary);
	if (inters == 0) return list;
	//qDebug() << inters->exportToJson();

	OGRMultiLineString* ms = inters->toMultiLineString();
	if (ms == 0) list;
	//qDebug() << ms->exportToJson();

	for (OGRLineString* c : *ms)
	{
		//qDebug() << c->exportToJson();

		OGRGeometry* temp = c;
		OGRwkbGeometryType type = c->getGeometryType();
		if (type == wkbPolygon) temp = c->Boundary();

		QPolygonF polygon;
		for (const OGRPoint p : *temp->toCurve())
		{
			QPointF pt(p.getX(), p.getY());
			polygon << pt;
		}

		//if (type == wkbLineString) polygon << polygon.first();

		list << polygon;
	}

	//polygonList.append(shapePoly);

	//QPolygonF poly = boundary.intersected(shapePoly);
	//return poly;

	return list;
}

QList<QPolygonF> ShapeFile::getShape(const QPolygonF & boundary, int layerNo) const
{
	QList<QPolygonF> polygonList;
	OGRLayer* layer = _shp->GetLayer(layerNo);

	QTime t;
	t.start();
	//qDebug() << "No of Land Features:" << layer->GetFeatureCount();
	QRectF boundingRect = boundary.boundingRect();
	layer->SetSpatialFilterRect(boundingRect.left(), boundingRect.bottom(), boundingRect.right(), boundingRect.top());
	//qDebug() << "No of Filtered Land Features:" << layer->GetFeatureCount();

	OGRLinearRing  boundRing;

	for (const QPointF& p : boundary)
	{
		boundRing.addPoint(p.x(), p.y());
	}

	OGRPolygon boundPoly;
	boundPoly.addRing(&boundRing);

	//Concurrent Run Results
	QList<QFuture<QList<QPolygonF>> > results;

	OGRFeature* feature;
	//for (int i = 0; i < layer->GetFeatureCount(); i++)
	while((feature = layer->GetNextFeature()) != NULL)
	{
		OGRGeometry *geometry = feature->GetGeometryRef();
		OGRPolygon *bound = geometry->toPolygon();
		//QPolygonF shapePoly;

		//OGRFeature* feature = layer->GetFeature(i);
		QFuture<QList<QPolygonF>> future = QtConcurrent::run(inPolygon, bound, &boundPoly);
		results << future;

		////OGRCurve* curve = bound->toCurve();
		//OGRGeometry* inters = bound->Intersection(&boundPoly);
		//if (inters == 0) continue;
		////qDebug() << inters->exportToJson();
		//
		//OGRMultiLineString* ms = inters->toMultiLineString();
		//if (ms == 0) continue;
		////qDebug() << ms->exportToJson();

		//for (OGRLineString* c : *ms)
		//{
		//	//qDebug() << c->exportToJson();

		//	OGRGeometry* temp = c;
		//	OGRwkbGeometryType type = c->getGeometryType();
		//	if(type == wkbPolygon) temp = c->Boundary();

		//	QPolygonF pp;
		//	for (const OGRPoint p : *temp->toCurve())
		//	{
		//		QPointF pt(p.getX(), p.getY());
		//		pp << pt;
		//		//	 if( boundary.contains(pt) ) shapePoly << pt;
		//	}
		//	
		//	if (type == wkbLineString) pp << pp.first();

		//	polygonList << pp;
		//}

		//OGRLineString* c = inters->Boundary()->toLineString();

		////OGRCurve* c = inters->Boundary()->toCurve();
		////OGRPolygon* inter = (OGRPolygon*)inters;
		////
		//if (c == 0) continue;


		////for (OGRCurve* c : *inter)
		////{
		//	for (const OGRPoint p : *c)
		//	{
		//		QPointF pt(p.getX(), p.getY());
		//		shapePoly << pt;
		//		//	 if( boundary.contains(pt) ) shapePoly << pt;
		//	}
		////}

		//polygonList << shapePoly;
		//continue;

		///OGREnvelope env;
		///geometry->getEnvelope(&env);
		///QRectF envRect(env.MinX, env.MinY, (env.MaxX - env.MinX), (env.MaxY - env.MinY));

		///if (!boundingRect.intersects(envRect)) continue;

		//OGRPolygon* polygons = geometry->toPolygon();
		//for (OGRCurve* c : *polygons)
		//{
		//	QPolygonF subPoly;
		//	for (const OGRPoint p : *c)
		//	{
		//		QPointF pt(p.getX(), p.getY());
		//		subPoly << pt;
		//		//	 if( boundary.contains(pt) ) shapePoly << pt;
		//	}

		//	QPolygonF poly = boundary.intersected(subPoly);
		//	if (!poly.isEmpty()) polygonList.append( poly);
		//}

		//OGRCurve* curve = bound->toCurve();
		////OGRCurve* curve = geometry->toCurve();
		//for (const OGRPoint p : *curve)
		//{
		//	shapePoly << QPointF(p.getX(), p.getY());
		//}

		///polygonList.append(shapePoly);

		// QPolygonF poly = boundary.intersected(shapePoly);

		//if (!poly.isEmpty()) polygonList.append( poly);
	}

	for (QFuture< QList<QPolygonF>>& future : results) future.waitForFinished();

	for (QFuture< QList<QPolygonF>>& future : results) polygonList << future.result();

	//qDebug() << tr("Found:%1, Elapsed: %2 msec").arg(polygonList.count()).arg(t.elapsed());

	return polygonList;
}
