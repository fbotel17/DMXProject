#include "DMXProject.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidgetItem> // Ajoutez cette ligne
#include <QScrollArea>
#include <QStringList>



DMXProject::DMXProject(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testCodeDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "�chec de la connexion � la base de donn�es.";
		// G�rer les erreurs de connexion
		return;
	}


	
	// Afficher toutes les sc�nes existantes
	afficherScenes();
	afficherEquipements(); 
	afficherScenesCheckbox();
}

DMXProject::~DMXProject()
{}

void DMXProject::on_pushButtonValider_clicked()
{
	// R�cup�rer le contenu de lineEditNomScene
	QString nomScene = ui.lineEditNomScene->text();

	// Ex�cuter une requ�te SQL pour ins�rer les donn�es dans la table scene
	QSqlQuery query;
	query.prepare("INSERT INTO scene (nom) VALUES (:nom)");
	query.bindValue(":nom", nomScene);

	if (query.exec()) {
		qDebug() << "Sc�ne ins�r�e avec succ�s!";
		// Faire quelque chose apr�s l'insertion r�ussie, si n�cessaire
	}
	else {
		qDebug() << "Erreur lors de l'insertion de la sc�ne:" << query.lastError().text();
		// G�rer les erreurs d'insertion ici
	}

	ui.listWidget->clear();

	afficherScenes();

}

void DMXProject::on_actionCreer_une_sc_ne_triggered()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void DMXProject::on_actionConfigurer_une_sc_ne_2_triggered()
{
	afficherEquipements();
	afficherScenesCheckbox();
	ui.stackedWidget->setCurrentIndex(1);
}

void DMXProject::on_actionAjouter_un_equipement_triggered()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void DMXProject::afficherScenes()
{
	

	// Ex�cuter une requ�te pour r�cup�rer toutes les sc�nes existantes
	QSqlQuery query("SELECT * FROM scene");

	// Parcourir les r�sultats de la requ�te et ajouter chaque sc�ne � la liste
	while (query.next()) {
		int id = query.value(0).toInt();
		QString nom = query.value(1).toString();

		// Cr�er un nouvel �l�ment pour chaque sc�ne et l'ajouter � la liste
		QListWidgetItem *item = new QListWidgetItem(nom);
		ui.listWidget->addItem(item);
	}
}

void DMXProject::afficherEquipements()
{
	

	// Ex�cuter une requ�te pour r�cup�rer tous les �quipements existants
	QSqlQuery query("SELECT nom FROM equipement");

	// Cr�er un widget de d�filement pour contenir le layout des �quipements
	QScrollArea *scrollArea = new QScrollArea;
	QWidget *scrollWidget = new QWidget;
	QVBoxLayout *layoutEquipements = new QVBoxLayout(scrollWidget);
	scrollWidget->setLayout(layoutEquipements);
	scrollArea->setWidget(scrollWidget);
	scrollArea->setWidgetResizable(true);

	// R�cup�rer le layout pour les �quipements
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui.verticalLayoutEquipements->layout());
	if (!mainLayout) {
		qDebug() << "Erreur : le layout des �quipements est invalide.";
		return;
	}

	// Effacer le contenu existant du layout principal
	QLayoutItem *child;
	while ((child = mainLayout->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Parcourir les r�sultats de la requ�te et ajouter chaque �quipement en tant que case � cocher
	while (query.next()) {
		QString nomEquipement = query.value(0).toString();

		// Cr�er une case � cocher pour l'�quipement
		QCheckBox *checkBox = new QCheckBox(nomEquipement);
		// Ajouter la case � cocher au layout des �quipements
		layoutEquipements->addWidget(checkBox);
	}

	// Ajouter le widget de d�filement au layout principal
	mainLayout->addWidget(scrollArea);
}


// Modifiez votre slot on_buttonEquip_clicked pour appeler cette fonction
void DMXProject::on_buttonEquip_clicked() {
	QString nomEquipement = ui.nomEquipEdit->text();
	QString adresseEquipement = ui.adresseEquipEdit->text();
	int numCanal = ui.adresseEquipEdit->text().toInt(); // Convertir le texte en entier
	int nbCanaux = ui.nbCannauxEdit->text().toInt(); // Convertir le texte en entier
	createChannelLabelsAndLineEdits(nbCanaux, numCanal);


	QSqlQuery query;
	query.prepare("INSERT INTO equipement (nom, adresse, nbCanal) VALUES (:nom, :adresse, :nbCanal)");
	query.bindValue(":nom", nomEquipement);
	query.bindValue(":adresse", adresseEquipement);
	query.bindValue(":nbCanal", nbCanaux);

	if (query.exec()) {
		qDebug() << "�quipement ins�r� avec succ�s!";
	}
	else {
		qDebug() << "Erreur lors de l'insertion de l'�quipement:" << query.lastError().text();
	}
}





void DMXProject::afficherScenesCheckbox()
{
	

	// Ex�cuter une requ�te pour r�cup�rer tous les �quipements existants
	QSqlQuery query("SELECT nom FROM scene");

	// Cr�er un widget de d�filement pour contenir le layout des sc�nes
	QScrollArea *scrollArea = new QScrollArea;
	QWidget *scrollWidget = new QWidget;
	QVBoxLayout *layoutScenes = new QVBoxLayout(scrollWidget);
	scrollWidget->setLayout(layoutScenes);
	scrollArea->setWidget(scrollWidget);
	scrollArea->setWidgetResizable(true);

	// R�cup�rer le layout pour les sc�nes
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui.verticalLayoutScenes->layout());
	if (!mainLayout) {
		qDebug() << "Erreur : le layout des sc�nes est invalide.";
		return;
	}

	// Effacer le contenu existant du layout principal
	QLayoutItem *child;
	while ((child = mainLayout->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Garder une liste de toutes les cases � cocher
	QList<QCheckBox *> checkBoxes;

	// Parcourir les r�sultats de la requ�te et ajouter chaque sc�ne en tant que case � cocher
	while (query.next()) {
		QString nomScene = query.value(0).toString();

		// Cr�er une case � cocher pour la sc�ne
		QCheckBox *checkBox = new QCheckBox(nomScene);
		// Ajouter la case � cocher au layout des sc�nes
		layoutScenes->addWidget(checkBox);
		checkBoxes.append(checkBox); // Ajouter la case � cocher � la liste
	}

	// D�finir une largeur fixe pour le widget de d�filement
	scrollArea->setFixedWidth(200); // Vous pouvez ajuster cette valeur selon vos besoins

	// Ajouter le widget de d�filement au layout principal
	mainLayout->addWidget(scrollArea);

	// Connecter le signal toggled() de chaque case � cocher � une fonction de gestion
	for (QCheckBox *checkBox : checkBoxes) {
		connect(checkBox, &QCheckBox::toggled, [this, checkBoxes, checkBox](bool checked) {
			if (checked) {
				// D�s�lectionner toutes les autres cases � cocher sauf celle qui vient d'�tre coch�e
				for (QCheckBox *otherCheckBox : checkBoxes) {
					if (otherCheckBox != checkBox) {
						otherCheckBox->setChecked(false);
					}
				}
			}
		});
	}
}




void DMXProject::createChannelLabelsAndLineEdits(int channelCount, int numCanal) {
	// Supprimez les widgets pr�c�demment ajout�s s'il y en a
	ui.stackedWidget->setCurrentIndex(3);
	QLayoutItem *child;
	while ((child = ui.verticalLayout_17->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Affichez les QLabel et les QLineEdit pour chaque canal
	for (int i = 0; i < channelCount; ++i) {
		QLabel *label = new QLabel("Nom du canal " + QString::number(numCanal++) + ":");
		QLineEdit *lineEdit = new QLineEdit;
		ui.verticalLayout_17->addWidget(label);
		ui.verticalLayout_17->addWidget(lineEdit);
	}
}

void DMXProject::on_validateButtonEquip_clicked() {
	// R�cup�rer l'ID du dernier �quipement ins�r�
	QSqlQuery query;
	query.exec("SELECT LAST_INSERT_ID()");
	int idEquip = query.next() ? query.value(0).toInt() : -1;

	if (idEquip == -1) {
		qDebug() << "Erreur : impossible de r�cup�rer l'ID du dernier �quipement ins�r�.";
		return;
	}

	// R�cup�rer la valeur de numCanal
	int idNumCanal = ui.adresseEquipEdit->text().toInt();

	// Parcourir tous les QLineEdit et ins�rer les donn�es dans la table "champ"
	QLayoutItem *child;
	while ((child = ui.verticalLayout_17->takeAt(0)) != nullptr) {
		QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(child->widget());
		if (lineEdit) {
			QString nom = lineEdit->text();

			// Ins�rer les donn�es dans la table "champ"
			query.prepare("INSERT INTO champ (idEquip, idNumCanal, nom) VALUES (:idEquip, :idNumCanal, :nom)");
			query.bindValue(":idEquip", idEquip);
			query.bindValue(":idNumCanal", idNumCanal);
			query.bindValue(":nom", nom);

			if (!query.exec()) {
				qDebug() << "Erreur lors de l'insertion des donn�es dans la table 'champ' : " << query.lastError().text();
			}

			idNumCanal++; // Incr�menter idNumCanal pour le prochain enregistrement
		}
		delete child->widget();
		delete child;
	}
}


void DMXProject::on_pushButton_clicked()
{
	qDebug() << "Number of children in verticalLayoutEquipements:" << ui.verticalLayoutEquipements->count();

	// Find the widgetEquipements and widgetScenes widgets
	QWidget *widgetEquipements = ui.verticalLayoutEquipements->itemAt(0)->widget();
	QWidget *widgetScenes = ui.verticalLayoutScenes->itemAt(0)->widget();

	// R�cup�rer les �quipements s�lectionn�s
	QList<QCheckBox*> checkedEquipements = widgetEquipements->findChildren<QCheckBox*>();
	QList<QString> selectedEquipements;

	if (checkedEquipements.isEmpty())
	{
		qDebug() << "No QCheckBox objects found in widgetEquipements.";
	}
	else
	{
		qDebug() << "Found" << checkedEquipements.size() << "QCheckBox objects in widgetEquipements.";

		for (QCheckBox* checkBox : checkedEquipements)
		{
			if (checkBox->isChecked())
			{
				// R�cup�rer le nom de l'�quipement � partir du texte de la QCheckBox
				QString equipementName = checkBox->text();
				selectedEquipements.append(equipementName);
			}
		}
	}

	// R�cup�rer la sc�ne s�lectionn�e
	QList<QCheckBox*> checkedScenes = widgetScenes->findChildren<QCheckBox*>();
	QString selectedScene;

	if (checkedScenes.isEmpty())
	{
		qDebug() << "No QCheckBox objects found in widgetScenes.";
	}
	else
	{
		qDebug() << "Found" << checkedScenes.size() << "QCheckBox objects in widgetScenes.";

		for (QCheckBox* checkBox : checkedScenes)
		{
			if (checkBox->isChecked())
			{
				// R�cup�rer le nom de la sc�ne � partir du texte de la QCheckBox
				QString sceneName = checkBox->text();
				selectedScene = sceneName;
				break;
			}
		}
	}

	if (!selectedEquipements.isEmpty() && !selectedScene.isEmpty())
	{
		// Cr�er le formulaire dynamique
		createFormForSelectedEquipements(selectedEquipements, selectedScene);

		// Passer � la page d'index 4
		ui.stackedWidget->setCurrentIndex(4);
	}
	else
	{
		qDebug() << "No equipments or scene selected.";
	}
}






void DMXProject::createFormForSelectedEquipements(const QList<QString>& selectedEquipements, const QString& selectedScene)
{
	// Effacer le layout verticalLayout_18
	QLayoutItem* child;
	while ((child = ui.verticalLayout_18->takeAt(0)) != nullptr)
	{
		delete child->widget();
		delete child;
	}

	// R�cup�rer les informations sur les canaux pour chaque �quipement s�lectionn�
	QStringList equipementNames;
	for (const QString& equipementName : selectedEquipements)
	{
		equipementNames.append("'" + equipementName + "'");
	}
	QString joinedEquipementNames = equipementNames.join(",");

	QSqlQuery query("SELECT e.nom, c.idNumCanal, c.nom AS canalNom "
		"FROM equipement e "
		"JOIN champ c ON e.id = c.idEquip "
		"WHERE e.nom IN (" + joinedEquipementNames + ") "
		"ORDER BY e.nom, c.idNumCanal");

	QMap<QString, QList<QPair<int, QString>>> equipementCanaux;
	while (query.next())
	{
		QString equipementName = query.value(0).toString();
		int canalId = query.value(1).toInt();
		QString canalNom = query.value(2).toString();

		if (!equipementCanaux.contains(equipementName))
		{
			equipementCanaux[equipementName] = QList<QPair<int, QString>>();
		}

		equipementCanaux[equipementName].append(qMakePair(canalId, canalNom));
	}

	// Cr�er les QLabel et QLineEdit pour chaque canal des �quipements s�lectionn�s
	for (const QString& equipementName : selectedEquipements)
	{
		QList<QPair<int, QString>> canaux = equipementCanaux[equipementName];
		for (const QPair<int, QString>& canal : canaux)
		{
			QLabel* label = new QLabel(canal.second + " :");
			QLineEdit* lineEdit = new QLineEdit;

			ui.verticalLayout_18->addWidget(label);
			ui.verticalLayout_18->addWidget(lineEdit);
		}
	}
}
