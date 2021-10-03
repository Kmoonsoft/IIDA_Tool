#pragma once

/*!
@file       NotificationWidget.h
@date       2021.09.13
@author     Lucky
@version    v0.1
@brief      Overlay notification widget class header
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QFrame>

/*!
@brief Overlay Notification widget on the image scrren
*/
class NotificationWidget : public QFrame
{
	Q_OBJECT

public:
	NotificationWidget(QWidget *parent = Q_NULLPTR);
	~NotificationWidget();

	void setMesageColor(const QString& color);

public slots:
	void showNotification(const QString& message, int level);

private slots:
	void slidingHide();

private:

	class QLabel* _label;
	//class SlidingAnimation* _slide;
	int _unreadCnt = 0;
	QString _textColor = "#E16964";
};
