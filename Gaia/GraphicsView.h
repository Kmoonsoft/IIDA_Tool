#pragma once

/*!
@file       GraphicsView.h
@date       2021.09.02
@author     ACJ
@version    v0.1
@brief      Main GraphicsView Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include <QGraphicsView>
#include <QTime>
#include "gaia_global.h"

class GraphicsModel;

/*!
@brief GraphicsView is a main view to manage all graphics items
*/
class GAIA_EXPORT GraphicsView : public QGraphicsView
{
	Q_OBJECT

public:

	/*!
	@brief Define draw mode with mouse events
	*/
	enum DrawMode
	{
		DrawNone, DrawRect, DrawPoygon, DrawMerge, DrawSync, DrawBuffer

	};

	GraphicsView(QWidget *parent);
	~GraphicsView();

	void	setModel(GraphicsModel* model);
	void	addAction( QAction* action );
	void	setImageLayer(class ImageLayer* layer);

signals:
	void	currentPosition(QPointF imgPos, QPointF lonlat);

public slots:

private slots:
	void	setupMatrix();
	void	saveImage();
	void	saveToClip();


protected:
	void	keyPressEvent ( QKeyEvent * event );
	//void	keyReleaseEvent ( QKeyEvent * event );
	void	wheelEvent ( QWheelEvent * event );
	void	mousePressEvent ( QMouseEvent * event );
	void	mouseMoveEvent ( QMouseEvent * event );
	void	mouseReleaseEvent(QMouseEvent * event);

	void	paintEvent ( QPaintEvent * event );

private:
	class GraphicsViewData*	_d;
	QTime	_moveTime;

	QAction* _separator;
	class ImageLayer* _imglayer = 0;
};

