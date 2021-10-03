#include <QtDebug>
#include <QFileDialog>
#include <QApplication>
#include <QImage>
#include <QtDebug>
#include <QPolygonF>
#include <QColorDialog>

#include <Gaia.h>
#include <MapModel.h>
//#include <ShapeFileLayer.h>
#include <ImageLayer.h>
#include <ImageItem.h>
#include <GraphicsPointItem.h>
#include <ImageLayer.h>
//#include <GridLayer.h>
//#include <ShapeLayer.h>
#include "GraphicsRectItem.h"
#include "GraphicsPolygonItem.h"
#include "GeoTiffFile.h"
#include "MetaData.h"
#include <GraphicsLineItem.h>
#include "Thot.h"
#include "MapView.h"
#include "MapWidget.h"
#include "NotificationWidget.h"

#include "CocoJsonObject.h"

#include <MapLayer.h>

QIcon makeColorIcon(const QColor& color, int width = 16, int height = 16)
{
	QPixmap m(width, height);
	QPainter p(&m);
	p.fillRect(0, 0, width, height, color);

	return QIcon(m);
};

class MapControlData
{
public:
	//QAction*	captureAction;
	MapLayer*				worldLayer;
//	ShapeFileLayer*			worldMap;
	GridLayer*				gridLayer;

	ImageLayer*			satImgLayer;
	//SatImageItem*			mainImage  = 0;
	ShapeLayer*				shapeLayer = 0;
	//bool					single = true;
	QGraphicsScene* scene;
	NotificationWidget* noti = 0;

	MapControlData(QObject* parent)
	{
		//captureAction = new QAction("Save to image", parent);

		worldLayer = 0;
		//worldMap = 0;
		gridLayer = 0;
		satImgLayer = 0;
		shapeLayer = 0;
	};

	//~MapControlData()
	//{
	//	//delete layers;
	//};

	MapModel model;

};

MapWidget::MapWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_d = new MapControlData(this);

	_d->noti = new NotificationWidget(this);
	_d->noti->hide();

	setFocusPolicy(Qt::StrongFocus);

	initUI();
}

MapWidget::~MapWidget()
{

}

MapView * MapWidget::mapView() const
{
	return ui.mapView;
}

MapModel * MapWidget::mapModel() const
{
	return &_d->model;
}

// Load Current Image
void MapWidget::setCurrentImage(const QString & path)
{
	_d->satImgLayer->addImage(path);

	const CocoJsonObject* coco = Thot::instance()->cocoJsonObject();
	QString fileName = QFileInfo(path).fileName();
	QList<COCOAnnotation> list = coco->annotation(fileName);

	_d->model.clearSprites();
	_d->model.resetUndoStack();

	QList<GraphicsDrawItem*> items;
	for (const COCOAnnotation& anno : list)
	{
		GraphicsRectItem* item = new GraphicsRectItem(0);
		item->setData(anno.bbox);
		item->setPen(QPen(Qt::green));
		items << item;

		GraphicsPolygonItem* pItem = new GraphicsPolygonItem(0);
		pItem->setData(anno.segmentation);
		pItem->setPen(QPen(Qt::blue));
		items << pItem;
	}
	_d->model.addSprites(items, false);

	//if (_d->single)
	//{
	//	_d->mainImage->openImage(path);
	//}
	//else
	{
		//_d->mainImage->openImage(path);
		//SatImageItem* item = new SatImageItem(_d->satImgLayer);
		//_d->satImgLayer->addImage(item);
		//item->openImage(path);
	}
	//qDebug() << _d->model.sceneRect();

	//_d->mainImage->setVisible(_d->single);
	//_d->satImgLayer->setVisible(!_d->single);
}

void MapWidget::initUI()
{
	connect(Thot::instance(), &Thot::logAdded, _d->noti, &NotificationWidget::showNotification);

	//setContextMenuPolicy(Qt::ActionsContextMenu);
	//connect(_d->captureAction, SIGNAL(triggered(bool)), this, SLOT(saveImage()));
	//addAction(_d->captureAction);

	_d->worldLayer = new MapLayer(_d->model.topLayer(), tr("World"), "World");
	_d->model.addLayer(_d->worldLayer);

	//_d->worldMap = new ShapeFileLayer(_d->worldLayer, tr("Country Board"), "Country Board");
	//_d->worldMap->open(qApp->applicationDirPath() + "/resource/world_countries_wgs84_region.shp", 30);
	//_d->worldMap->setPen( QPen(QColor(0x66, 0x33, 0xFF, 0xff)) );
	//_d->worldMap->setVisibility(false);
	//_d->worldLayer->addSubLayer(_d->worldMap);

	//_d->gridLayer = new GridLayer(_d->worldLayer, tr("Grid Lines"), "Grid");
	//_d->worldLayer->addSubLayer(_d->gridLayer);
	//_d->model.setGridLayer(_d->gridLayer);

	_d->satImgLayer = new ImageLayer(_d->worldLayer);
	_d->worldLayer->addSubLayer(_d->satImgLayer);

	_d->satImgLayer->setSingleImage(true);
	_d->model.setImageLayer(_d->satImgLayer);

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
	ui.mapView->setModel(&_d->model);
	ui.mapView->setImageLayer(_d->satImgLayer);

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

	connect(Thot::instance(), &Thot::singleScene, this, &MapWidget::setSingleImage);
}

void MapWidget::moveToImage()
{
	ImageItem* a = dynamic_cast<ImageItem*>(sender());
	qDebug() << _d->model.sceneRect();
	//_d->model.setFocusItem(a);
	//_d->model.setSceneRect(_d->mainImage->boundingRect());
	
}

void MapWidget::setSingleImage(bool single)
{
	_d->satImgLayer->setSingleImage(single);
}

void MapWidget::saveImage()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save to an Image file", ".", "Image File(*.png *.jpg *.bmp)" );
	if( filePath.isEmpty() ) return;

	QString format = QFileInfo(filePath).suffix().toUpper();
	if( format.isEmpty() ) format = "PNG";
	QPixmap map = QPixmap::grabWidget(this);
	map.save(filePath, format.toLatin1());
}

void MapWidget::resizeEvent(QResizeEvent * e)
{
	QWidget::resizeEvent(e);

	_d->noti->move( QPoint(width() / 2 - _d->noti->width() / 2, 2));
}

void MapWidget::moveEvent(QMoveEvent * e)
{
	QWidget::moveEvent(e);
	//_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, height() / 2 - _d->noti->height() / 2));
	_d->noti->move(QPoint(width() / 2 - _d->noti->width() / 2, 2));
}

