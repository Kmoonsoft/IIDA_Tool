#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QtDebug>

#include "GeoTiffFile.h"
#include "GeoTiffPainter.h"
#include "SatImageItem.h"

class SatImageItemData
{
public:
	GeoTiffPainter artist;
	QRectF boundary;
};

SatImageItem::SatImageItem(QGraphicsItem* parent)
	:QObject(0), QGraphicsPixmapItem(parent)
{
	_d = new SatImageItemData;

	connect(&_d->artist, &GeoTiffPainter::imageReady, this, &SatImageItem::draw);

	//QTransform t;
	//t.rotate(180, Qt::XAxis);
	//setTransform(t);
}

SatImageItem::~SatImageItem()
{
	delete _d;
}

bool SatImageItem::openImage(const QString& multiBandTiffPath)
{
	bool ok = _d->artist.openTiff(multiBandTiffPath);
	if (ok)
	{
		//_d->boundary = _d->artist.tiff(0)->geoBoundary().boundingRect();
		_d->boundary = _d->artist.tiff(0)->getUtmRect();
		QPixmap map = QPixmap::fromImage(_d->artist.thumbnail(100));
		QGraphicsPixmapItem::setPixmap(map);

		//map.save("C:/Temp/draw.png", "png");
		//qDebug() << map.size() << map.isNull();
		//QGraphicsPixmapItem::setPixmap(map);

		_d->artist.start();
	}
	else
	{
		qDebug() << "Invalid Image : " + multiBandTiffPath;
	//	QPixmap map(200, 200);
	//	QPainter p(&map);
	//	p.setPen(Qt::red);

	//	QFont f;
	//	f.setPointSize(12);
	//	p.drawText(20, 20, tr("Invalid Image"));
	}

	return ok;
}

bool SatImageItem::openImage(const QString & bluePath, const QString & greenPath, const QString & redPath)
{
	return _d->artist.openTiff(bluePath, greenPath, redPath);
}

void SatImageItem::moveToPos()
{
	setPos(_d->boundary.x(), _d->boundary.y());
}

QPointF SatImageItem::lonlat(const QPointF & pos)
{
	QPointF ll = _d->artist.tiff(0)->latLon(pos.x(), pos.y());
	return ll;
}

//QRectF SatImageItem::boundingRect() const
//{
////	return _d->boundary;
//	return _d->artist.tiff(0)->imageRect();
//}

void SatImageItem::draw()
{
	//QPixmap map = QPixmap::fromImage(_d->artist.thumbnail(1000));
	//QGraphicsPixmapItem::setPixmap(map);
	QGraphicsPixmapItem::setPixmap(_d->artist.pixmap());
	//setPos(_d->boundary.x(), _d->boundary.y());
	//setPos(1000, 1000);
	update();

	emit imageReady();
}

