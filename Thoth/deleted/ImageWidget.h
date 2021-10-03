#pragma once

#include <QWidget>
#include "ui_ImageWidget.h"

class ImageWidget : public QWidget
{
	Q_OBJECT

public:
	ImageWidget(QWidget *parent = Q_NULLPTR);
	~ImageWidget();

	class MapWidget* imageWidget() const;

public slots:
	void setCurrentPos(const QPointF& pos, const QPointF & lonlat);

protected:
	void resizeEvent(QResizeEvent * e);
	void moveEvent(QMoveEvent * e);

private slots:
	void na();

private:
	Ui::ImageWidget ui;
	class ImageWidgetData* _d;
};
