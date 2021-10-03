#pragma once

/*!
@file       Gaia.h
@date       2021.09.17
@author     Lucky
@version    v0.1
@brief      Item ���� �⺻ ���� ��� Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QLineF>
#include <QRectF>
#include <QColor>
#include <QIcon>
#include "gaia_global.h"

/*!
@brief Item management tools
*/
class GAIA_EXPORT Figure
{
public:

	// Rect Ȯ��
	static QRectF expand(const QRectF& rect, double buffer);
	
	// Point Ȯ��
	static QRectF expand(const QPointF& point, double buffer);
	
	// Line Ȯ��
	static QPolygonF expand(const QLineF& line, double buffer);
	
	// Polygon Ȯ��
	static QPolygonF expand(const QPolygonF& line, double buffer);

	static bool intersected(const QLineF& line, const QPointF& point, double buffer, QPointF& interSection);
	static bool intersected(const QPolygonF& line, const QPointF& point, double buffer, QPointF& interSection, int& idx );

	// Line�� Boundary
	static QRectF boundingRect(const QLineF& line);

	// ���� ����
	static QColor invert(const QColor& color);

	// Color Rect Icon
	static QIcon makeColorIcon(const QColor& color, int width = 16, int height = 16);

};
