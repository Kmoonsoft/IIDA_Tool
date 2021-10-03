#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include "GraphicsFreeLineItem.h"
#include "WorkspaceGraphicsScene.h"
#include "WorkspaceUndoCommand.h"

GraphicsFreeLineItem::GraphicsFreeLineItem( QGraphicsItem* parent )
	: QGraphicsRectItem( parent )
{	
	setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
}

GraphicsFreeLineItem::~GraphicsFreeLineItem()
{

}

void GraphicsFreeLineItem::setData( const QPolygonF& polygon )
{
	_info._polygon = polygon;

	_rTop = _rBottom = polygon[ 0 ].y();
	_rLeft = _rRight = polygon[ 0 ].x();
	
	for( int i = 1 ; i <  polygon.count() ; i++ )
	{
		_rTop = qMin( _rTop, polygon[ i ].y() );
		_rBottom = qMax( _rBottom, polygon[ i ].y() );
		_rLeft = qMin( _rLeft, polygon[ i ].x() );		
		_rRight = qMax( _rRight, polygon[ i ].x() );
	}

	QRectF tRect;
	tRect.setLeft( _rLeft - 10 );
	tRect.setRight( _rRight + 10 );
	tRect.setTop( _rTop - 10 );
	tRect.setBottom( _rBottom + 10 );
	setRect( tRect );
	update();
}

void GraphicsFreeLineItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform( painter->worldTransform() );

	QTransform t = painter->worldTransform();
	t.scale( 1.0 / lod, 1.0 / lod );	// adjust scale for adjust line width
	painter->setTransform( t );
	
	double dMargin = 10 / lod;	

	QRectF tRect;
	tRect.setLeft( _rLeft - ( 10 / lod ) );
	tRect.setRight( _rRight + ( 10 / lod ) );
	tRect.setTop( _rTop - ( 10 / lod ) );
	tRect.setBottom( _rBottom + ( 10 / lod ) );
	setRect( tRect );

	painter->save();

	painter->setPen( _info._drawInfo._pen );	
	painter->setBrush( _info._drawInfo._brush );

	QPainterPath pp;
	
	for( int i = 0 ; i < _info._polygon.count() ; i++ )
	{
		if( i == 0 )	pp.moveTo( _info._polygon[ i ] * lod );
		else			pp.lineTo( _info._polygon[ i ] * lod );
	}

	if( _info._drawInfo._bClosed )	pp.lineTo( _info._polygon.first() * lod );
	
	pp.setFillRule( _info._drawInfo._fillRule );
	painter->drawPath( pp );
	
	if( group() == NULL && option->state & QStyle::State_Selected )
	{
		QPen newPen;
		newPen.setStyle( Qt::DashLine );
		painter->setPen( newPen );		
		painter->setBrush( Qt::transparent );
		painter->drawRect( rect().x() * lod, rect().y() * lod, rect().width() * lod, rect().height() * lod );	
		
		painter->setPen( QPen( Qt::yellow ) );
		painter->setBrush( Qt::yellow );
				
		float fPointSize = 10;
		
		fPointSize = fPointSize / lod;		
	}	

	painter->restore();
}

void GraphicsFreeLineItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
	setSelected( true );
	
	WorkspaceGraphicsScene* tScene = dynamic_cast<WorkspaceGraphicsScene*>( scene() );
	if( tScene == NULL )	return;

	QMenu menu;		
	GraphicsFigureItemControl::instance()->addLineMenu( menu );
	GraphicsFigureItemControl::instance()->addFillMenu( menu, _info._drawInfo );
	menu.addSeparator();
	menu.addAction( GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill ) );
	menu.addAction( GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill ) );
	
	if( _info._drawInfo._fillRule == Qt::OddEvenFill )
	{
		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setChecked( true );
		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setEnabled( false );

		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setChecked( false );
		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setEnabled( true );
	}
	if( _info._drawInfo._fillRule == Qt::WindingFill )
	{
		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setChecked( false );
		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setEnabled( true );

		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setChecked( true );
		GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setEnabled( false );
	}
	menu.addAction( GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_Closed ) );
	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_Closed )->setChecked( _info._drawInfo._bClosed );
	menu.addSeparator();	
	GraphicsFigureItemControl::instance()->addGroupMenu( menu, tScene );
	menu.addSeparator();
	GraphicsFigureItemControl::instance()->addMoveZMenu( menu );
	
	QAction* action = menu.exec( event->screenPos() );
	if( action == NULL )	return;

	GraphicsFigureItemControl::instance()->processMenuAction( action, _info._drawInfo, tScene );	

	update();
}