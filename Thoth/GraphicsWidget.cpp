/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QtDebug>
#include <QMenu>
#include <QFileDialog>
#include <QApplication>
#include <QImage>
#include <QtDebug>
#include <QPolygonF>
#include <QLabel>
#include <QColorDialog>

#include <Figure.h>
#include <GraphicsModel.h>
#include <ImageLayer.h>
#include <ImageItem.h>
#include <GraphicsPointItem.h>
#include <ImageLayer.h>
#include <GraphicsLayer.h>
#include <GraphicsRectItem.h>
#include <GraphicsPolygonItem.h>
#include <GeoTiffFile.h>
#include <GraphicsLineItem.h>
#include <GraphicsView.h>

#include "LabelController.h"
#include "CocoJsonObject.h"
#include "NotificationWidget.h"


#include "GraphicsWidget.h"

#define POSDISPLAY_COUNT 1
/*!
@brief GraphicsWidgetData Private Data
*/
class GraphicsWidgetData
{
public:
	//QAction*	captureAction;
	ImageLayer*				imageLayer;
	QGraphicsScene* scene;
	NotificationWidget* noti = 0;
	QLabel* pos[POSDISPLAY_COUNT] = { 0 }; // Display Current Pos

	GraphicsWidgetData(QObject* parent)
	{
		//captureAction = new QAction("Save to image", parent);
		imageLayer = 0;
	};

	//~MapControlData()
	//{
	//	//delete layers;
	//};

	GraphicsModel model;

};

GraphicsWidget::GraphicsWidget(QWidget *parent)
	: GraphicsView(parent)
{
	_d = new GraphicsWidgetData(this);

	_d->noti = new NotificationWidget(this);
	_d->noti->hide();

	for (int i = 0; i < POSDISPLAY_COUNT; i++)
	{
		_d->pos[i] = new QLabel(this);
		_d->pos[i]->setMinimumWidth(400);
		_d->pos[i]->raise();
		_d->pos[i]->setAttribute(Qt::WA_TranslucentBackground, true);
		_d->pos[i]->setText(tr("N/A"));
		_d->pos[i]->move(i, i);
	}

	setFocusPolicy(Qt::StrongFocus);

	initUI();

	LabelController* ctrl = LabelController::instance();
	connect(&_d->model, &GraphicsModel::currentPosition, this, &GraphicsWidget::displayCurrentPos);
	connect(&_d->model, &GraphicsModel::drawDone, this, &GraphicsWidget::addAnnotation);
	connect(&_d->model, &GraphicsModel::pasted, this, &GraphicsWidget::addAnnotation);
	connect(&_d->model, &GraphicsModel::updated, this, &GraphicsWidget::updateAnnotation);
	connect(&_d->model, &GraphicsModel::selected, this, &GraphicsWidget::grphicsItemSelected);
	connect(&_d->model, &GraphicsModel::deleted, ctrl, &LabelController::removeAnnotation);
	connect(&_d->model, &GraphicsModel::contextMenuRequested, this, &GraphicsWidget::showMenu);
	connect(ctrl, &LabelController::labelReset, &_d->model, &GraphicsModel::clearSprites);
	connect(ctrl, &LabelController::logAdded, _d->noti, &NotificationWidget::showNotification);
	connect(ctrl, &LabelController::annotationSelected, this, &GraphicsWidget::selectAnnotations);
}

GraphicsWidget::~GraphicsWidget()
{
	delete _d;
}

GraphicsModel * GraphicsWidget::model() const
{
	return &_d->model;
}

void GraphicsWidget::updateAnnotation(const GraphicsDrawItem* item)
{
	LabelController* ctrl = LabelController::instance();
	Annotation label = ctrl->annotation(item->id());

	switch (item->type())
	{
	case GraphicsDrawItem::PointType:
		break;
	case GraphicsDrawItem::LineType:
		break;
	case GraphicsDrawItem::RectType:
	{
		GraphicsRectItem* rectItem = (GraphicsRectItem *)(item);
		label.type = Annotation::RECT;
		label.rectangle = rectItem->data();
		label.rotate = rectItem->rotation();
	}
	break;
	case GraphicsDrawItem::PolygonType:
		break;
	}

	ctrl->setAnnotation(label);
}

// Load Current Image
void GraphicsWidget::setCurrentImage(const QString & path)
{
	_d->imageLayer->addImage(path);

	QString fileName = QFileInfo(path).fileName();
	QList<Annotation> list = LabelController::instance()->annotations(fileName);

	_d->model.clearSprites();
	_d->model.resetUndoStack();

	QList<GraphicsDrawItem*> items;
	for (const Annotation& anno : list)
	{
		switch (anno.type)
		{
		case Annotation::POINT:
			break;
		case Annotation::LINE:
			break;
		case Annotation::RECT:
		{
			GraphicsRectItem* item = new GraphicsRectItem(0);
			item->setId(anno.id);
			item->setData(anno.rectangle);
			item->setPen(QPen(Qt::green));
			item->rotate(anno.rotate);
			item->setName(anno.categoryName);
			items << item;
		}
			break;
		case Annotation::POLYGON:
		{
			GraphicsPolygonItem* item = new GraphicsPolygonItem(0);
			item->setId(anno.id);
			item->setData(anno.polygon);
			item->setPen(QPen(Qt::blue));
			items << item;

		}
			break;
		}

	}
	_d->model.addSprites(items, false);

	//const CocoJsonObject* coco = LabelController::instance()->cocoJsonObject();
	//QList<COCOAnnotation> list = coco->annotation(fileName);

	//_d->model.clearSprites();
	//_d->model.resetUndoStack();

	//QList<GraphicsDrawItem*> items;
	//for (const COCOAnnotation& anno : list)
	//{
	//	GraphicsRectItem* item = new GraphicsRectItem(0);
	//	item->setData(anno.bbox);
	//	item->setPen(QPen(Qt::green));
	//	items << item;

	//	GraphicsPolygonItem* pItem = new GraphicsPolygonItem(0);
	//	pItem->setData(anno.segmentation);
	//	pItem->setPen(QPen(Qt::blue));
	//	items << pItem;
	//}
	//_d->model.addSprites(items, false);

}

void GraphicsWidget::initUI()
{

	//setContextMenuPolicy(Qt::ActionsContextMenu);
	//connect(_d->captureAction, SIGNAL(triggered(bool)), this, SLOT(saveImage()));
	//addAction(_d->captureAction);

	GraphicsLayer* topLayer = _d->model.topLayer();
	//_d->worldLayer = new GraphicsLayer(_d->model.topLayer(), tr("World"), "World");
	//_d->model.addLayer(_d->worldLayer);

	//_d->worldMap = new ShapeFileLayer(_d->worldLayer, tr("Country Board"), "Country Board");
	//_d->worldMap->open(qApp->applicationDirPath() + "/resource/world_countries_wgs84_region.shp", 30);
	//_d->worldMap->setPen( QPen(QColor(0x66, 0x33, 0xFF, 0xff)) );
	//_d->worldMap->setVisibility(false);
	//_d->worldLayer->addSubLayer(_d->worldMap);

	//_d->gridLayer = new GridLayer(_d->worldLayer, tr("Grid Lines"), "Grid");
	//_d->worldLayer->addSubLayer(_d->gridLayer);
	//_d->model.setGridLayer(_d->gridLayer);

	_d->imageLayer = new ImageLayer(topLayer);
	topLayer->addSubLayer(_d->imageLayer);

	_d->imageLayer->setSingleImage(true);
	_d->model.setImageLayer(_d->imageLayer);

	//QString fname = "C:\\Data\\KOMPSAT\\MSC_201212004615_76718_10751276_1G_PS\\MSC_201212004615_76718_10751276_PS.tif";
	//GeoTiffFile* tiff[3] = { new GeoTiffFile(), new GeoTiffFile(), new GeoTiffFile() };
	//tiff[0]->open(fname);
	//SatImgPainter p;
	//SatImageMeta meta = MetaData::readK2Meta("C:\\Data\\KOMPSAT\\MSC_201212004615_76718_10751276_1G_PS\\MSC_201212004615_76718_10751276PN13_1G.txt");

	//_d->mainImage = new SatImageItem(_d->worldLayer);

	//_d->satImgLayer->addImage(_d->mainImage);
	//_d->model.setSceneRect(_d->satImgLayer->boundingRect());
	//QString path = "C:/Data/KOMPSAT/MSC_201212004615_76718_10751276_1G_PS/sub2000/";
	//SatImageItem* item = new SatImageItem(_d->satImgLayer);
	//SatImageItem* item2 = new SatImageItem(_d->satImgLayer);
	//_d->satImgLayer->addImage(item);
	//connect(_d->mainImage, &SatImageItem::imageReady, this, &Map2DWidget::moveToImage);

	//_d->shapeLayer = new ShapeLayer(_d->worldLayer, tr("Labels"), "Labels");
	//QPen pen = _d->shapeLayer->pen();
	//pen.setColor(Qt::magenta);
	//_d->shapeLayer->setPen(pen);

	//_d->shapeLayer->addLine(QLineF(0, 0, 100, 100));
	//_d->shapeLayer->addPoint(QPointF(200, 200));

	// ºä¿¡ ¸ðµ¨ ¼³Á¤
	setModel(&_d->model);
	setImageLayer(_d->imageLayer);

	//////////////////////////////////////////////////////////////////////
	// TEST
	//////////////////////////////////////////////////////////////////////
	//QLineF tline(QPointF(0, 0), QPointF(100, 100));

	//GraphicsLineItem* li = new GraphicsLineItem(0, true);
	//QLineF line(QPointF(300, -300), QPointF(-10, 50));
	//li->setData(line);
	//li->setPen(QPen(Qt::blue));
	//_d->model.addSprite(li);

	//QPolygonF exline = Gaia::expand(line, 5);
	//GraphicsPolygonItem* exlinei = new GraphicsPolygonItem(0);
	//exlinei->setPen(QPen(Qt::magenta));
	//exlinei->setData(exline);
	//_d->model.addSprite(exlinei);

	//GraphicsPointItem* pi = new GraphicsPointItem(0);
	//pi->setData(QPointF(250, -250));
	//_d->model.addSprite(pi);


	//GraphicsRectItem* ri = new GraphicsRectItem(0);
	//ri->setData(QRectF(10, -500, 200, 100));
	//ri->setPen(QPen(Qt::blue));
	////ri->rotate(120);
	//_d->model.addSprite(ri);

	//GraphicsPolygonItem* poly = new GraphicsPolygonItem(0);
	//QPolygonF polygon;
	//polygon << QPointF(100, 100);
	//polygon << QPointF(300, 200);
	//polygon << QPointF(500, 700);
	//polygon << QPointF(800, 800);
	//poly->setPen(QPen(Qt::blue));
	//poly->setData(polygon);
	//_d->model.addSprite(poly);

	//QPolygonF exPoly = Gaia::expand(polygon, 5);
	//GraphicsPolygonItem* poly2 = new GraphicsPolygonItem(0);
	//poly2->setPen(QPen(Qt::magenta));
	//poly2->setData(exPoly);
	//_d->model.addSprite(poly2);

	//connect(LabelController::instance(), &LabelController::singleScene, this, &GraphicsWidget::setSingleImage);
}

void GraphicsWidget::moveToImage()
{
	ImageItem* a = dynamic_cast<ImageItem*>(sender());
	//qDebug() << _d->model.sceneRect();
	//_d->model.setFocusItem(a);
	//_d->model.setSceneRect(_d->mainImage->boundingRect());
	
}

void GraphicsWidget::addAnnotation(const GraphicsDrawItem * item)
{
	LabelController* ctrl = LabelController::instance();
	// Add Annotation and return it which has automaticaly assigned id)
	Annotation anno = ctrl->newAnnotation();

	switch (item->type())
	{
	case GraphicsDrawItem::PointType:
		break;
	case GraphicsDrawItem::LineType:
		break;
	case GraphicsDrawItem::RectType:
	{
		GraphicsRectItem* rectItem = (GraphicsRectItem *)(item);
		rectItem->setId(anno.id);
		anno.type = Annotation::RECT;
		anno.rectangle = rectItem->data();
		anno.rotate = rectItem->rotation();
	}
	break;
	case GraphicsDrawItem::PolygonType:
		break;
	}

	ctrl->setAnnotation(anno);
}

// Display Current mouse pos and geo pos
void GraphicsWidget::displayCurrentPos(const QPointF & pos, const QPointF & lonlat)
{
	static const QStringList colors = { "blue", "gray", "white" };

	for (int i = 0; i < POSDISPLAY_COUNT; i++)
	{
		QString posStr = QString::asprintf("<font color=%s size=3><b>X:%05d, Y:%05d, Lon:%0.6f, Lat:%0.6f</b></font>", colors[i].toLatin1().data(), (int)pos.x(), (int)pos.y(), lonlat.x(), lonlat.y());
		_d->pos[i]->setText(posStr);
	}

	//QString str = QString::asprintf("<font color=white size=3><b>X:%05d, Y:%05d, Lon:%0.6f, Lat:%0.6f</b></font>", (int)pos.x(), (int)pos.y(), lonlat.x(), lonlat.y());
	//_d->pos0->setText(str);
}

void GraphicsWidget::setSingleImage(bool single)
{
	_d->imageLayer->setSingleImage(single);
}

void GraphicsWidget::grphicsItemSelected(const QList<int> ids)
{
	LabelController* ctrl = LabelController::instance();
	disconnect(ctrl, &LabelController::annotationSelected, this, &GraphicsWidget::selectAnnotations);
	emit ctrl->annotationSelected(ids);
	connect(ctrl, &LabelController::annotationSelected, this, &GraphicsWidget::selectAnnotations);
}

void GraphicsWidget::selectAnnotations(const QList<int>& ids)
{
	QSignalBlocker blocK(&_d->model);
	_d->model.selectSprites(ids);
}

void GraphicsWidget::saveImage()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save to an Image file", ".", "Image File(*.png *.jpg *.bmp)" );
	if( filePath.isEmpty() ) return;

	QString format = QFileInfo(filePath).suffix().toUpper();
	if( format.isEmpty() ) format = "PNG";
	QPixmap map = QPixmap::grabWidget(this);
	map.save(filePath, format.toLatin1());
}

void GraphicsWidget::showMenu(int id, const QPoint & screenPos)
{
	QMenu menu;
	QAction *setClassAction = menu.addAction(tr("Set Class"));
	QAction *action = menu.exec(screenPos);

	if (action == setClassAction)
	{
	}LabelController::instance()->addLog("Not implemented yet!");
}

void GraphicsWidget::resizeEvent(QResizeEvent * e)
{
	GraphicsView::resizeEvent(e);

	_d->noti->move( QPoint(width() / 2 - _d->noti->width() / 2, 2));
}

void GraphicsWidget::moveEvent(QMoveEvent * e)
{
	GraphicsView::moveEvent(e);
	//_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, height() / 2 - _d->noti->height() / 2));
	_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, 2));
}

