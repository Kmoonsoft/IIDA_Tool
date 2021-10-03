#include <QtDebug>
#include "SatImageItem.h"
#include "SatImageLayer.h"

SatImageLayer::SatImageLayer(QGraphicsItem *parent)
	: GraphicsLayer(parent)
{
	//QTransform t;
	//t.rotate(180, Qt::XAxis);
	//setTransform(t);
}

SatImageLayer::~SatImageLayer()
{
}

void SatImageLayer::addImage(SatImageItem * item)
{
	if (_single) clear();
	addToGroup(item);
	if (!_single) item->moveToPos();
}

QRectF SatImageLayer::boundingRect() const
{
	//qDebug() << MapLayer::boundingRect();
	//return MapLayer::boundingRect();
	return childrenBoundingRect();
}

SatImageItem * SatImageLayer::addImage(const QString & tiffPath)
{
	if (_single) clear();
	SatImageItem* item = new SatImageItem(this);
	item->openImage(tiffPath);
	addToGroup(item);
	if (!_single) item->moveToPos();
	return item;
}

SatImageItem * SatImageLayer::addImage(const QString & bluePath, const QString & greenPath, const QString & redPath)
{
	if (_single) clear();
	SatImageItem* item = new SatImageItem(this);
	
	item->openImage(bluePath, greenPath, redPath);
	addToGroup(item);

	if (!_single) item->moveToPos();
	return item;
}

void SatImageLayer::setSingleImage(bool single)
{
	_single = single;

	if (_single)
	{
		QList<QGraphicsItem*> children = childItems();
		for (int i = 1; i < children.count(); i++)
		{
			removeFromGroup(children[i]);
			delete children[i];
		}

		if (!children.isEmpty())
		{
			children[0]->setPos(0, 0);
		}
	}
}

QPointF SatImageLayer::lonlat(const QPointF & pos)
{
	QList<QGraphicsItem*> children = childItems();
	for (QGraphicsItem* item : children)
	{
		SatImageItem* image = (SatImageItem*)item;
		if (image->boundingRect().contains(pos))
		{
			return image->lonlat(pos);
		}
	}

	return QPointF();
}
