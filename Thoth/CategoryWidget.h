#pragma once

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QWidget>
#include "ui_CategoryWidget.h"

class CategoryWidget : public QWidget
{
	Q_OBJECT

public:
	CategoryWidget(QWidget *parent = Q_NULLPTR);
	~CategoryWidget();

	void append();
	void remove();

	void addCategory(const class CategoryObject* cat);


private:
	Ui::CategoryWidget ui;

	class CategoryWidgetData* _d;
};
