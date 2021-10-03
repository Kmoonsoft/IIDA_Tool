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
	
	//! ��ܰ�
	qreal _rTop;
	//! �ϴܰ�
	qreal _rBottom;
	//! ������
	qreal _rLeft;
	//! ������
	qreal _rRight;
};

#endif // GRAPHICSFREELINEITEM_H
