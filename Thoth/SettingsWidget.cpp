/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QFileDialog>

#include "Settings.h"
#include "LabelController.h"
#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.openObjectClassFile, &QToolButton::clicked, this, &SettingsWidget::setObjectClass);

	QString objClassPath = Settings::getFilePath(ObjectClassPath);
	ui.objClassPath->setText(objClassPath);

}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::setObjectClass()
{
	// Open Path
	QString path = QFileDialog::getOpenFileName(this, tr("Select Object class defines"), Settings::getFilePath(ObjectClassPath), tr("Json files(*.json))"));
	if (path.isEmpty()) return;

	LabelController* ctrl = LabelController::instance();
	bool ok = ctrl->loadCategories(path);

	if (!ok)
	{
		ctrl->addLog(tr("Fail to open a Object class defines file(json)"), 1);
		ctrl->addLog(path, 1);
	}
	else
	{
		Settings::setFilePath(ObjectClassPath, path);
		ui.objClassPath->setText(path);
	}

}
