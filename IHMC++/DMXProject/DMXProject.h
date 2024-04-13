#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DMXProject.h"
#include "Scene.h"
#include "Equipement.h"


class DMXProject : public QMainWindow
{
	Q_OBJECT

public:
	DMXProject(QWidget* parent = nullptr);
	~DMXProject();

	void afficherScenesCheckbox();
	void createChannelLabelsAndLineEdits(int channelCount, int numCanal);
	void createFormForSelectedEquipements(const QList<QString>& selectedEquipements, const QString& selectedScene);



	void clearForm();
	void Gerer_un_equipement();
	


public slots:
	void on_actionCreer_une_sc_ne_triggered();
	void on_actionConfigurer_une_sc_ne_2_triggered();
	void on_actionAjouter_un_equipement_triggered();
	void on_actionSupprimer_un_equipement_triggered();

	void on_pushButtonValider_clicked();
	void on_buttonEquip_clicked();
	void on_validateButtonEquip_clicked();
	void on_pushButton_clicked();
	//void on_ValidateButtonCanal_clicked();
	void handleDeleteButtonClicked();
	void handleModifyButtonClicked(int idEquipement, const QString& nomEquipement, const QString& adresseEquipement, int nbCanalEquipement);

	//void updateUi(int);
	void saveSettings();

private:
	Ui::DMXProjectClass ui;
	QList<QPair<QString, QList<QPair<int, QString>>>> m_selectedEquipementsData;
	int m_currentEquipementIndex;
	QString m_selectedScene;
	QList<QLineEdit*> m_lineEdits;
	int numCanal;
	int m_idEquipementASupprimer = -1; // Initialis� � -1 pour indiquer qu'aucun �quipement n'est s�lectionn� initialement
	QList<QString> m_selectedEquipements;
	Scene* scene;
	Equipement* equipement;



};

