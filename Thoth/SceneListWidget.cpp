/*!
@file       SceneListWidget.cpp
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

#include <QFileDialog>

#include "LabelController.h"
#include "SceneListModel.h"
#include "Settings.h"
#include "SceneListWidget.h"

/*!
@brief Private SceneListWidget Data class
*/
class SceneListWidgetData
{
public:
	//QStandardItemModel* model = 0; // File List Model
	SceneListModel* model;
};

SceneListWidget::SceneListWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new SceneListWidgetData;
	_d->model = new SceneListModel(ui.listView);
	ui.listView->setModel(_d->model);

	LabelController* ctrl = LabelController::instance();

	// Button Interface Connections
	connect(ui.open, &QToolButton::clicked, this, &SceneListWidget::open);
	connect(ui.listView, &QListView::activated, this, &SceneListWidget::changeScene);
	//connect(ui.single, &QToolButton::toggled, ctrl, &LabelController::singleScene);
	connect(ctrl, &LabelController::prevSceneRequested, this, &SceneListWidget::prev);
	connect(ctrl, &LabelController::nextSceneRequested, this, &SceneListWidget::next);
	connect(this, &SceneListWidget::sceneChanged, ctrl, &LabelController::setCurrentScene);

	ui.titleLabel->setProperty("style", "title");

	ui.single->hide();
}

SceneListWidget::~SceneListWidget()
{
	delete _d;
}

QString SceneListWidget::currentFileName() const
{
	QModelIndex idx = ui.listView->currentIndex();
	QString title = idx.sibling(idx.row(), 0).data().toString();
	return title;
}

/*!
@brief Change Scene Item
*/
void SceneListWidget::changeScene(const QModelIndex & idx)
{
	// emit path
	int row = idx.row();
	QString title = idx.sibling(row, 0).data().toString();
	QString path = idx.sibling(row, 1).data().toString();
	emit sceneChanged(title, path);
}

/*!
@brief Change to the previous Scene
*/
void SceneListWidget::prev()
{
	QModelIndex curIdx = ui.listView->currentIndex();
	if (curIdx.row() <= 0) return;
	QModelIndex idx = curIdx.sibling(curIdx.row() - 1, 0);
	ui.listView->setCurrentIndex(idx);
	changeScene(idx);
}

/*!
@brief Chage to the next scene
*/
void SceneListWidget::next()
{
	QModelIndex curIdx = ui.listView->currentIndex();
	if (curIdx.row() >= _d->model->rowCount()-1 ) return;
	QModelIndex idx = curIdx.sibling(curIdx.row() + 1, 0);
	ui.listView->setCurrentIndex(idx);

	changeScene(idx);
}

// Get Scene List
void SceneListWidget::open()
{
	// Open Path
	QString path = QFileDialog::getExistingDirectory(this, tr("Select Scene Path"), Settings::getFilePath(ScenePath));
	if (path.isEmpty()) return;

	// Load Scene List
	_d->model->setPath(path);

	Settings::setFilePath(ScenePath, path);

	ui.titleLabel->setText(tr("Scene List(%1)").arg(_d->model->rowCount()));

	QString cocoPath;
	QDir dir(path);
	dir.cdUp();
	QFileInfoList files = dir.entryInfoList({ "*.json" }, QDir::Files);
	if (!files.isEmpty())
	{
		bool ok = LabelController::instance()->openCOCO(files.first().absoluteFilePath());

	}
}
