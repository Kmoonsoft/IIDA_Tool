#pragma once
/*!
@file       SatImageLayer.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Satellite Image(Tiff image) Graphics item Layer Class Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20210902	Lucky		First release(v0.1)
*/

#include "GraphicsLayer.h"
#include "gaia_global.h"

class GAIA_EXPORT SatImageLayer : public GraphicsLayer
{
	//Q_OBJECT

public:
	SatImageLayer(QGraphicsItem *parent);
	~SatImageLayer();


	void addImage(class SatImageItem* item);
	QRectF	boundingRect() const;

	// tiff file must contains B/G/R band
	class SatImageItem* addImage(const QString& tiffPath);
	class SatImageItem* addImage(const QString& bluePath, const QString& greenPath, const QString& redPath);

	void setSingleImage(bool single);

	QPointF lonlat(const QPointF& pos);

private:
	bool	_single = false;
};
