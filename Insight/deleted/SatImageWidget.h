#pragma once
/*!
@file       PolygonLayer.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief		SatImageWidget Header file
@details
*/

#include <QWidget>
#include "insight_global.h"

class INSIGHT_EXPORT SatImageWidget : public QWidget
{
	Q_OBJECT

public:

	enum PickMode { PickNormal, PickAddAis };
	//enum EditMode { EditRotate, EditHeight, EditWidth };

	SatImageWidget(QWidget *parent);
	~SatImageWidget();

	void setImages(class GeoTiffFile* tiff[3]);
	void setDrawRange(int min[3], int max[3]);
	void setDrawBoundary(const QRect& boundary);

signals:
	void currentPos(const QPoint& p, const QPointF& ll, int value);
	void imageReady();

	// Pick Mode, lonLat, xy of Original Tiff
	void picked(PickMode mode, const QPointF& lonLat, const QPointF& xy);

public slots:
	void pick(PickMode mode);

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent * e);
	void mousePressEvent(QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);
	void wheelEvent(QWheelEvent * e);

	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

private slots:

	void refresh();
	void readyImage();

private:
	void makeDrawMap();
	void makeLoadingImage();
	void drawText(QPainter& p, const QString& msg);

	QPointF toImagePos(const QPointF& screenPos);
	QPointF toScreenPos(const QPointF& imagePos);
	QPointF toLonLat(const QPointF& screenPos);
	QPointF image2LonLat(const QPointF& imgPos);
	QPointF lonlat2image(double lon, double lat);

private:
	class SatImageWidgetData* _d;
};
