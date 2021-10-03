#pragma once
/*!
@file       ImageLayer.h
@date       2021.09.09
@author     Lucky
@version    v0.1
@brief      Image(Tiff image) Layer Header file
@details	Contains ImageItem and display
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include "gaia_global.h"
#include "GraphicsLayer.h"

/*
@brief ImageLayer Class
*/
class GAIA_EXPORT ImageLayer : public GraphicsLayer
{
	
public:
	ImageLayer(QGraphicsItem * parent);
	~ImageLayer();

	/*!
	@brief Add a new ImageItem from a tiff file(path)
	*/
	class ImageItem* addImage(const QString& path);

	/*!
	@brief Get Lon/Lat position(by ImageItem/Tiff)
	@param pos : Scene Position(same as image postion)
	*/
	QPointF lonlat(const QPointF& pos);

	void setSingleImage(bool single);

private:
	bool	_single = true;
};



