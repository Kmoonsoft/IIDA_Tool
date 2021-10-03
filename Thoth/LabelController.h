#pragma once
#include <QObject>
#include <GraphicsDrawItem.h>
#include "LabelObject.h"

/*!
@file       PolygonLayer.h
@date       2021.09.01
@author     Lucky
@version    v0.1
@brief		Thot Main Control class Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

class GraphicsDrawItem;

/*!
@brief Main Control calss
*/
class LabelController : public QObject
{
    Q_OBJECT

public:

	// Only ONE(1) instance be created
	static LabelController* create(QObject *parent);
	// Use this method to get the INSTANCE of Thot.
	static LabelController* instance();

	~LabelController();

	bool loadCategories(const QString& path);
	bool openLable(const QString& path);

	bool existCurrentLabelFile() const;

	const class LabelObject* labelObject() const;
	bool isChanged() const;
	bool isNew() const;
	QString categoryName(int categoryId) const;
	Annotation newAnnotation();

	Annotation annotation(int id);
	//int addAnnotation(const Annotation& anno);
	void setAnnotation(const Annotation& anno);

	Category category(int id) const;
	const class CategoryObject* categoryObject() const;

	int imageID(const QString& imageName) const;
	int addImage(const QString& imageName);

	int currentImage() const;
	QString currentImageName() const;
	QList<Annotation> annotations(const QString& imageName);

signals:
	void labelReset();
	void sceneChanged(const QString& name, const QString& path);

	void prevSceneRequested();
	void nextSceneRequested();
	void singleScene(bool single);
	void annotationChanged(Annotation);
	void annotationSelected(const QList<int>& ids);
	void annotationRemoved(const QList<int>& ids);

	void logAdded(QString message, int level = 0);

public slots:
	void addLog(const QString& message, int level = 0);
	bool openCOCO(const QString& cocoPath);
	void setCurrentScene(const QString& name, const QString& path);
	//void grphicsItemSelected(const QList<GraphicsDrawItem*> list);
	void removeAnnotation(const QList<int> ids);

	void newLabels();
	bool saveLabels(const QString& labelPath = QString::null);
	bool openLabels(const QString& labelPath);

private:

private:

	LabelController(QObject *parent = Q_NULLPTR);
	class LabelControllerData* _d = 0;

};
