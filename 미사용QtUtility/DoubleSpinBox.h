#pragma once

#include <QDoubleSpinBox>
#include "qtutility_global.h"

namespace MOON
{
	class QTUTILITY_EXPORT DoubleSpinBox : public QDoubleSpinBox
	{
		Q_OBJECT

	public:
		DoubleSpinBox(QWidget *parent);
		~DoubleSpinBox();

		virtual void stepBy(int steps);

	};
}