#ifndef GRAPHICSFREELINEITEM_H
#define GRAPHICSFREELINEITEM_H

#include <QGraphicsRectItem>
#include "GraphicsFigureItemControl.h"

class GraphicsFreeLineItem : public QGraphicsRectItem
{
public:
	GraphicsFreeLineItem( QGraphicsItem* parent );
	virtual	~GraphicsFreeLineItem();

	void setData( const QPolygonF& polygon );
	FreeLineInfo& info() { return _info; }
	void setInfo( const FreeLineInfo& info ) { _info = info; }

private:
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );	
	
private:
	FreeLineInfo _info;
	
	//! 상단값
	qreal _rTop;
	//! 하단값
	qreal _rBottom;
	//! 좌측값
	qreal _rLeft;
	//! 우측값
	qreal _rRight;
};

#endif // GRAPHICSFREELINEITEM_H
