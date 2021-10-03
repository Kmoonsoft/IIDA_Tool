#pragma once

/*!
@file       GraphicsPolygonItem.h
@date       2021.09.23
@author     ACJ
@version    v0.1
@brief      GraphicsPolygonItem 클래스 Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include "GraphicsDrawItem.h"
#include "gaia_global.h"

class GAIA_EXPORT GraphicsPolygonItem : public GraphicsDrawItem
{

public:
	GraphicsPolygonItem( QGraphicsItem* parent );
	virtual ~GraphicsPolygonItem();

	void setData( const QPolygonF& polygon, bool closed = true );
	QPolygonF data() const;
	GraphicsPolygonItem* clone() const;

	bool deleteSelectedPoint();
	int type() const { return PolygonType; };
	void translate(double dx, double dy);

protected:
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
	void updateData();

private:
	QPolygonF _polygon;

	//! 폴리곤의 각 지점 포인터 선택 영역
	QList<QRectF> _selectRects;
	int _dragIdx = -1;
	bool _closed = true;
};

