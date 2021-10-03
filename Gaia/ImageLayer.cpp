/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QtCore>
#include <QtGui>

#include "ImageItem.h"
#include "ImageLayer.h"

ImageLayer::ImageLayer( QGraphicsItem * parent)
	: GraphicsLayer(parent)
{
}

ImageLayer::~ImageLayer()
{

}

ImageItem * ImageLayer::addImage(const QString & path)
{
	if (_single) clear();

	ImageItem* item = new ImageItem(this);
	item->openImage(path);
	addToGroup(item);
	if (!_single) item->moveToPos();
	return item;
}

QPointF ImageLayer::lonlat(const QPointF & pos)
{
	QList<QGraphicsItem*> children = childItems();
	for (QGraphicsItem* item : children)
	{
		ImageItem* image = (ImageItem*)item;
		if (image->boundingRect().contains(pos))
		{
			return image->lonlat(pos);
		}
	}

	return QPointF();
}

void ImageLayer::setSingleImage(bool single)
{
	_single = single;
}
