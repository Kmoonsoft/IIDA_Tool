#ifndef MAP2DWDGT_H
#define MAP2DWDGT_H

/*!
@file       Map2DWidget.h
@date       2021.09.02
@author     Lucky
@version    v1.0
@brief      Main Canvas Class Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20210902	Lucky		First release(v0.1)
*/

#include <QWidget>
#include "ui_mapwidget.h"

class MapLayer;
class ShapeLayer;
class GridLayer;

class MapModel;
class QImage;

class MapWidget : public QWidget
{
	Q_OBJECT

public:
	MapWidget(QWidget *parent = 0);
	~MapWidget();

	class MapView* mapView() const;
	class MapModel* mapModel() const;

public slots:
	void	setCurrentImage(const QString& path);

private slots:
	void	moveToImage();
	void	setSingleImage(bool single);
	void	saveImage();
	
protected:
	void resizeEvent(QResizeEvent * e);
	void moveEvent(QMoveEvent * e);

private:
	void			initUI();


private:
	Ui::MapWidget			 ui;
	class MapControlData*	_d;
};

#endif // MAP2DWDGT_H
