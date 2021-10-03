#pragma once

/*!
@file       GraphicsLineItem.h
@date       2021.09.23
@author     ACJ
@version    v0.1
@brief      GraphicsLineItem 클래스 Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include "GraphicsDrawItem.h"
#include "gaia_global.h"

/*!
@brief Line아이템 클래스
@details Line 아이템. 화살표 지정 가능
*/
class GAIA_EXPORT GraphicsLineItem : public GraphicsDrawItem
{
public:
	enum ArrowStyle { ArrowNone, ArrowPoint, ArrowTriangle, ArrowFillTriangle };

	GraphicsLineItem( QGraphicsItem* parent, bool bArrow = false );
	virtual ~GraphicsLineItem();

	void setData(const QPointF& startPos, const QPointF& endPos, ArrowStyle arraw = ArrowNone );
	void setData( const QLineF& line, ArrowStyle arraw = ArrowNone);
	QLineF data() const;
	GraphicsLineItem* clone() const;

	//QRectF boundingRect() const;
	QPainterPath shape() const;

	int type() const { return LineType; };
	void translate(double dx, double dy);

protected:
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void mousePressEvent( QGraphicsSceneMouseEvent* event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
	void updateData();


protected:
	QLineF _line;

	QRectF _selectRect[2];
	int _selIdx = -1;

	ArrowStyle _arrow;
};

