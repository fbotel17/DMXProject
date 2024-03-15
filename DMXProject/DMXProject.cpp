#include "DMXProject.h"
#include <QSqlDatabase>
#include <QtSql>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidgetItem> // Ajoutez cette ligne



DMXProject::DMXProject(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "�chec de la connexion � la base de donn�es.";
		// G�rer les erreurs de connexion
		return;
	}


	
	// Afficher toutes les sc�nes existantes
	afficherScenes();
	afficherEquipements(); // Assurez-vous que cette ligne est bien pr�sente
}

DMXProject::~DMXProject()
{}

void DMXProject::on_pushButtonValider_clicked()
{
	// R�cup�rer le contenu de lineEditNomScene
	QString nomScene = ui.lineEditNomScene->text();

	// Ex�cuter une requ�te SQL pour ins�rer les donn�es dans la table scene
	QSqlQuery query;
	query.prepare("INSERT INTO Scene (nom) VALUES (:nom)");
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
	ui.stackedWidget->setCurrentIndex(1);
}

void DMXProject::on_actionAjouter_un_equipement_triggered()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void DMXProject::afficherScenes()
{
	// Connexion � la base de donn�es
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "�chec de la connexion � la base de donn�es.";
		return;
	}

	// Ex�cuter une requ�te pour r�cup�rer toutes les sc�nes existantes
	QSqlQuery query("SELECT * FROM Scene");

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
	// Connexion � la base de donn�es
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "�chec de la connexion � la base de donn�es.";
		return;
	}

	// Ex�cuter une requ�te pour r�cup�rer tous les �quipements existants
	QSqlQuery query("SELECT nom FROM Equipement");

	// R�cup�rer le layout pour les �quipements
	QVBoxLayout *layoutEquipements = qobject_cast<QVBoxLayout*>(ui.verticalLayoutEquipements->layout());
	if (!layoutEquipements) {
		qDebug() << "Erreur : le layout des �quipements est invalide.";
		return;
	}

	// Effacer le contenu existant du layout
	QLayoutItem *child;
	while ((child = layoutEquipements->takeAt(0)) != nullptr) {
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
}

void DMXProject::on_buttonEquip_clicked()
{
	QString nomEquipement = ui.nomEquipEdit->text();
	QString adresseEquipement = ui.adresseEquipEdit->text();
	int nbCanaux = ui.nbCannauxEdit->text().toInt(); // Convertir le texte en entier

	QSqlQuery query;
	query.prepare("INSERT INTO Equipement (nom, adresse, nbCanal) VALUES (:nom, :adresse, :nbCanal)");
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


