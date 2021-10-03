#pragma once

#include <QFrame>
#include "qtutility_global.h"

namespace MOON
{
	//-----------------------------------------------------------------------------
	//	Plain Digital Colock
	//-----------------------------------------------------------------------------
	class QTUTILITY_EXPORT PlainDigitalClock : public QFrame
	{
		Q_OBJECT

	public:
		PlainDigitalClock(QWidget *parent = Q_NULLPTR);
		~PlainDigitalClock();

		void setColor(const QColor& color);
		void setUtc(bool utc = true);

		void setFont(const QFont & f);

	protected:
		void timerEvent(QTimerEvent *event);

	private:
		class PlainDigitalClockData* _d;
	};
}
