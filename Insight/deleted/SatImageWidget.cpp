#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <qtconcurrentrun.h>

#include "GeoTiffFile.h"
#include "SatImgPainter.h"

#include "SatImageWidget.h"

#define MIN_ZOOM	0.1
#define MAX_ZOOM	3.0

class SatImageWidgetData
{
public:
	bool imgReady;
	QPixmap map;
	QPixmap drawMap;

	SatImgPainter painter;

	// Target Image Boundary
	QRect imgRect;

	// Zoom/Pan/Select control
	double zoom = 1.0;
	bool moving = false;
	QPoint offset;
	QPoint moveStartPos;
	Qt::KeyboardModifiers keyMod = Qt::NoModifier;

	// AIS 
	QPen defaultPen = QPen(Qt::red);
	QPen selectPen = QPen(Qt::green);

	GeoTiffFile* tiff = 0;
	QVector<QPolygonF> landmask;
	bool onLandmask = false;

	SatImageWidget::PickMode pickMode = SatImageWidget::PickNormal;

	bool copy;

	double res = 1.0;
	bool onFiltering = false;

	QFuture<QPixmap> future;
};

/*!
@brief Satellite Image Canvas Widget
*/
SatImageWidget::SatImageWidget(QWidget *parent)
	: QWidget(parent)
{
	_d = new SatImageWidgetData;
	_d->defaultPen.setWidth(1);
	_d->selectPen.setWidth(1);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);

	connect(&_d->painter, &SatImgPainter::imageReady, this, &SatImageWidget::readyImage);
}

SatImageWidget::~SatImageWidget()
{
	delete _d;
}

/*!
@brief Set Imageries
*/
void SatImageWidget::setImages(GeoTiffFile * tiff[3])
{
	_d->tiff = tiff[0];
	_d->map = QPixmap();
	_d->drawMap = QPixmap();

	_d->res = fabs(_d->tiff->geoTransform()[1]);
	_d->painter.setData(tiff);
}

void SatImageWidget::setDrawRange(int min[3], int max[3])
{
	_d->painter.setDrawRange(min, max);
	_d->painter.start();
}

/*!
@brief Set Patch Range
*/
void SatImageWidget::setDrawBoundary(const QRect & boundary)
{
	_d->imgRect = boundary;
	_d->zoom = qMin(width() / (double)_d->imgRect.width(), height() / (double)_d->imgRect.height());
	_d->offset = QPoint(0, 0);

	_d->painter.setDrawBoundary(boundary);
	//_d->painter.stop();
	makeLoadingImage();
	_d->painter.start();
}

/*!
@brief Set Mouse Pick Mode
@details Mouse pick mode setting. ie. to add a new ais data
*/
void SatImageWidget::pick(PickMode mode)
{
	_d->pickMode = mode;
}

/*!
@brief Paint Event
@details Main Drawing Method
*/
void SatImageWidget::paintEvent(QPaintEvent * e)
{
	if (_d->drawMap.isNull()) return;

	QPainter p(this);

	// Draw Base SAR Image
	p.drawPixmap(_d->offset, _d->drawMap);

	if (!_d->imgReady) return;
}

void SatImageWidget::resizeEvent(QResizeEvent * e)
{
	QWidget::resizeEvent(e);
	if (_d->imgReady == false)
	{
		// 현재 SAR 영상이 없다면 로딩 이미지를 다시 생성하여 도시
		makeLoadingImage();
	}
	else
	{
		// 현재 SAR 이미지 크기 조정하여 도시
		makeDrawMap();
	}
	update();
}

void SatImageWidget::mousePressEvent(QMouseEvent * e)
{
	QWidget::mousePressEvent(e);
	//if (e->button() == Qt::LeftButton)
	//{
	//	_d->moving = true;
	//	_d->moveStartPos = e->pos();

	//}

	if (e->button() == Qt::LeftButton)
	{
		QPointF xy = toImagePos(e->pos());
		QPointF ll = image2LonLat(xy);
		QPointF imgXy = _d->imgRect.topLeft() + xy;
		if (_d->pickMode == PickAddAis)
		{
			emit picked(_d->pickMode, ll, imgXy);
			_d->pickMode = PickNormal;
			return;
		}

		_d->moveStartPos = e->pos();

		_d->moving = true;
	}
}

void SatImageWidget::mouseMoveEvent(QMouseEvent * e)
{
	if (!_d->imgReady || _d->map.isNull() ) return;


	QWidget::mouseMoveEvent(e);
	if (_d->moving)
	{
		// 마우스 클릭 지점에서 offset 계산
		QPoint offset = _d->moveStartPos - e->pos();
		_d->offset -= offset;

		_d->moveStartPos = e->pos();
		update();
	}
	else
	{
		// Screen Pos to Image Pos
		QPointF imgPos = toImagePos(e->pos());

		QPointF ll;
		//QPoint p = screentPos + _d->cropTopLeft;
		if (_d->tiff)
		{
			ll = image2LonLat(imgPos);

			QPointF rp = imgPos + _d->imgRect.topLeft();

			int v = _d->tiff->value(rp.x(), rp.y());

			// Current Position Signal
			emit currentPos(QPoint(imgPos.x(), imgPos.y()), ll, v);

			qApp->restoreOverrideCursor();
		}
	}
}

void SatImageWidget::mouseReleaseEvent(QMouseEvent * e)
{
	QWidget::mouseReleaseEvent(e);
	_d->moving = false;

	qApp->restoreOverrideCursor();
}

/*!
@brief	Wheel Event
@details Zoom in/out or Rotate Ship
*/
void SatImageWidget::wheelEvent(QWheelEvent * e)
{
	// Screen Zoomming
	double prevZoom = _d->zoom;

	double incZoom = _d->zoom / 10.0;

	_d->zoom += (e->angleDelta().y() > 0 ? incZoom : -incZoom);
	_d->zoom = qMax(_d->zoom, MIN_ZOOM);
	_d->zoom = qMin(_d->zoom, MAX_ZOOM);

	//if ((int)(prevZoom * 10) == (int)(_d->zoom * 10))
	//{
	//	return;
	//}

	QPoint refP = e->pos() - _d->offset;

	QPoint pz = refP - (refP * _d->zoom / prevZoom);
	_d->offset = _d->offset + pz;

	if (_d->future.isFinished())
	{
		makeDrawMap();
	}
	update();
}

void SatImageWidget::keyPressEvent(QKeyEvent * e)
{
	QWidget::keyPressEvent(e);
	_d->keyMod = e->modifiers();
}

void SatImageWidget::keyReleaseEvent(QKeyEvent * e)
{
	QWidget::keyReleaseEvent(e);
	_d->keyMod = Qt::NoModifier;
	qApp->restoreOverrideCursor();
}

void SatImageWidget::refresh()
{
	makeDrawMap();
	update();
}

void SatImageWidget::readyImage()
{
	_d->imgReady = true;
	const uchar* buffer = _d->painter.image();
	QImage image(buffer, _d->imgRect.width(), _d->imgRect.height(), SatImgPainter::imageFormat());

	_d->map = QPixmap::fromImage(image);
	refresh();

	emit imageReady();
}

void SatImageWidget::makeDrawMap()
{
	if (_d->map.isNull()) return;
	QSize s = _d->map.size() * _d->zoom;
	_d->drawMap = _d->map.scaled(s);// , Qt::AspectRatioMode::KeepAspectRatio);
}

void SatImageWidget::makeLoadingImage()
{
	_d->imgReady = false;
	_d->map = QPixmap(size());
	QPainter p(&_d->map);
	p.fillRect(_d->map.rect(), Qt::black);

	QString msg = "Loading...";
	drawText(p, msg);
	_d->drawMap = _d->map;

	update();
}

void SatImageWidget::drawText(QPainter & p, const QString & msg)
{
	p.save();
	p.setRenderHint(QPainter::TextAntialiasing);
	p.setRenderHint(QPainter::HighQualityAntialiasing);
	//p.fillRect(_d->map.rect(), Qt::black);
	QPen pen(Qt::white);
	QBrush brush(Qt::black);
	pen.setWidth(2);
	p.setPen(pen);
	p.setBrush(brush);

	QPainterPath text;
	QFont font("Times New Roman");
	font.setItalic(true);
	font.setPointSize(50);

	QFontMetrics fm(font);
	int w = fm.width(msg);
	int h = fm.height();
	text.addText(rect().center() - QPoint(w / 2, h / 2), font, msg);
	//text.addText(_d->map.rect().center() - QPoint(w / 2, h / 2), font, msg);
	//text.addText(QPoint(300, 100) - QPoint(w / 2, h / 2), font, msg);
	p.drawPath(text);
	p.restore();
}

/*!
@brief	Screent to Image grid
@details Calculate a scren position to a Image position
*/
QPointF SatImageWidget::toImagePos(const QPointF & screenPos)
{
	QPointF imagePos = (screenPos - _d->offset) / _d->zoom;

	return imagePos;
}

QPointF SatImageWidget::toScreenPos(const QPointF & imagePos)
{
	QPointF screenPos = (imagePos * _d->zoom) + _d->offset;
	return screenPos;
}

/*!
@brief Screen to LonLat grid
*/
QPointF SatImageWidget::toLonLat(const QPointF & screenPos)
{
	return image2LonLat(toImagePos(screenPos));
}

/*!
@brief Image to LonLat grid
*/
QPointF SatImageWidget::image2LonLat(const QPointF & imgPos)
{
	QPointF ll;
	if (_d->tiff)
	{
		QPointF p = imgPos + _d->imgRect.topLeft();
		ll = _d->tiff->latLon(p.x(), p.y());
	}

	return ll;
}

/*!
@brief LonLat to Image grid
*/
QPointF SatImageWidget::lonlat2image(double lon, double lat)
{
	QPointF img;
	if (_d->tiff)
	{
		img = _d->tiff->point(lon, lat);
		img = img - _d->imgRect.topLeft();
	}

	return img;
}
