#pragma once

/*!
@file       GraphicsRectItem.h
@date       2021.09.23
@author     ACJ
@version    v0.1
@brief      GraphicsRectItem Class Header file
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

/*!
@brief Rect Graphics Item
*/
class GAIA_EXPORT GraphicsRectItem : public GraphicsDrawItem
{
	enum RectEditMode{ DragMode_None = -1, DragMode_TopLeft, DragMode_Top, DragMode_TopRight, DragMode_Left, DragMode_Right,
					DragMode_BottomLeft, DragMode_Bottom, DragMode_BottomRight, DragMode_Rotate
	};

public:
	GraphicsRectItem( QGraphicsItem* parent );
	virtual ~GraphicsRectItem();

	void setData( const QRectF& rect );
	QRectF boundingRect() const;
	QRectF data() const;
	void rotate(double angle);
	double rotation() const;

	GraphicsRectItem* clone() const;
	int type() const { return RectType; };

	void translate(double dx, double dy);

protected:
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void mousePressEvent( QGraphicsSceneMouseEvent* event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	//void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
	QRectF _rect; // Item Rect
	double _rotate = 0.0f;

	RectEditMode _mode = DragMode_None; // Edit Mode
	QList<QRectF> _editPointRects; // Edit Points
	QTransform _transform;

	//Related Non-Members
public:
	// For serialization
	friend QDataStream & operator<<(QDataStream &out, const GraphicsRectItem &item);
	friend QDataStream & operator>>(QDataStream &in, GraphicsRectItem &item);
};

