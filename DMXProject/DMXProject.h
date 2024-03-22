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

	void insertChannelData(int idScene, QList<QPair<int, int>> channelData);
	int getEquipmentId(const QString &equipmentName);
	int getSceneId(const QString &sceneName);
	int getEquipmentCanalNumber(const QString &equipmentName, int canalNumber);
	void createFormForCurrentEquipement();
	void clearForm();


public slots:
	void on_actionCreer_une_sc_ne_triggered();
	void on_actionConfigurer_une_sc_ne_2_triggered();
	void on_actionAjouter_un_equipement_triggered();
	void on_actionSupprimer_un_equipement_triggered();
	
	void on_pushButtonValider_clicked();
	void on_buttonEquip_clicked();
	void on_validateButtonEquip_clicked();
	void on_pushButton_clicked();
	void on_ValidateButtonCanal_clicked();



private:
    Ui::DMXProjectClass ui;
	QList<QPair<QString, QList<QPair<int, QString>>>> m_selectedEquipementsData;
	int m_currentEquipementIndex;
	QString m_selectedScene;
	QList<QLineEdit*> m_lineEdits;
};
