#include <math.h>
#include <QGraphicsScene>
#include <qgraphicsview.h>
#include <QPainter>
#include <QGraphicsLineItem>
#include <QStyleOptionGraphicsItem>
#include <QtDebug>

#include "GridLayer.h"

GridLayer::GridLayer(QGraphicsItem * parent, const QString& name, const QString& id)
	: MapLayer(parent, name, id)
{
	_pen.setColor(Qt::gray);
	_pen.setStyle(Qt::DotLine);
	setZValue(100);

	//QTransform t;
	//t.rotate(180, Qt::XAxis);

	//double d = 1.0;
	//for( double x = -180.0; x <= 180.0; x += d )
	//{
	//	QGraphicsLineItem* item = new QGraphicsLineItem(0.0, 0.0, 0.0, 180.0);
	//	item->setPos(x, -90.0);
	//	this->addToGroup(item);
	//	_xitems << item;
	//	item->setPen(_pen);
	//}

	//for( double y = -90; y <= 90.0; y += d )
	//{
	//	QGraphicsLineItem* item = new QGraphicsLineItem(0.0, 0.0, 360, 0.0);
	//	item->setPos(-180, y);
	//	this->addToGroup(item);
	//	_yitems << item;
	//	item->setPen(_pen);
	//}
}

GridLayer::~GridLayer()
{

}

QRectF	GridLayer::boundingRect() const
{
	//return visibleRect();
	return QRectF(-190.0, -100.0, 380.0, 200.0);
}

//QPainterPath GridLayer::shape() const
//{
//    QPainterPath path;
//    path.addRect(-180, -90, 360, 180);
//    return path;
//}

void	GridLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /* widget*/)
{
	const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());

	painter->setPen(_pen);

	double i = 0.5;
	if( lod <= 1 ) i = 90;
	else if( lod <= 2 ) i = 45;
	else if( lod <= 4 ) i = 30;
	else if( lod <= 8 ) i = 15;
	else if( lod <= 16 ) i = 10;
	else if( lod <= 32 ) i = 5;
	else if( lod <= 64 ) i = 2;
	else if( lod <= 128 ) i = 1;

	//if( lod <= 1 ) i = 180;
	//else if( lod <= 2 ) i = 90;
	//else if( lod <= 4 ) i = 60;
	//else if( lod <= 8 ) i = 30;
	//else if( lod <= 16 ) i = 20;
	//else if( lod <= 32 ) i = 10;
	//else if( lod <= 64 ) i = 4;
	//else if( lod <= 128 ) i = 2;
	//else if( lod <= 256 ) i = 1;

	QTransform t = painter->worldTransform();
	t.rotate(180, Qt::XAxis);
	t.scale(1.0/lod, 1.0/lod);
	painter->setTransform(t);	
	
	QRectF r = visibleRect();

	double left = r.x();
	double bottom = r.y();
	double right = left + r.width();
	double top = bottom + r.height();
	double x = left*lod;
	double y = (top+bottom-r.height())/(-2.0)*lod-4;

	//double i = qMax(0.1, 90.0/lod);
	//if( i >= 1 ) i = (int)i;
	//else 
	//{
	//	i = (((int)(i*10.0))%10)/10.0;
	//}

	double yMax = 90*lod;
	double xMax = 180*lod;
	for( double k = -180; k <= 180; k += i )
	{
		if( k > left && k < right )
		{
			QLineF line(k*lod, -yMax, k*lod, yMax);
			painter->drawLine(line);
			painter->drawText(k*lod, y, QString("%1").arg(k));
		}
	}

	for( double w = -90; w <= 90; w += i )
	{
		if( -w > bottom && -w < top )
		{
			QLineF line(-xMax, w*lod, xMax, w*lod);
			painter->drawLine(line);
			painter->drawText(x, w*lod, QString("%1").arg(-w));
		}
	}
}
