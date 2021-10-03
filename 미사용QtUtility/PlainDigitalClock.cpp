#include <QGridLayout>
#include <QTimer>
#include <QLabel>
#include <QDateTime>
#include <QPalette>
#include "PlainDigitalClock.h"

using namespace MOON;

namespace MOON
{
	class PlainDigitalClockData
	{
	public:
		//PlainDigitalClockData() {};
		QLabel clock;
		bool   isUtc = false;
	};
}

PlainDigitalClock::PlainDigitalClock(QWidget *parent)
	: QFrame(parent)
{
	setFrameShadow(QFrame::Sunken);
	setFrameShape(QFrame::StyledPanel);

	QGridLayout* layout = new QGridLayout(this);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	_d = new PlainDigitalClockData();
	layout->addWidget(&_d->clock, 0, 0, Qt::AlignLeft);
	//setColor(Qt::green);
	//QFont font = _d->clock.font();
	//font.setBold(true);
	//font.setPixelSize(14);
	//_d->clock.setFont(this->font());
	_d->clock.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	startTimer(1000);
}

PlainDigitalClock::~PlainDigitalClock()
{
	delete _d;
}

void PlainDigitalClock::setColor(const QColor & color)
{
	QPalette pal = _d->clock.palette();
	pal.setColor(QPalette::Text, color);
	_d->clock.setPalette(pal);
}

void MOON::PlainDigitalClock::setUtc(bool utc)
{
	_d->isUtc = utc;
}

void MOON::PlainDigitalClock::setFont(const QFont & f)
{
	QWidget::setFont(f);
	_d->clock.setFont(f);
}

void PlainDigitalClock::timerEvent(QTimerEvent * event)
{
	QDateTime dt = _d->isUtc ? QDateTime::currentDateTimeUtc() : QDateTime::currentDateTime();
	_d->clock.setText(dt.toString("yyyy/MM/dd hh:mm:ss"));
}
