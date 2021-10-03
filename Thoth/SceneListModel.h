#pragma once

/*!
@file       SceneListModel.h
@date       2021.09.13
@author     Lucky
@version    v0.1
@brief      SceneList Item List Model Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20210913	Lucky		First release(v0.1)
*/

#include <QStandardItemModel>

/*!
@brief SceneList Item List Model
*/
class SceneListModel : public QStandardItemModel
{
	Q_OBJECT

public:
	SceneListModel(QObject *parent);
	~SceneListModel();

	void setPath(const QString& path);

private:
	QString _path;
};
