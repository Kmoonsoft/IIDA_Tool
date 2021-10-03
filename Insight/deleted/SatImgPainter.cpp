#include <QPainter>
#include <QtDebug>
#include <QTime>
#include <qapplication.h>
#include <qtconcurrentrun.h>

#include "GeoTiffFile.h"
#include "SatImgPainter.h"


struct DrawParam
{
	int hStart;
	int hEnd;
	int width;
	int* min;
	float* scales;
	int pixcelByte;
	quint16** data;
	uchar* buffer;

	bool left2right = true;
	bool top2bottom = true;
	DrawParam(int hStart_, int hEnd_, int width_, int* min_, float* scales_, int pixcelByte_, quint16** data_, quint16* monoData_, uchar* buffer_, bool _left2right=true, bool _top2bottom=true)
	{
		hStart = hStart_;
		hEnd = hEnd_;
		width = width_;
		min = min_;
		scales = scales_;
		pixcelByte = pixcelByte_;
		data = data_;
		buffer = buffer_;
		left2right = _left2right;
		top2bottom = _top2bottom;
	}
};

/*!
@brief Private/Internal Data Class
*/
class SatImgDrawerData
{
public:
	//QPixmap map;
	GeoTiffFile* tiffs[3] = { 0, 0, 0 }; // Red, Green, Blue bands

	//uchar* thumbBuffer = 0;
	uchar* buffer = 0;
	quint16* data[3] = { 0, 0, 0 }; // B, G, R
	float* filteredData = 0;

	bool reloadMode = true;
	
	int min[3];
	int max[3];

	QRect imgRect;

	const static int pixcelByte = 4;

	bool stop = false;

	bool left2right = true;
	bool top2bottom = true;

	void clearData()
	{
		for (int i = 0; i < 3; i++)
		{
			if (data[i])
			{
				//type == K5L1A ? delete[] data[i] : CPLFree(data[i]);
				delete[] data[i];
				data[i] = 0;
			}
		}
	}
};

SatImgPainter::SatImgPainter(QObject *parent)
	: QThread(parent)
{
	_d = new SatImgDrawerData;
}

SatImgPainter::~SatImgPainter()
{
	if (_d->buffer) delete[] _d->buffer;
	_d->clearData();
	delete _d;
}

void SatImgPainter::setData(GeoTiffFile * multiBand)
{
	GeoTiffFile * tiff[3] = { multiBand, 0, 0 };
	setData(tiff);
}

void SatImgPainter::setData(GeoTiffFile * blue, GeoTiffFile * green, GeoTiffFile * red)
{
	GeoTiffFile * tiff[3] = { blue, green, red };
	setData(tiff);
}

void SatImgPainter::setData(GeoTiffFile * tiff[3])
{
	_d->tiffs[0] = tiff[0]; // blue
	_d->tiffs[1] = tiff[1]; // green
	_d->tiffs[2] = tiff[2]; // red

	_d->clearData();

	// Whole Image
	_d->imgRect = QRect(QPoint(0, 0), tiff[0]->size());

	QPolygonF bound = tiff[0]->geoBoundary();
	if (bound.count() > 3)
	{
		QPointF topLeft = bound[0];
		QPointF bottomRight = bound[2];

		_d->left2right = topLeft.x() < bottomRight.x();
		_d->top2bottom = topLeft.y() > bottomRight.y();
	}
}

void SatImgPainter::setDrawRange(int min[3], int max[3])
{
	// B,G,R -> R, G, B 순으로 저장
	for (int i = 0; i < 3; i++)
	{
		_d->min[i] = min[2-i];
		_d->max[i] = max[2-i];
	}

	_d->reloadMode = false;
}

void SatImgPainter::setDrawBoundary(const QRect & imgRect)
{
	_d->imgRect = imgRect;
	_d->reloadMode = true;
}

const uchar* SatImgPainter::image() const
{
	return _d->buffer;
}

void SatImgPainter::stop()
{
	_d->stop = true;
}

QImage::Format SatImgPainter::imageFormat()
{
	//return QImage::Format::Format_RGB888;
	return QImage::Format::Format_RGBA8888;
}

QImage SatImgPainter::thumbnail(int size)
{
	if (_d->tiffs[0] == 0) return QImage();

	ImageData temp[3];

	if( _d->tiffs[0]->bandCount() >= 3 )
	{
		//R, G, B 순으로 저장
		for (int c = 1; c <= 3; c++)
		{
			temp[3-c] = _d->tiffs[0]->overview(c, size);
		}
	}
	else
	{
		//R, G, B 순으로 저장
		for (int c = 0; c < 3; c++)
		{
			temp[c] = _d->tiffs[2-c]->overview(1, size);
		}
	}

	QImage img(temp[0].width, temp[0].height, imageFormat());

	float scales[3];
	scales[0] = (_d->max[0] - _d->min[0]) / 256.0f; // red
	scales[1] = (_d->max[1] - _d->min[1]) / 256.0f; // green
	scales[2] = (_d->max[2] - _d->min[2]) / 256.0f; // blue

	quint16* data[3] = { (quint16*)temp[0].data, (quint16*)temp[1].data, (quint16*)temp[2].data };
	for (int h = 0; h < size; h++)
	{
		if (_d->stop) break;

		size_t hi = h * size;
		for (int w = 0; w < size; w++)
		{
			int ii = hi + w;
			size_t idx = _d->pixcelByte * ii;
			quint16 v[3];
			for (int c = 0; c < 3; c++)
			{
				quint16 d = data[c][ii];
				if ( d < _d->min[c])
				{
					v[c] = 0;
				}
				else
				{
					v[c] = (d - _d->min[c]);
					v[c] /= scales[c];
					if (v[c] > 255) v[c] = 255;
				}
			}

			int alpha = v[0] | v[1] | v[2] ? 255 : 0;

			img.setPixelColor(w, h, QColor(v[0], v[1], v[2], alpha));
		}
	}

	for (int c = 0; c < 3; c++)
	{
		temp[c].free();
	}
	return img;
}

/*!
@brief Run thread
@details Draw Image with input channel data
*/
void SatImgPainter::run()
{
	//qDebug() << "Image Painter Start!";
	if (_d->imgRect.isEmpty())
	{
		qDebug() << "Image Painter End! Boundary is not selected!";
		return;
	}

	_d->stop = false;

	if (_d->reloadMode)
	{
		reload();
	}
	else
	{
		redraw();
	}

	//qDebug() << "Image Painter End!";
}

/*!
@brief Reload data from file and redraw
*/
void SatImgPainter::reload()
{
	_d->clearData();

	if (_d->tiffs[0] == 0) return;

	if( _d->tiffs[0]->bandCount() >= 3 )
	{
		//R, G, B
		for (int c = 1; c <= 3; c++)
		{
			_d->data[3-c] = _d->tiffs[0]->data(c, _d->imgRect);
		}
		drawColor2(_d->data[0], _d->data[1], _d->data[2], _d->imgRect.width(), _d->imgRect.height());
	}
	else
	{
		//B, G, R
		for (int i = 0; i < 3; i++)
		{
			_d->data[i] = _d->tiffs[i]->data(1, _d->imgRect);
		}

		drawColor(_d->data[2], _d->data[1], _d->data[0], _d->imgRect.width(), _d->imgRect.height());
	}

	emit imageReady();
}
/*!
@brief Redraw with changed parameter such as min/max
*/
void SatImgPainter::redraw()
{
	//B, G, R
	drawColor(_d->data[2], _d->data[1], _d->data[0], _d->imgRect.width(), _d->imgRect.height());
	emit imageReady();
}

void runColor(const DrawParam& p)
{
	for (int h = p.hStart; h < p.hEnd; h++)
	{
		size_t hi = h * p.width;
		for (int w = 0; w < p.width; w++)
		{
			int ii = hi + w;
			size_t idx = p.pixcelByte * ii;

			quint16 v[3];
			for (int c = 0; c < 3; c++)
			{
				quint16 d = p.data[c][ii];
				if (d < p.min[c])
				{
					v[c] = 0;
				}
				else
				{
					v[c] = (d - p.min[c]);
					v[c] /= p.scales[c];
					if (v[c] > 255) v[c] = 255;
				}

				p.buffer[idx + c] = v[c];
			}

			// RGB 값이 없으면 투명처리(alpha = 0)
			//char alpha = v[0] | v[1] | v[2] ? 255 : 0;
			//p.buffer[idx + 3] = alpha;
		}
	}
}

void SatImgPainter::drawColor(quint16 * red, quint16 * green, quint16 * blue, int width, int height)
{
	// The Boundary Rect to draw
	size_t size = _d->pixcelByte * width * height;
	if (_d->buffer) delete[] _d->buffer;
	_d->buffer = new uchar[size];
	memset(_d->buffer, 255, size);

	quint16* data[3] = { red, green, blue };

	int runCnt = width < 1000 ? 1 : 10;
	int hStep = height / runCnt;
	QList<QFuture<void>> futureList;

	float scales[3];
	scales[0] = (_d->max[2] - _d->min[2]) / 256.0f;
	scales[1] = (_d->max[1] - _d->min[1]) / 256.0f;
	scales[2] = (_d->max[0] - _d->min[0]) / 256.0f;

	float min[3] = { (float)_d->min[2], (float)_d->min[1], (float)_d->min[0] };

	QTime t;
	t.start();

	for (int n = 0; n < runCnt; n++)
	{
		int h = n * hStep;
		int he = h + hStep;
		if (n == runCnt - 1 && he < height) he = height;

		if (_d->stop) break;

		DrawParam p(h, he, width, _d->min, scales, _d->pixcelByte, data, 0, _d->buffer);

		futureList << QtConcurrent::run(runColor, p);
	}

	for (QFuture<void>& f : futureList) f.waitForFinished();

	qDebug() << "Draw Elapsed" << t.elapsed() << "msec";
}

#include <omp.h>
/*!
@brief draw color image(obsolete)
*/
void SatImgPainter::drawColor2(quint16 * red, quint16 * green, quint16 * blue, int width, int height)
{
	// The Boundary Rect to draw
	size_t size = _d->pixcelByte * width * height;
	if (_d->buffer) delete[] _d->buffer;
	_d->buffer = new uchar[size];
	memset(_d->buffer, 255, size);

	quint16* data[3] = { red, green, blue };
	QTime t;
	t.start();

	for (int c = 0; c < 3; c++)
	{
		if (data[c] == 0) continue;

		//int mmIndex = 2 - c;

		float scales = (_d->max[c] - _d->min[c]) / 256.0f;

		omp_set_num_threads(20);
#pragma omp parallel for
		for (int h = 0; h < height; h++)
		{
			if (_d->stop) break;

			size_t hi = h * width;

			for (int w = 0; w < width; w++)
			{
				int ii = hi + w;
				size_t idx = _d->pixcelByte * ii;
				quint16 v;
				if (data[c][ii] < _d->min[c])
				{
					v = 0;
				}
				else
				{
					v = (data[c][ii] - _d->min[c]);
					v /= scales;
					if (v > 255) v = 255;
				}

				_d->buffer[idx + c] = v;
			}
		}

	}
	
	qDebug() << "Draw Elapsed" << t.elapsed() << "msec";
}
