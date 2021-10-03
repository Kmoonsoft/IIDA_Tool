/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QFileInfo>
#include "LabelController.h"
#include "LabelObject.h"
#include "CocoJsonObject.h"
#include "LabelListModel.h"
#include "LabelListWidget.h"

class LabelListWidgetData
{
public:
	LabelListModel* model;
};

LabelListWidget::LabelListWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new LabelListWidgetData;
	_d->model = new LabelListModel(ui.labelList);
	ui.labelList->setModel(_d->model);
	ui.labelList->verticalHeader()->hide();

	LabelController* ctrl = LabelController::instance();
	ui.sortCombo->addItems(tr("by Class,by Created,by Name").split(','));
	connect(ui.sortCombo, qOverload<int>(&QComboBox::currentIndexChanged), _d->model, qOverload<int>(&LabelListModel::sortBy));
	connect(ctrl, &LabelController::annotationSelected, this, &LabelListWidget::selectAnnotations);
	connect(ui.labelList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LabelListWidget::changedSelection);

	ui.labelList->setItemDelegateForColumn(0, 0);
	ui.labelList->setItemDelegateForColumn(2, 0);

	QList<int> hiddens = _d->model->hiddenColumns();
	for (int col : hiddens)
	{
		ui.labelList->hideColumn(col);
	}
}

LabelListWidget::~LabelListWidget()
{
	delete _d;
}

void LabelListWidget::selectRows(const QList<int>& selectedRows)
{
	QSignalBlocker block(ui.labelList);
	ui.labelList->clearSelection();

	for (int row : selectedRows)
	{
		ui.labelList->selectRow(row);
	}
}

void LabelListWidget::changedSelection(const QItemSelection & selected, const QItemSelection & deselected)
{
	LabelController* ctrl = LabelController::instance();
	disconnect(ctrl, &LabelController::annotationSelected, this, &LabelListWidget::selectAnnotations);
	
	QList<int> ids;
	QModelIndexList	indexes = ui.labelList->selectionModel()->selectedRows();
	for (const QModelIndex& idx : indexes)
	{
		int id = _d->model->annotationId(idx.row());
		ids << id;
	}

	emit ctrl->annotationSelected(ids);
	connect(ctrl, &LabelController::annotationSelected, this, &LabelListWidget::selectAnnotations);
}

// Select Annotations by id
void LabelListWidget::selectAnnotations(const QList<int>& idList)
{
	QList<int> selectedRows = _d->model->rows(idList);
	selectRows(selectedRows);
}

void LabelListWidget::search()
{
	QString keywork = ui.serchText->text();
}

void LabelListWidget::setCurrentScene(const QString & name, const QString & path)
{
	QString fileName = QFileInfo(path).fileName();
	_d->model->setFilter(fileName);

	ui.labelList->resizeColumnsToContents();
}
