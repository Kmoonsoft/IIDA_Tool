#pragma once

/*!
@file       MainWidget.h
@date       2021.09.01
@author     Lucky
@version    v0.1
@brief      MainWidget Header file
@details	Main GUI
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QWidget>
#include "ui_MainWidget.h"

/*!
@brief Main GUI class
*/
class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();

	void setCurrentScene(const QString& name, const QString& path);

private slots:
	void setCurrentPos(const QPointF& pos, const QPointF & lonlat);
	void doneDraw(class GraphicsDrawItem* item);

	void newLabel();
	void openLabel();
	void saveLabel();

	void showSettings();

	void na();


private:
	Ui::MainWidget ui;
	class MainWidgetData* _d;
};
