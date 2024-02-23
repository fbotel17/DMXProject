#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DMXProject.h"

class DMXProject : public QMainWindow
{
    Q_OBJECT

public:
    DMXProject(QWidget *parent = nullptr);
    ~DMXProject();

private:
    Ui::DMXProjectClass ui;
};
