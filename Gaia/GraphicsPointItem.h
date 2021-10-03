#pragma once

/*!
@file       GraphicsPointItem.h
@date       2021.09.23
@author     ACJ
@version    v0.1
@brief      GraphicsPointItem 클래스 Header file
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

//! GraphicsPointItem 클래스
/*!
   포인트 아이템 클래스
*/
class GAIA_EXPORT GraphicsPointItem : public GraphicsDrawItem
{
public:
	//! 생성자
	GraphicsPointItem( QGraphicsItem *parent );
	//! 소멸자
	~GraphicsPointItem();

	//! 그리기 정보 입력
	/*!
	   \param info 그리기 정보
	*/
	//void setDrawInfo( const OverlayPointInfo& info );
	//! 데이터 입력
	/*!
	   \param pos 위치
	   \param realPos 좌표값
	*/
	void setData( const QPointF& pos);
	QPointF data() const;

	GraphicsPointItem* clone() const;
	int type() const { return PointType; };
	void translate(double dx, double dy);

private:
	//! 페인트
	/*!
	   \param painter 페인터
	   \param option 옵션
	   \param widget 화면
	*/
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	
private:
	QPointF _point;
};

