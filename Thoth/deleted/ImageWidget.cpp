#include <QtDebug>
#include <MapView.h>

#include "Thot.h"
#include "NotificationWidget.h"
#include "ImageWidget.h"

class ImageWidgetData
{
public:
	QButtonGroup* bGroup = 0;
	NotificationWidget* noti = 0;
};

ImageWidget::ImageWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new ImageWidgetData;
	_d->noti = new NotificationWidget(this);
	_d->noti->hide();

	_d->bGroup = new QButtonGroup(this);
	_d->bGroup->addButton(ui.select, 0);
	_d->bGroup->addButton(ui.rect, 1);
	_d->bGroup->addButton(ui.poly, 2);
	_d->bGroup->setExclusive(true);

	connect(ui.image->mapView(), &MapView::currentPosition, this, &ImageWidget::setCurrentPos);
	
	connect(ui.rect, &QToolButton::toggled, this, &ImageWidget::na);
	connect(ui.poly, &QToolButton::toggled, this, &ImageWidget::na);
	connect(ui.merge, &QToolButton::clicked, this, &ImageWidget::na);
	connect(ui.buffer, &QToolButton::clicked, this, &ImageWidget::na);
	connect(ui.sync, &QToolButton::clicked, this, &ImageWidget::na);
	connect(ui.osm, &QToolButton::clicked, this, &ImageWidget::na);

	connect(Thot::instance(), &Thot::logAdded, _d->noti, &NotificationWidget::showNotification);
}

ImageWidget::~ImageWidget()
{
	delete _d;
}

MapWidget * ImageWidget::imageWidget() const
{
	return ui.image;
}

void ImageWidget::resizeEvent(QResizeEvent * e)
{
	QWidget::resizeEvent(e);
	_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, ui.titleFrame->height() + 2));
}

void ImageWidget::moveEvent(QMoveEvent * e)
{
	QWidget::moveEvent(e);
	//_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, height() / 2 - _d->noti->height() / 2));
	_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, ui.titleFrame->height() + 2));
}

void ImageWidget::na()
{
	QToolButton* o = dynamic_cast<QToolButton*>(sender());
	if (o)
	{
		_d->noti->showNotification(o->text() + tr("! Not implemented yet!"), 1);
	}
}

void ImageWidget::setCurrentPos(const QPointF & pos, const QPointF & lonlat)
{
	QString str = QString::asprintf("x:%5d, y:%5d, Lon:%0.6f, Lat:%0.6f", pos.x(), pos.y(), lonlat.x(), lonlat.y());
	ui.currentPos->setText(str);
	//ui.currentPos->setText(QString("x:%1, y:%2, Lon:%3, Lat:%4").arg((int)pos.x(),5).arg((int)pos.y(), 5)
	//	.arg(QString::number(lonlat.x(), 'f', 6)).arg(QString::number(lonlat.y(), 'f', 6)));
}
