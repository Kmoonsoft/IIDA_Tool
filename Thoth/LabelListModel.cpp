/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include "LabelObject.h"
#include "LabelController.h"
#include "LabelListModel.h"

LabelListModel::LabelListModel(QObject *parent)
	: QStandardItemModel(parent)
{
	const QStringList columns = tr("ID,Class,Size,Category ID").split(',');
	setColumnCount(columns.count());
	setHorizontalHeaderLabels(columns);

	LabelController* ctrl = LabelController::instance();
	connect(ctrl, &LabelController::annotationChanged, this, &LabelListModel::setAnnotation);
	connect(ctrl, &LabelController::labelReset, this, &LabelListModel::resetLabel);
	connect(ctrl, &LabelController::annotationRemoved, this, &LabelListModel::removeAnnotation);
}

LabelListModel::~LabelListModel()
{
}

//void LabelListModel::setLabelObject(const LabelObject * label)
//{
//	_label = const_cast<LabelObject*>(label);
//}

void LabelListModel::setFilter(const QString & imageFileName)
{
	setRowCount(0);

	LabelController* ctrl = LabelController::instance();
	QList<Annotation> list = ctrl->annotations(imageFileName);
	//QList<Annotation> list = _label->annotations(imageFileName);

	beginResetModel();
	for (const Annotation& anno : list)
	{
		addAnnotation(anno);
	}

	endResetModel();
}

// Return Hidden columns
QList<int> LabelListModel::hiddenColumns()
{
	static const QList<int> hiddens = { 3 };
	return hiddens;
}

int LabelListModel::annotationId(int row) const
{
	return index(row, 0).data(Qt::EditRole).toInt();
}

QList<int> LabelListModel::rows(const QList<int>& ids)
{
	QList<int> list;
	for (int row = 0; row < rowCount(); row++)
	{
		int id = annotationId(row);
		if (ids.contains(id)) list << row;
	}

	return list;
}

// Update or Append Annotation
void LabelListModel::setAnnotation(const Annotation & anno)
{
	bool found = false;
	LabelController* ctrl = LabelController::instance();
	for (int row = 0; row < rowCount(); row++)
	{
		int id = annotationId(row);
		if (anno.id == id)
		{
			found = true;
			QStandardItem* item1 = item(row, 1);
			Category cat = ctrl->category(anno.category);
			item1->setText(cat.name);
			QStandardItem* item2 = item(row, 2);
			item2->setText(anno.sizeText());
			QStandardItem* item3 = item(row, 2);
			item3->setData(anno.category);
		}
	}

	if (!found)
	{
		addAnnotation(anno);
	}
}

void LabelListModel::resetLabel()
{
	setRowCount(0);
}

void LabelListModel::removeAnnotation(const QList<int>& ids)
{
	for (int row = 0; row < rowCount(); row++)
	{
		int id = annotationId(row);
		if (ids.contains(id))
		{
			removeRows(row, 1);
		}
	}
}

// Add Item from Annotation(Private Method)
void LabelListModel::addAnnotation(const Annotation & anno)
{
	LabelController* ctrl = LabelController::instance();
	QList<QStandardItem *> items;
	QStandardItem* item = new QStandardItem(); // column 0
	item->setData(anno.id, Qt::EditRole);
	items << item;
	Category cat = ctrl->category(anno.category);
	items << new QStandardItem(cat.name); // column 1
	items << new QStandardItem(anno.sizeText()); // column 2
	items << new QStandardItem(QString::number(anno.category)); // column 3

	appendRow(items);
}

// Sort by Class/Catetory id, Created/ID, Category Name/Text
void LabelListModel::sortBy(int type)
{
	// 0:Class, 1:Created, 2:Name
	const QMap<int, int> sortMap = { {0, 3}, {1, 0}, {2, 1} };

	sort(sortMap.value(type), Qt::AscendingOrder);
	//LabelController::instance()->addLog("Sorting is NOT Implemented Yet!");
}

//QList<COCOAnnotation> list = _coco->annotation(imageFileName);
//for (const COCOAnnotation& anno : list)
//{
//	QList<QStandardItem *> items;
//	QStandardItem* item = new QStandardItem(QString::number(anno.id));
//	items << item;

//	COCOCategory cat = _coco->category(anno.category_id);
//	QStandardItem* item2 = new QStandardItem(cat.name);
//	items << item2;
//	QString size = QString("%1x%2").arg(anno.bbox.width()).arg(anno.bbox.height());
//	QStandardItem* item3 = new QStandardItem(size);
//	items << item3;
//	appendRow(items);
//}
