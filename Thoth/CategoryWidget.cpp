/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include "LabelController.h"
#include "CategoryTreeModel.h"
#include "CategoryWidget.h"

class CategoryWidgetData
{
public:
	CategoryTreeModel* model = 0;
};

CategoryWidget::CategoryWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_d = new CategoryWidgetData;
	_d->model = new CategoryTreeModel(QVariantList({tr("Class")}), ui.catTree);
	ui.catTree->setModel(_d->model);

	layout()->setMargin(0);
	layout()->setSpacing(0);

	ui.title->setProperty("style", "title");

	connect(ui.close, &QToolButton::clicked, this, &CategoryWidget::hide);
	connect(ui.append, &QToolButton::clicked, this, &CategoryWidget::append);
	connect(ui.remove, &QToolButton::clicked, this, &CategoryWidget::remove);

	_d->model->addCategory(LabelController::instance()->categoryObject());
}

CategoryWidget::~CategoryWidget()
{
}

void CategoryWidget::append()
{
}

void CategoryWidget::remove()
{
}

void CategoryWidget::addCategory(const CategoryObject * cat)
{
	_d->model->addCategory(cat);
}
