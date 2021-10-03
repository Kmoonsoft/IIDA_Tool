#pragma once

/*!
@file       ImageItem.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Satellite Image(Tiff image) Graphics item Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QObject>
#include <QGraphicsPixmapItem>
#include "gaia_global.h"

class GAIA_EXPORT ImageItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_CLASSINFO("DefaultProperty", "children")
	Q_INTERFACES(QGraphicsItem)

public:
	ImageItem(QGraphicsItem* parent = Q_NULLPTR);
	~ImageItem();
	
	using QObject::children;

	bool openImage(const QString& path);
	
	//QRectF boundingRect() const;

	void moveToPos();

	QPointF lonlat(const QPointF& pos);

signals:
	void imageReady();

private:
	void draw();

private:
	class ImageItemData* _d;
};
