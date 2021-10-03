#pragma once

/*!
@file       SceneListModel.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Label List Model Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QStandardItemModel>

struct Annotation;

class LabelListModel : public QStandardItemModel
{
	Q_OBJECT

public:
	LabelListModel(QObject *parent);
	~LabelListModel();

	//void setLabelObject(const class LabelObject* label);
	void setFilter(const QString& imageFileName);
	static QList<int> hiddenColumns();

	int annotationId(int row) const;
	QList<int> rows(const QList<int>& ids);

signals:
	//void selected(const QList<int>& selectedRows);

public slots:
	void sortBy(int type); // 0:Class, 1:Created, 2:Name
	void setAnnotation(const Annotation& anno);
	void resetLabel();
	void removeAnnotation(const QList<int>& ids);

private:
	void addAnnotation(const Annotation& anno);
};
