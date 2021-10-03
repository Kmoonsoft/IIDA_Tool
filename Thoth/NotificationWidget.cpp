/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtDebug>
//#include "SlidingAnimation.h"
#include "NotificationWidget.h"

// 0 : info
// 1 : warning
// 2 : error

NotificationWidget::NotificationWidget(QWidget *parent)
	: QFrame(parent)
{
	//setFrameShape(QFrame::StyledPanel);
	//setFrameShadow(QFrame::Sunken);

	//ui.setupUi(this);
	QToolButton* closeBtn = new QToolButton(this);
	closeBtn->setIcon(QIcon(":/Thoth/Resources/icons/ic_clear_black_36dp.png"));
	//closeBtn->setMaximumSize(15, 15);
	closeBtn->setMinimumWidth(20);
	closeBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	
	connect(closeBtn, &QToolButton::released, this, &NotificationWidget::slidingHide);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setMargin(0);

	QLabel* icon = new QLabel(this);
	icon->setObjectName(QStringLiteral("label"));
	icon->setPixmap(QPixmap(QString::fromUtf8(":/Thoth/Resources/icons/ic_error_outline_black_36dp.png")));

	layout->addWidget(icon);
	
	_label = new QLabel(this);
	layout->addWidget(_label);

	QVBoxLayout *verticalLayout = new QVBoxLayout();
	verticalLayout->setMargin(0);
	verticalLayout->setSpacing(0);
	//verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
	
	QSpacerItem* vSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	verticalLayout->addWidget(closeBtn); 
	//verticalLayout->addItem(vSpacer);

	QSpacerItem* hSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

	layout->addItem(hSpacer);
	layout->addLayout(verticalLayout);

	setMinimumWidth(400);

	//_slide = new SlidingAnimation(this);
	//_slide->setOrientation(Qt::Vertical);
	//_slide->setSlidingAnimation(false);
	//_slide->setTransparencyAnimation(true);

	setProperty("style", "message");
}

NotificationWidget::~NotificationWidget()
{
}

void NotificationWidget::setMesageColor(const QString & color)
{
	_textColor = color;
}

void NotificationWidget::slidingHide()
{
	hide();
	_unreadCnt = 0;
	//_slide->sliding(false);
}

void NotificationWidget::showNotification(const QString & message, int level)
{
	if (level < 1) return;

	//static const QStringList colors = { "white", "#FAC05C", "#E16964", "#E16964", "white" };
	_label->setText(QString("<b><font color=%1>%2(%3)</font></b>").arg(_textColor).arg(message).arg(++_unreadCnt));
	//slidingHide();
	//if( !isVisible() ) _slide->sliding(true);

	raise();
	setFocus();

	show();
}
