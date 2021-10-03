#include "DoubleSpinBox.h"

MOON::DoubleSpinBox::DoubleSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
{
}

MOON::DoubleSpinBox::~DoubleSpinBox()
{
}

void MOON::DoubleSpinBox::stepBy(int steps)
{
	QDoubleSpinBox::stepBy(steps);
	emit editingFinished();
}
