/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QFile>
#include <QFileInfo>
#include <QStack>

#include "CocoJsonObject.h"
#include "Settings.h"
#include "CategoryObject.h"
#include "ImageObject.h"
#include "LabelController.h"

class LabelControllerData
{
public:
	//CocoJsonObject coco;
	
	LabelObject labels;
	CategoryObject categories;

	QString currentImageName;
	int currentImageID = -1;
};

// Static Only one LabelController
static LabelController* g_myself = 0;

// Private constructor
LabelController::LabelController(QObject *parent)
    : QObject(parent)
{
	_d = new LabelControllerData;
	//qRegisterMetaType<GraphicsDrawItem>("GraphicsDrawItem");

	connect(&_d->labels, &LabelObject::reset, this, &LabelController::labelReset);
	connect(&_d->labels, &LabelObject::changed, this, &LabelController::annotationChanged);

	// Load Categories
	QString objClassPath = Settings::getFilePath(ObjectClassPath);
	if(!objClassPath.isEmpty()) loadCategories(objClassPath);
}

LabelController::~LabelController()
{
	delete _d;
}

bool LabelController::loadCategories(const QString & path)
{
	return _d->categories.open(path);
}

bool LabelController::openLable(const QString & path)
{
	return _d->labels.open(path);
}

bool LabelController::existCurrentLabelFile() const
{
	return false;
}

const LabelObject * LabelController::labelObject() const
{
	return &_d->labels;
}

bool LabelController::isChanged() const
{
	return _d->labels.isChanged();
}

bool LabelController::isNew() const
{
	return _d->labels.isNew();
}

QString LabelController::categoryName(int categoryId) const
{
	Category cat = _d->categories.category(categoryId);
	return (cat.isNull() ? tr("Unknown") : cat.name);
}

void LabelController::addLog(const QString & message, int level)
{
	emit logAdded(message, level);
}

LabelController* LabelController::create(QObject *parent)
{
	if (g_myself == 0)
	{
		g_myself = new LabelController(parent);
	}

	return g_myself;
}

LabelController * LabelController::instance()
{
	return g_myself;
}

bool LabelController::openCOCO(const QString& cocoPath)
{
	return false;// coco.open(cocoPath);
}

void LabelController::setCurrentScene(const QString & name, const QString & path)
{
	QFileInfo fi(path);
	_d->currentImageName = fi.fileName();

	_d->currentImageID = _d->labels.insertImage(_d->currentImageName);

	emit sceneChanged(name, path);
}

//void LabelController::grphicsItemSelected(const QList<GraphicsDrawItem*> list)
//{
//	QList<int> ids;
//	for (const GraphicsDrawItem* item : list)
//	{
//		ids << item->id();
//	}
//
//	emit annotationSelected(ids);
//}

Annotation LabelController::newAnnotation()
{
	Annotation anno = _d->labels.newAnnotation();
	anno.imageID = _d->currentImageID;
	anno.imageName = _d->currentImageName;

	return anno;
}

Annotation LabelController::annotation(int id)
{
	return _d->labels.annotation(id);
}

//int LabelController::addAnnotation(const Annotation & anno)
//{
//	int id = _d->labels.addAnnotation(anno);
//	return id;
//}

void LabelController::setAnnotation(const Annotation & anno)
{
	_d->labels.setAnnotation(anno);
}

void LabelController::removeAnnotation(const QList<int> ids)
{
	for (int id : ids)
	{
		_d->labels.removeAnnotation(id);
	}
	emit annotationRemoved(ids);
}

Category LabelController::category(int id) const
{
	return _d->labels.category(id);
}

const CategoryObject * LabelController::categoryObject() const
{
	return &_d->categories;
}

int LabelController::imageID(const QString & imageName) const
{
	return _d->labels.imageObject().id(imageName);
}

int LabelController::addImage(const QString & imageName)
{
	return _d->labels.insertImage(imageName);
}

int LabelController::currentImage() const
{
	return _d->currentImageID;
}

QString LabelController::currentImageName() const
{
	return _d->currentImageName;
}

QList<Annotation> LabelController::annotations(const QString & imageName)
{
	return _d->labels.annotations(imageName);
}

// Mak new Label list
void LabelController::newLabels()
{
	_d->labels.createNew();
}

bool LabelController::saveLabels(const QString & labelPath)
{
	return _d->labels.save(labelPath);
}

bool LabelController::openLabels(const QString & labelPath)
{
	return _d->labels.open(labelPath);
}

