#include "GeoTiffFile.h"
#include "ImagePainter.h"

class GeoTiffPainterData
{
public:
	GeoTiffFile tiff[3] = { 0, 0, 0 };
};

GeoTiffPainter::GeoTiffPainter(QObject *parent)
	: SatImgPainter(parent)
{
	_d = new GeoTiffPainterData();
}

GeoTiffPainter::~GeoTiffPainter()
{
	delete _d;
}

/*!
@brief Open Single Tiff file contains Multi-Band channels
*/
bool GeoTiffPainter::openTiff(const QString & multiBandTiffPath)
{
	bool ok = _d->tiff[0].open(multiBandTiffPath);
	if (!ok) return false;
	
	setData(&_d->tiff[0]);

	Histogram hist[3];
	hist[0] = _d->tiff[0].histogram(1);
	hist[1] = _d->tiff[0].histogram(2);
	hist[2] = _d->tiff[0].histogram(3);

	int min[3] = { hist[0].finePercent(0.001), hist[1].finePercent(0.001), hist[2].finePercent(0.001) };
	int max[3] = { hist[0].finePercent(0.999), hist[1].finePercent(0.999), hist[2].finePercent(0.999) };
	setDrawRange(min, max);
	setDrawBoundary(_d->tiff[0].imageRect());

	//QImage img = thumbnail(1000);
	//img.save("C:/temp/imgsave2.png", "png");

	return true;
}

/*!
@brief Open Multi-Tiff files. Each file contains each band data(B, G, R)
*/
bool GeoTiffPainter::openTiff(const QString & bluePath, const QString & greenPath, const QString & redPath)
{
	bool ok = _d->tiff[0].open(bluePath);
	if (!ok) return false;
	ok = _d->tiff[1].open(greenPath);
	if (!ok) return false;
	ok = _d->tiff[2].open(redPath);
	if (!ok) return false;

	setData(&_d->tiff[0], &_d->tiff[1], &_d->tiff[2]);
	return true;
}

const GeoTiffFile * GeoTiffPainter::tiff(int band) const
{
	if (band < 0 || band > 3) return 0;
	return &_d->tiff[band];
}

QPixmap GeoTiffPainter::pixmap() const
{
	const uchar* buffer = SatImgPainter::image();
	QImage image(buffer, _d->tiff[0].size().width(), _d->tiff[0].size().height(), SatImgPainter::imageFormat());

	QPixmap map = QPixmap::fromImage(image);
	
	return map;
}

QRectF GeoTiffPainter::boundingRect() const
{
	QRectF r;
	if ( _d->tiff[0].isOpen() ) r = _d->tiff[0].getUtmRect();

	return r;
}

