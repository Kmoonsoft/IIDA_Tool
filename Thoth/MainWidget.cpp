/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QtDebug>
#include <QFileDialog>

#include <GraphicsRectItem.h>
#include <GraphicsModel.h>

#include "UiControl.h"
#include "LabelController.h"
#include "Settings.h"
#include "SettingsWidget.h"
#include "MainWidget.h"

class MainWidgetData
{
public:
	LabelController* ctrl = 0;
	QButtonGroup* bGroup = 0;
	SettingsWidget* settings = 0;
	QString title;
};

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new MainWidgetData;
	_d->title = windowTitle();
	_d->ctrl = LabelController::instance();

	connect(ui.sceneList, &SceneListWidget::sceneChanged, this, &MainWidget::setCurrentScene);
	connect(_d->ctrl, &LabelController::sceneChanged, ui.labelList, &LabelListWidget::setCurrentScene);
	//connect(ui.quit, &QToolButton::clicked, this, &MainWidget::close);
	connect(ui.prev, &QToolButton::clicked, _d->ctrl, &LabelController::prevSceneRequested);
	connect(ui.next, &QToolButton::clicked, _d->ctrl, &LabelController::nextSceneRequested);
	connect(ui.settings, &QToolButton::clicked, this, &MainWidget::showSettings);

	_d->bGroup = new QButtonGroup(this);
	_d->bGroup->addButton(ui.select, 0);
	_d->bGroup->addButton(ui.rect, 1);
	_d->bGroup->addButton(ui.poly, 2);
	_d->bGroup->setExclusive(true);

	GraphicsModel* model = ui.map->model();

	//connect(model, &GraphicsModel::currentPosition, this, &MainWidget::setCurrentPos);

	//connect(ui.rect, &QToolButton::toggled, ui.map->mapView(), &MapView::drawRect);
	connect(ui.rect, &QToolButton::toggled, model, &GraphicsModel::drawRect);
	connect(ui.poly, &QToolButton::toggled, this, &MainWidget::na);
	connect(ui.merge, &QToolButton::clicked, this, &MainWidget::na);
	connect(ui.buffer, &QToolButton::clicked, this, &MainWidget::na);
	connect(ui.sync, &QToolButton::clicked, this, &MainWidget::na);
	connect(ui.osm, &QToolButton::clicked, this, &MainWidget::na);

	connect(model, &GraphicsModel::drawDone, this, &MainWidget::doneDraw);

	connect(ui.newLabel, &QPushButton::clicked, this, &MainWidget::newLabel);
	connect(ui.openLabel, &QPushButton::clicked, this, &MainWidget::openLabel);
	connect(ui.saveLabel, &QPushButton::clicked, this, &MainWidget::saveLabel);

	// Set Style
	ui.titleLabel->setProperty("style", "title");
	
	// Set Stylesheet
	QString styleSheet = Settings::getStyleSheet("Main.qss");
	setStyleSheet(styleSheet);

	layout()->setMargin(0);
	ui.mainFrame->layout()->setMargin(0);
	ui.mainFrame->layout()->setSpacing(0);
	ui.verticalLayout->setSpacing(0);

	ui.mainSplitter->setSizes({ 3000, 6000, 2000 });

	setWindowIcon(QIcon(":/Thoth/Resources/Thoth.png"));


}

MainWidget::~MainWidget()
{
}

// Change Current Scene Image
void MainWidget::setCurrentScene(const QString & name, const QString & path)
{
	ui.map->setCurrentImage(path);
	ui.titleLabel->setText(name);
}

void MainWidget::doneDraw(GraphicsDrawItem* item)
{
	ui.select->setChecked(true);
}

void MainWidget::newLabel()
{
	// If changed
	if (_d->ctrl->isChanged())
	{
		int ans = MSG_QUESTION(tr("Lables are edited. Do you want to chaned labels?"));
		if (ans == QMessageBox::Yes)
		{
			saveLabel();
			//// New file
			//if (_d->ctrl->isNew())
			//{
			//	QString path = QFileDialog::getSaveFileName(this, tr("Input Thot Lable file"), Settings::getFilePath(LastSavePath), tr("Thot Label files(*.thot))"));
			//	if (path.isEmpty()) return;
			//	bool ok = _d->ctrl->saveLabels(path);
			//	// Fail save
			//	if (!ok)
			//	{
			//		QString msg = tr("Fail to Save a lable file.");
			//		MSG_WARNING(msg + "/n" + path);
			//		_d->ctrl->addLog(msg, 1);
			//		_d->ctrl->addLog(path, 1);
			//		return;
			//	}
			//}
			//else
			//{
			//	bool ok = _d->ctrl->saveLabels();
			//	if (!ok)
			//	{
			//		QString msg = tr("Fail to Save the lable file.");
			//		MSG_WARNING(msg);
			//		_d->ctrl->addLog(msg, 1);
			//	}
			//}
		}
	}

	_d->ctrl->newLabels();

	// 추후 기능 이동
	ui.map->model()->clearSprites();
}

void MainWidget::openLabel()
{
	newLabel();

	QString path = QFileDialog::getOpenFileName(this, tr("Open IIDA Lable file"), Settings::getFilePath(LastSavePath), tr("IIDA Label files(*.iida))"));
	if (path.isEmpty()) return;
	bool ok = _d->ctrl->openLabels(path);

	// Fail open
	if (!ok)
	{
		QString msg = tr("Fail to open the lable file.");
		MSG_WARNING(msg + "/n" + path);
		_d->ctrl->addLog(msg, 1);
		_d->ctrl->addLog(path, 1);
		return;
	}

	Settings::setFilePath(LastSavePath, path);
}

void MainWidget::saveLabel()
{
	// If unchanged
	if (!_d->ctrl->isChanged()) return;
	
	bool ok = false;
	// New file
	if (_d->ctrl->isNew())
	{
		QString path = QFileDialog::getSaveFileName(this, tr("Input IIDA Lable file"), Settings::getFilePath(LastSavePath), tr("IIDA Label files(*.iida))"));
		if (path.isEmpty()) return;
		ok = _d->ctrl->saveLabels(path);

		// Fail save
		if (!ok)
		{
			QString msg = tr("Fail to Save a lable file.");
			MSG_WARNING(msg + "/n" + path);
			_d->ctrl->addLog(msg, 1);
			_d->ctrl->addLog(path, 1);
			return;
		}

		Settings::setFilePath(LastSavePath, path);
	}
	else
	{
		ok = _d->ctrl->saveLabels();
		if (!ok)
		{
			QString msg = tr("Fail to Save the lable file.");
			MSG_WARNING(msg);
			_d->ctrl->addLog(msg, 1);
			return;
		}
	}

	if(ok)
	{
		_d->ctrl->addLog(tr("Successfully saved!"));
	}

	// Set Unchanged mark
	setWindowTitle(_d->title);
}

void MainWidget::showSettings()
{
	if (_d->settings == 0)
	{
		_d->settings = new SettingsWidget(this);
		_d->settings->setWindowFlags(Qt::Tool);
		//_d->settings->resize(200, 300);
	}

	_d->settings->show();
}

// NOT Available Message
void MainWidget::na()
{
	QToolButton* o = dynamic_cast<QToolButton*>(sender());
	if (o)
	{
		QString msg = o->text() + tr(" Not implemented yet!");
		_d->ctrl->addLog(msg, 1);
	}
}

// Display Current mouse pos and geo pos
void MainWidget::setCurrentPos(const QPointF & pos, const QPointF & lonlat)
{
	QString str = QString::asprintf("x:%5d, y:%5d, Lon:%0.6f, Lat:%0.6f", (int)pos.x(), (int)pos.y(), lonlat.x(), lonlat.y());
	ui.currentPos->setText(str);
}

