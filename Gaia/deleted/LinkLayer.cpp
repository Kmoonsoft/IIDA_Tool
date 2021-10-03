#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "LinkLayer.h"

LinkLayer::~LinkLayer(void)
{
	clear();
}

QRectF LinkLayer::boundingRect() const
{
	return QRectF(-180, -90, 360, 180);
}

void LinkLayer::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
	QTransform t = painter->worldTransform();
	t.scale(1.0/lod, 1.0/lod);	// adjust scale for adjust line width
	painter->setTransform(t);

	foreach(LinkItem* link, links)
	{
		if(!link->visible || 
			link->tx == 0 || !link->tx->visibility() || 
			link->rx == 0 || !link->rx->visibility() ) continue;

		painter->setPen(link->pen);
		painter->drawLine(link->tx->currentPosition()*lod, link->rx->currentPosition()*lod);
	}
}

LinkItem* LinkLayer::addLink( StationLayer* tx, StationLayer* rx, const QString& id, int type )
{
	if( links.contains (id) ) 
	{
		return links[id];
	}

	LinkItem* link = new LinkItem();
	link->tx = tx;
	link->rx = rx;
	link->type = type;
	links[id] = link;

	return link;
}

void LinkLayer::removeLink( const QString& id )
{
	delete links.take(id);
}

void LinkLayer::clear()
{
	qDeleteAll(links);
	links.clear();
}

LinkItem* LinkLayer::getLink( const QString& id )
{
	return links.value(id, 0);
}

void LinkLayer::setPen( const QPen& pen, int type )
{
	foreach(LinkItem* link, links)
	{
		if( link->type == type ) link->pen = pen;
	}
}

//
//void LinkLayer::setVisibleByStationID( const QString& id, bool visible )
//{
//	QMap<QString, LinkItem*>::iterator i;
//	for (i = links.begin(); i != links.end(); ++i)
//	 {
//		 LinkItem* item = i.value();
//		
//		 if( item->tx->id() == id || item->rx->id() == id ) item->visible = (visible && item->tx->visibility() && item->rx->visibility());
//	 }
//}
