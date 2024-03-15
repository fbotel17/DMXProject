#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DMXProject.h"

class DMXProject : public QMainWindow
{
    Q_OBJECT

public:
    DMXProject(QWidget *parent = nullptr);
    ~DMXProject();
	void afficherScenes();
	void afficherEquipements();


public slots:
	void on_actionCreer_une_sc_ne_triggered();
	void on_actionConfigurer_une_sc_ne_2_triggered();
	void on_pushButtonValider_clicked();
	


private:
    Ui::DMXProjectClass ui;
};
