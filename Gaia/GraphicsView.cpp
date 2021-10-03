/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include <QClipboard>
#include <QtGui>
#include <QtWidgets>
#include <QtDebug>

#include "Figure.h"
#include "GraphicsModel.h"
#include "GraphicsRectItem.h"
#include "GraphicsView.h"
#include "ImageLayer.h"

class GraphicsViewData
{
public:
	GraphicsViewData()
	{
		zoom = 250;
		//modKey = Qt::NoModifier;
	}

	int	zoom;
	//Qt::KeyboardModifiers	modKey;
	QPoint prevPos;

	bool	drawRectStart = false;
	QGraphicsItem* drawItem = 0;
	qreal scale;

	GraphicsModel* model = 0;
};

GraphicsView::GraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	_d = new GraphicsViewData;

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(transform());

	setMouseTracking(true);
	this->setContextMenuPolicy (Qt::ActionsContextMenu);
	_moveTime.start();

	_separator = new QAction(this);
	_separator->setSeparator(true);
	QGraphicsView::addAction(_separator);
	_separator->setVisible(false);

	// Zoom center on mouse
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

GraphicsView::~GraphicsView()
{
	delete _d;
}

void GraphicsView::keyPressEvent ( QKeyEvent * e )
{
	QGraphicsView::keyPressEvent(e);
}

void GraphicsView::wheelEvent(QWheelEvent * event)
{

	_d->zoom += event->delta() / 10;
	setupMatrix();
	event->ignore();

}

void GraphicsView::setupMatrix()
{
    _d->scale = qPow(2.0, (_d->zoom - 250) / 50.0);

	QTransform matrix;// = _d->baseTransform;
    matrix.scale(_d->scale, _d->scale);
	
	setTransform(matrix);
}

void GraphicsView::mousePressEvent ( QMouseEvent * event )
{
	_d->prevPos = event->pos();

	QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent ( QMouseEvent * event )
{
	if (_d->model )
	{
		setCursor((_d->model->drawMode() == GraphicsModel::DrawNone) ? Qt::ArrowCursor : Qt::CrossCursor);
	}

	//// move map
	if( event->buttons() & Qt::LeftButton)
	{
		if(_d->model && _d->model->drawMode() == GraphicsModel::DrawNone && _d->model->selectedItems().isEmpty())
		{
			QPointF df = _d->prevPos - event->pos();
			_d->prevPos = event->pos();

			//this->scroll(event->x(), event->y() );
			this->horizontalScrollBar()->setValue(horizontalScrollBar()->value() + df.x());
			this->verticalScrollBar()->setValue(verticalScrollBar()->value() + df.y());
			viewport()->update();
		}
	}
	
	QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent * event)
{
	if (_d->model && _d->model->drawMode() != GraphicsModel::DrawNone)
	{
		setCursor(Qt::ArrowCursor);
	}

	QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::paintEvent ( QPaintEvent * event )
{
	QGraphicsView::paintEvent(event); // 지도를 먼저 그린다.
}

void GraphicsView::setModel(GraphicsModel* model )
{
	_d->model = model;
	setScene((QGraphicsScene*)model);
}

void GraphicsView::addAction( QAction* action )
{
	_separator->setVisible(true);
	QGraphicsView::insertAction( _separator, action );
}

void GraphicsView::setImageLayer(ImageLayer * layer)
{
	_imglayer = layer;
}

void GraphicsView::saveImage()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save to an Image file", ".", "Image File(*.png *.jpg *.bmp)" );
	if( filePath.isEmpty() ) return;

	QString format = QFileInfo(filePath).suffix().toUpper();
	if( format.isEmpty() ) format = "PNG";

	QPixmap pm = QPixmap::grabWidget(this);
	pm.save(filePath, format.toLatin1());
}

void GraphicsView::saveToClip()
{
	QPixmap pm = QPixmap::grabWidget(this);
	QClipboard* cb = QApplication::clipboard();
	cb->setImage(pm.toImage());
}
