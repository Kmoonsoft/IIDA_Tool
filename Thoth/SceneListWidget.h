#pragma once

/*!
@file       SceneListWidget.h
@date       2021.09.13
@author     Lucky
@version    v0.1
@brief      SceneList Display Widget Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/


#include <QWidget>
#include "ui_SceneListWidget.h"

/*!
@brief	Scene, Satellite Image, List Display widget
*/
class SceneListWidget : public QWidget
{
	Q_OBJECT

public:
	SceneListWidget(QWidget *parent = Q_NULLPTR);
	~SceneListWidget();

	QString currentFileName() const;

signals:
	void sceneChanged(const QString& name, const QString& path);

private slots:
	/*!
	@brief Select Path and display scene list
	*/
	void open();
	void changeScene(const QModelIndex &index);
	void prev();
	void next();

private:
	Ui::SceneListWidget ui;

	class SceneListWidgetData* _d; // Private data
};
