/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <omp.h>

#include <QPainter>
#include <QtDebug>
#include <qapplication.h>
#include <qtconcurrentrun.h>

#include "GeoTiffFile.h"
#include "ImagePainter.h"


ImagePainter::ImagePainter(QObject *parent)
	: QThread(parent)
{
}

ImagePainter::~ImagePainter()
{
}

bool ImagePainter::open(const QString & path)
{
	return _file.open(path);
}

QImage ImagePainter::image() const
{
	return _img;
}

QImage::Format ImagePainter::imageFormat()
{
	return QImage::Format::Format_RGBA8888;
	//return QImage::Format_RGB888;
}

QImage ImagePainter::thumbnail(int size)
{
	if ( !_file.isOpen()) return QImage();

	ImageData temp[3];

	//R, G, B 순으로 저장
	for (int c = 1; c <= 3; c++)
	{
		temp[c - 1] = _file.overview(c, size);
	}

	QImage img(temp[0].width, temp[0].height, imageFormat());

	quint8* data[3] = { (quint8*)temp[0].data,(quint8*)temp[1].data, (quint8*)temp[2].data };

	int pixcelByte = 3;

	for (int h = 0; h < size; h++)
	{
		size_t hi = h * size;
		for (int w = 0; w < size; w++)
		{
			int ii = hi + w;
			img.setPixelColor(w, h, QColor(data[0][ii], data[1][ii], data[2][ii]));
		}
	}

	for (int c = 0; c < 3; c++)
	{
		temp[c].free();
	}
	return img;
}

QRectF ImagePainter::utmRect() const
{
	QRectF r;
	if (_file.isOpen()) r = _file.getUtmRect();
	return r;
}

QPointF ImagePainter::lonlat(const QPointF & pos) const
{
	return _file.latLon(pos.x(), pos.y());
}

/*!
@brief Run thread
@details Draw Image with input channel data
*/
void ImagePainter::run()
{
	draw();
}

/*!
@brief Draw Color image
*/
void ImagePainter::draw()
{
	if ( !_file.isOpen() ) return;
	if (_file.bandType(1) != GDT_Byte) return;
	if (_file.bandCount() < 3)
	{
		drawMono();
		return;
	}

	ImageData temp[3];
	//R, G, B
	for (int c = 1; c <= 3; c++)
	{
		temp[c - 1] = _file.band(c);
	}
	quint8* data[3] = { (quint8*)temp[0].data,(quint8*)temp[1].data, (quint8*)temp[2].data };

	int height = temp[0].height;
	int width = temp[0].width;
	_img = QImage(width, height, imageFormat());

	uchar* bits = _img.bits();

	// Using OpenMP parallel processing
	omp_set_num_threads(20);
#pragma omp parallel for
	for (int h = 0; h < temp[0].height; h++)
	{
		size_t hi = h * width;
		for (int w = 0; w < width; w++)
		{
			int ii = hi + w;
			_img.setPixelColor(w, h, QColor(data[0][ii], data[1][ii], data[2][ii]));
			//*bits = data[0][ii];
			//bits++;
			//*bits = data[1][ii];
			//bits++;
			//*bits = data[2][ii];
			//bits++;
			//bits++;
		}
	}

	for (int c = 0; c < 3; c++)
	{
		temp[c].free();
	}

	emit imageReady();
}

/*!
@brief Draw Grayscale image
*/
void ImagePainter::drawMono()
{
	if (!_file.isOpen()) return;
	if (_file.bandType(1) != GDT_Byte) return;

	ImageData temp[3];
	//R, G, B
	for (int c = 1; c <= 3; c++)
	{
		temp[c - 1] = _file.band(c);
	}
	quint8* data = (quint8*)temp[0].data;

	int height = temp[0].height;
	int width = temp[0].width;
	_img = QImage(width, height, imageFormat());

	uchar* bits = _img.bits();

	omp_set_num_threads(20);
#pragma omp parallel for
	for (int h = 0; h < temp[0].height; h++)
	{
		size_t hi = h * width;
		for (int w = 0; w < width; w++)
		{
			int ii = hi + w;
			_img.setPixelColor(w, h, QColor(data[ii], data[ii], data[ii]));
		}
	}

	for (int c = 0; c < 3; c++)
	{
		temp[c].free();
	}

	emit imageReady();
}

