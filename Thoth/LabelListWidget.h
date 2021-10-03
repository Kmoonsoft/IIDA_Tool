#pragma once
/*!
@file       LabelListWidget.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Label List Widget Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QWidget>
#include "ui_LabelListWidget.h"

/*!
@brief LableListWidget class provides Lable List 
*/
class LabelListWidget : public QWidget
{
	Q_OBJECT

public:
	LabelListWidget(QWidget *parent = Q_NULLPTR);
	~LabelListWidget();

public slots:
	void setCurrentScene(const QString & name, const QString & path);

private slots:
	void search();
	void selectRows(const QList<int>& selectedRows);
	void changedSelection(const QItemSelection &selected, const QItemSelection &deselected);
	void selectAnnotations(const QList<int>& idList);

private:
	Ui::LabelListWidget ui;

	class LabelListWidgetData* _d;
};
