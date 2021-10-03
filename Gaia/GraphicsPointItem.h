#pragma once

/*!
@file       GraphicsPointItem.h
@date       2021.09.23
@author     ACJ
@version    v0.1
@brief      GraphicsPointItem Ŭ���� Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include "GraphicsDrawItem.h"
#include "gaia_global.h"

//! GraphicsPointItem Ŭ����
/*!
   ����Ʈ ������ Ŭ����
*/
class GAIA_EXPORT GraphicsPointItem : public GraphicsDrawItem
{
public:
	//! ������
	GraphicsPointItem( QGraphicsItem *parent );
	//! �Ҹ���
	~GraphicsPointItem();

	//! �׸��� ���� �Է�
	/*!
	   \param info �׸��� ����
	*/
	//void setDrawInfo( const OverlayPointInfo& info );
	//! ������ �Է�
	/*!
	   \param pos ��ġ
	   \param realPos ��ǥ��
	*/
	void setData( const QPointF& pos);
	QPointF data() const;

	GraphicsPointItem* clone() const;
	int type() const { return PointType; };
	void translate(double dx, double dy);

private:
	//! ����Ʈ
	/*!
	   \param painter ������
	   \param option �ɼ�
	   \param widget ȭ��
	*/
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	
private:
	QPointF _point;
};

