/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "GraphicsPointItem.h"

GraphicsPointItem::GraphicsPointItem(QGraphicsItem *parent)
	: GraphicsDrawItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

GraphicsPointItem::~GraphicsPointItem()
{

}

void GraphicsPointItem::setData( const QPointF& pos )
{
	_point = pos;
	_internalChanged = false;
	//_info._realPos = realPos;
	QRectF tRect;
	tRect.setLeft( pos.x() - 10 );
	tRect.setRight( pos.x() + 10 );
	tRect.setTop( pos.y() - 10 );
	tRect.setBottom( pos.y() + 10 );
	setRect( tRect );
}

QPointF GraphicsPointItem::data() const
{
	return _point;
}

GraphicsPointItem * GraphicsPointItem::clone() const
{
	GraphicsPointItem* item = new GraphicsPointItem(0);
	item->setData(_point);
	item->setPen(pen());
	return item;
}

void GraphicsPointItem::translate(double dx, double dy)
{
	_point += QPointF(dx, dy);
	setData(_point);
}

void GraphicsPointItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QTransform t = painter->worldTransform();
	const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform( t );

	painter->save();
	QPen pen = GraphicsDrawItem::pen();

	pen.setWidthF(pen.widthF() / lod);
	painter->setPen(pen);

	//painter->setFont( _info._drawInfo._font );
	painter->setBrush(brush());

	float radiusSize = 3/lod;
	QPointF radius(radiusSize, radiusSize);
	QRectF ellipse(_point - radius, _point + radius);
	painter->drawEllipse(ellipse);

	if( option->state & QStyle::State_Selected )
	{
		QPen newPen = pen;
		newPen.setColor(Qt::gray);
		newPen.setStyle(Qt::DashLine);
		painter->setPen( newPen );		
		painter->setBrush( Qt::transparent );

		float radiusSize = _selectPointRadius / lod;
		QPointF radius(radiusSize, radiusSize);

		QRectF ellipse(_point - radius, _point + radius);
		painter->drawRect( ellipse );		
		//painter->drawRect(rect().x() / lod, rect().y() / lod, rect().width() / lod, rect().height() / lod);
	}
	
	painter->restore();
}