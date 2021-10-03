#include <QtCore>
#include <QtGui>
#include <QStyleOptionGraphicsItem>

#include "GraphicsPointItem.h"
#include "GraphicsLineItem.h"
#include "ShapeLayer.h"

ShapeLayer::ShapeLayer(QGraphicsItem * parent, const QString& name, const QString& id)
	: MapLayer(parent, name, id)
{
	setFlags(ItemIsSelectable);
}

ShapeLayer::~ShapeLayer()
{

}

int ShapeLayer::addPoint(const QPointF & point, int id)
{
	//if (id == -1) id = _idman++;
	//_points[id] = point;

	//_bRect = _bRect.united(QRectF(point, QSizeF(1, 1)));
	//return id;

	//return addPath(path, id);

	GraphicsPointItem* item = new GraphicsPointItem(this);
	item->setData(point);
	//QRectF rect(point - QPointF(-1, -1), QSizeF(3, 3));
	//QGraphicsEllipseItem* item = new QGraphicsEllipseItem(rect, this);
	//item->setPen(_pen);
	//item->setBrush(_brush);
	int nid = addToGroup(item, id);
	return nid;
}

int ShapeLayer::addLine(const QLineF & line, int id)
{
	//QPainterPath path;
	//path.moveTo(line.p1());
	//path.lineTo(line.p2());

	//if (id == -1) id = _idman++;
	//_paths[id] = path;
	//_bRect = _bRect.united(path.boundingRect());
	//return id;

	////return addPath(path, id);

	GraphicsLineItem* item = new GraphicsLineItem(this);
	item->setData(line);
	item->setPen(_pen);
	int nid = addToGroup(item, id);
	return nid;
}

int ShapeLayer::addPath(const QPainterPath & path, int id)
{
	//if (id == -1) id = _idman++;
	//_paths[id] = path;
	//_bRect = _bRect.united(path.boundingRect());
	//return id;

	QGraphicsPathItem* item = new QGraphicsPathItem(path, this);
	item->setPen(_pen);
	item->setBrush(_brush);
	int nid = addToGroup(item, id);
	return nid;
}

int ShapeLayer::addRect(const QRectF & rect, int id)
{
	QGraphicsRectItem* item = new QGraphicsRectItem(rect, this);
	int nid = addToGroup(item, id);
	return nid;
}

int ShapeLayer::addPolygon(const QPolygonF & poly, int id)
{
	QGraphicsPolygonItem* item = new QGraphicsPolygonItem(poly, this);
	int nid = addToGroup(item, id);
	return nid;
}

int ShapeLayer::addEllipse(const QRectF & rect, int id)
{
	QGraphicsEllipseItem* item = new QGraphicsEllipseItem(rect, this);
	int nid = addToGroup(item, id);
	return nid;
}

QRectF	ShapeLayer::boundingRect() const
{
	return MapLayer::boundingRect();
	//return _bRect;
}

void ShapeLayer::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	//painter->save();
	//QTransform t = painter->worldTransform();
	//qreal lod = option->levelOfDetailFromTransform(t);
	////t.scale(1.0 / lod, 1.0 / lod);	// adjust scale for adjust line width
	////painter->setTransform(t);

	//QPen pen = QPen(Qt::red);//_pen;
	//pen.setWidthF(1.0f / lod);
	//painter->setPen(pen);
	//painter->setBrush(QBrush(Qt::yellow));

	//for (const QPointF& pnt : _points.values())
	//{
	//	painter->drawEllipse(pnt, 3/lod, 3/lod);
	//}

	//for (const QPainterPath& path : _paths.values())
	//{
	//	painter->drawPath(path);
	//}

	//for (QGraphicsItem* item : childItems())
	//{
	//	QAbstractGraphicsShapeItem* i = dynamic_cast<QAbstractGraphicsShapeItem*>(item);
	//	if (i)
	//	{
	//		i->setPen(pen);
	//		i->setBrush(QBrush(Qt::yellow));
	//		i->setTransform(t);
	//		i->paint(painter, option, widget);
	//	}
	//	
	//	
	//	//item->paint(painter, option, widget);
	//}

	MapLayer::paint(painter, option, widget);
	//painter->restore();
}
