#pragma once
/*!
@file       SettingsWidget.h
@date       2021.10.01
@author     Lucky
@version    v0.1
@brief      SettingsWidget Header file
@details	Main GUI
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QWidget>
#include "ui_SettingsWidget.h"

/*!
@brief SettingsWidget class provides a GUI to handle Settings
*/
class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QWidget *parent = Q_NULLPTR);
	~SettingsWidget();

private slots:
	void setObjectClass();

private:
	Ui::SettingsWidget ui;
};
