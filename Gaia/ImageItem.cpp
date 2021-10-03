/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QtDebug>

#include "ImagePainter.h"
#include "ImageItem.h"

class ImageItemData
{
public:
	ImagePainter artist;
	QRectF boundary;
};

ImageItem::ImageItem(QGraphicsItem* parent)
	:QObject(0), QGraphicsPixmapItem(parent)
{
	_d = new ImageItemData;

	connect(&_d->artist, &ImagePainter::imageReady, this, &ImageItem::draw);
}

ImageItem::~ImageItem()
{
	delete _d;
}

bool ImageItem::openImage(const QString& path)
{
	bool ok = _d->artist.open(path);
	if (ok)
	{
		_d->boundary = _d->artist.utmRect();
		_d->artist.start();
	}
	else
	{
		qDebug() << "Invalid Image : " + path;
	}

	return ok;
}

void ImageItem::moveToPos()
{
	setPos(_d->boundary.x(), _d->boundary.y());
}

QPointF ImageItem::lonlat(const QPointF & pos)
{
	QPointF ll = _d->artist.lonlat(pos);
	return ll;
}

//QRectF ImageItem::boundingRect() const
//{
////	return _d->boundary;
//	return _d->artist.tiff(0)->imageRect();
//}

void ImageItem::draw()
{
	QPixmap map = QPixmap::fromImage(_d->artist.image());
	QGraphicsPixmapItem::setPixmap(map);
	qDebug() << _d->artist.image().size();
	qDebug() << map.isNull() << map.size();;
	update();

	emit imageReady();
}

