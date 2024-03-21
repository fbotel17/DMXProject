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
	void afficherScenesCheckbox();
	void createChannelLabelsAndLineEdits(int channelCount, int numCanal);
	void createFormForSelectedEquipements(const QList<QString>& selectedEquipements, const QString& selectedScene);



public slots:
	void on_actionCreer_une_sc_ne_triggered();
	void on_actionConfigurer_une_sc_ne_2_triggered();
	void on_actionAjouter_un_equipement_triggered();
	
	void on_pushButtonValider_clicked();
	void on_buttonEquip_clicked();
	void on_validateButtonEquip_clicked();
	void on_pushButton_clicked();



private:
    Ui::DMXProjectClass ui;
};
