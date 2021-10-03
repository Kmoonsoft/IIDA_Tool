/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include "Figure.h"
#include "GraphicsLineItem.h"

static const double PI = 3.14159265358979323846264338327950288419717;

GraphicsLineItem::GraphicsLineItem( QGraphicsItem *parent, bool bArrow )
	: GraphicsDrawItem(parent)
{
	_arrow = ArrowNone;
	setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
}

GraphicsLineItem::~GraphicsLineItem()
{

}

void GraphicsLineItem::setData( const QPointF& startPos, const QPointF& endPos, ArrowStyle arraw)
{
	_line.setP1(startPos);
	_line.setP2(endPos);

	setData(_line, arraw);
}

void GraphicsLineItem::setData(const QLineF & line, ArrowStyle arrow)
{
	_line = line;
	_arrow = arrow;
	_internalChanged = false;

	QRectF rect = Figure::boundingRect(_line);
	setRect(rect);

	update();
}

QLineF GraphicsLineItem::data() const
{
	return _line;
}

GraphicsLineItem * GraphicsLineItem::clone() const
{
	GraphicsLineItem* item = new GraphicsLineItem(0);
	item->setData(_line);
	item->setPen(pen());
	item->setBrush(brush());
	return item;
}

/*
QRectF GraphicsLineItem::boundingRect() const
{
    return _selectionArea.boundingRect();
}
*/
QPainterPath GraphicsLineItem::shape() const
{
	return GraphicsDrawItem::shape();

 //   QPainterPath ret;	
	//ret.setFillRule( Qt::WindingFill );
	//ret.addRect(rect());
	//ret.addRect( _startRect );
	//ret.addRect( _endRect );
 //   return ret;
}

void GraphicsLineItem::translate(double dx, double dy)
{
	_line.translate(dx, dy);
	setData(_line);
}

void GraphicsLineItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QTransform t = painter->worldTransform();
	const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform( t );

	painter->save();
	painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
	QPen pen = GraphicsDrawItem::pen();
	pen.setWidthF(pen.widthF() / lod);
	painter->setPen( pen );	
	painter->drawLine( _line );	 // Draw Line

	// Draw Arraw
	if(_arrow != ArrowStyle::ArrowNone)
	{
		painter->setBrush(brush());
		QPen newPen = pen;
		newPen.setStyle( Qt::SolidLine );
		newPen.setJoinStyle( Qt::MiterJoin );
		painter->setPen( newPen );		

		QLineF line2( _line.p2(), _line.p1() );
		if ( qFuzzyCompare( line2.length(), qreal( 0. ) ) ) return;

		double angle = ::atan2( -line2.dy(), line2.dx() );
		QPointF sourceArrowP1 = line2.p1() + QPointF( sin( angle + PI / 3 ) * 15, cos( angle + PI / 3 ) * 15 )/lod;
		QPointF sourceArrowP2 = line2.p1() + QPointF( sin( angle + PI - PI / 3 ) * 15, cos( angle + PI - PI / 3 ) * 15 )/lod;

		if( _arrow == ArrowPoint )		painter->drawEllipse( line2.p1() / lod, 5 / lod, 5 / lod );
		else if(_arrow == ArrowTriangle)
		{
			painter->drawPolyline( QPolygonF() << sourceArrowP2 << line2.p1() << sourceArrowP1 );
		}
		else if(_arrow == ArrowFillTriangle)
		{
			painter->drawPolygon( QPolygonF() << sourceArrowP2 << line2.p1() << sourceArrowP1 );
		}		
	}

	// Draw Select Box and Edit Point
	if( group() == NULL && option->state & QStyle::State_Selected )
	{
		QPen selectPen = pen;
		selectPen.setColor(Qt::gray);
		selectPen.setStyle(Qt::DashLine);
		painter->setPen(selectPen);
		painter->setBrush( Qt::transparent );
		painter->drawRect(Figure::boundingRect(_line));	// Draw boundary box

		selectPen.setStyle(Qt::SolidLine);
		painter->setPen(selectPen);
		painter->setBrush( Qt::white );
		float fPointSize = _selectPointRadius / lod;
		
		_selectRect[0] = Figure::expand(_line.p1(), fPointSize);
		_selectRect[1] = Figure::expand(_line.p2(), fPointSize);

		// Draw select points
		painter->drawEllipse(_selectRect[0]);
		painter->drawEllipse(_selectRect[1]);

		setRect(Figure::expand(Figure::boundingRect(_line), fPointSize));
	}
	else
	{
		setRect(Figure::boundingRect(_line));
	}

	painter->restore();
}

void GraphicsLineItem::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	if( event->button() == Qt::LeftButton )
	{
		_selIdx = -1;
		for (int i = 0; i < 2; i++)
		{
			if (mapRectToScene(_selectRect[i]).contains(event->scenePos()))
			{
				_selIdx = i;
				break;
			}
		}
	}

	GraphicsDrawItem::mousePressEvent( event );
}

void GraphicsLineItem::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	QPointF s = _line.p1();
	QPointF e = _line.p2();
	if(_selIdx == 0) // Start Pos
	{
		s += event->scenePos() - event->lastScenePos();
		_line.setP1(s);
		updateData();
		event->ignore();
	}
	else if(_selIdx == 1) //End Pos
	{
		e += event->scenePos() - event->lastScenePos();
		_line.setP2(e);
		updateData();
		event->ignore();
	}
	else
	{
		GraphicsDrawItem::mouseMoveEvent(event);
		return;
	}

	_internalChanged = true;
}

void GraphicsLineItem::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	_selIdx = -1;
	GraphicsDrawItem::mouseReleaseEvent( event );
}

void GraphicsLineItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
	setSelected( true );
	
	GraphicsDrawItem::contextMenuEvent(event);
}

void GraphicsLineItem::updateData()
{
	QRectF rect = Figure::boundingRect(_line);
	//QRectF rect = Figure::expand(Figure::boundingRect(line), 3);
	setRect(rect);

	update();
}
