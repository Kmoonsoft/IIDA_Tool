#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Thot.h"

class Thot : public QMainWindow
{
    Q_OBJECT

public:
    Thot(QWidget *parent = Q_NULLPTR);

private:
    Ui::ThotClass ui;
};
