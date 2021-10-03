#pragma once

/*!
@file       Map2DWidget.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Main Canvas Class Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <GraphicsView.h>
//#include "ui_mapwidget.h"

class GraphicsModel;
class GraphicsDrawItem;
class QImage;
struct Annotation;

class GraphicsWidget : public GraphicsView
{
	Q_OBJECT

public:
	GraphicsWidget(QWidget *parent = 0);
	~GraphicsWidget();

	//class GraphicsView* mapView() const;
	class GraphicsModel* model() const;

public slots:
	void	setCurrentImage(const QString& path);
	void	updateAnnotation(const GraphicsDrawItem* item);

private slots:
	void	moveToImage();
	void	addAnnotation(const GraphicsDrawItem* item);
	void	displayCurrentPos(const QPointF& pos, const QPointF & lonlat);
	void	setSingleImage(bool single);
	void	grphicsItemSelected(const QList<int> ids);
	void	selectAnnotations(const QList<int>& ids);
	void	saveImage();
	void	showMenu(int id, const QPoint& screenPos);

protected:
	void resizeEvent(QResizeEvent * e);
	void moveEvent(QMoveEvent * e);

private:
	void			initUI();

private:
	//Ui::MapWidget			 ui;
	class GraphicsWidgetData*	_d;
};

