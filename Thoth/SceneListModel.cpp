/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QtWidgets>

#include "SceneListModel.h"

static QMap<QString, QIcon> fsFileIconMap; // [Type : Icon]
const static QStringList gsTargetExtList = { "*.tif", "*.png", "*.jpg"};

SceneListModel::SceneListModel(QObject *parent)
	: QStandardItemModel(parent)
{
	setHeaderData(0, Qt::Horizontal, tr("Scenes"));
}

SceneListModel::~SceneListModel()
{
}

void SceneListModel::setPath(const QString & path)
{
	if (_path == path)
	{
		return;
	}

	_path = path;

	clear();
	QDir dir(_path);
	QFileInfoList files = dir.entryInfoList(gsTargetExtList, QDir::Files);

	setRowCount(0);

	for (const QFileInfo& fi : files)
	{
		QList<QStandardItem *> items; 
		QStandardItem* item = new QStandardItem(fi.baseName());
		items << item;
		QStandardItem* item2 = new QStandardItem(fi.absoluteFilePath());
		items << item2;
		appendRow(items);
	}
	//appendRow(items);
}
