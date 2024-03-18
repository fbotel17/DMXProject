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
		qDebug() << "Échec de la connexion à la base de données.";
		// Gérer les erreurs de connexion
		return;
	}


	
	// Afficher toutes les scènes existantes
	afficherScenes();
	afficherEquipements(); 
	afficherScenesCheckbox();
}

DMXProject::~DMXProject()
{}

void DMXProject::on_pushButtonValider_clicked()
{
	// Récupérer le contenu de lineEditNomScene
	QString nomScene = ui.lineEditNomScene->text();

	// Exécuter une requête SQL pour insérer les données dans la table scene
	QSqlQuery query;
	query.prepare("INSERT INTO Scene (nom) VALUES (:nom)");
	query.bindValue(":nom", nomScene);

	if (query.exec()) {
		qDebug() << "Scène insérée avec succès!";
		// Faire quelque chose après l'insertion réussie, si nécessaire
	}
	else {
		qDebug() << "Erreur lors de l'insertion de la scène:" << query.lastError().text();
		// Gérer les erreurs d'insertion ici
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
	// Connexion à la base de données
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "Échec de la connexion à la base de données.";
		return;
	}

	// Exécuter une requête pour récupérer toutes les scènes existantes
	QSqlQuery query("SELECT * FROM Scene");

	// Parcourir les résultats de la requête et ajouter chaque scène à la liste
	while (query.next()) {
		int id = query.value(0).toInt();
		QString nom = query.value(1).toString();

		// Créer un nouvel élément pour chaque scène et l'ajouter à la liste
		QListWidgetItem *item = new QListWidgetItem(nom);
		ui.listWidget->addItem(item);
	}
}

void DMXProject::afficherEquipements()
{
	// Connexion à la base de données
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "Échec de la connexion à la base de données.";
		return;
	}

	// Exécuter une requête pour récupérer tous les équipements existants
	QSqlQuery query("SELECT nom FROM Equipement");

	// Créer un widget de défilement pour contenir le layout des équipements
	QScrollArea *scrollArea = new QScrollArea;
	QWidget *scrollWidget = new QWidget;
	QVBoxLayout *layoutEquipements = new QVBoxLayout(scrollWidget);
	scrollWidget->setLayout(layoutEquipements);
	scrollArea->setWidget(scrollWidget);
	scrollArea->setWidgetResizable(true);

	// Récupérer le layout pour les équipements
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui.verticalLayoutEquipements->layout());
	if (!mainLayout) {
		qDebug() << "Erreur : le layout des équipements est invalide.";
		return;
	}

	// Effacer le contenu existant du layout principal
	QLayoutItem *child;
	while ((child = mainLayout->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Parcourir les résultats de la requête et ajouter chaque équipement en tant que case à cocher
	while (query.next()) {
		QString nomEquipement = query.value(0).toString();

		// Créer une case à cocher pour l'équipement
		QCheckBox *checkBox = new QCheckBox(nomEquipement);
		// Ajouter la case à cocher au layout des équipements
		layoutEquipements->addWidget(checkBox);
	}

	// Ajouter le widget de défilement au layout principal
	mainLayout->addWidget(scrollArea);
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
		qDebug() << "Équipement inséré avec succès!";
	}
	else {
		qDebug() << "Erreur lors de l'insertion de l'équipement:" << query.lastError().text();
	}
}




void DMXProject::afficherScenesCheckbox()
{
	// Connexion à la base de données
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "Échec de la connexion à la base de données.";
		return;
	}

	// Exécuter une requête pour récupérer tous les équipements existants
	QSqlQuery query("SELECT nom FROM Scene");

	// Créer un widget de défilement pour contenir le layout des scènes
	QScrollArea *scrollArea = new QScrollArea;
	QWidget *scrollWidget = new QWidget;
	QVBoxLayout *layoutScenes = new QVBoxLayout(scrollWidget);
	scrollWidget->setLayout(layoutScenes);
	scrollArea->setWidget(scrollWidget);
	scrollArea->setWidgetResizable(true);

	// Récupérer le layout pour les scènes
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui.verticalLayoutScenes->layout());
	if (!mainLayout) {
		qDebug() << "Erreur : le layout des scènes est invalide.";
		return;
	}

	// Effacer le contenu existant du layout principal
	QLayoutItem *child;
	while ((child = mainLayout->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Garder une liste de toutes les cases à cocher
	QList<QCheckBox *> checkBoxes;

	// Parcourir les résultats de la requête et ajouter chaque scène en tant que case à cocher
	while (query.next()) {
		QString nomScene = query.value(0).toString();

		// Créer une case à cocher pour la scène
		QCheckBox *checkBox = new QCheckBox(nomScene);
		// Ajouter la case à cocher au layout des scènes
		layoutScenes->addWidget(checkBox);
		checkBoxes.append(checkBox); // Ajouter la case à cocher à la liste
	}

	// Définir une largeur fixe pour le widget de défilement
	scrollArea->setFixedWidth(200); // Vous pouvez ajuster cette valeur selon vos besoins

	// Ajouter le widget de défilement au layout principal
	mainLayout->addWidget(scrollArea);

	// Connecter le signal toggled() de chaque case à cocher à une fonction de gestion
	for (QCheckBox *checkBox : checkBoxes) {
		connect(checkBox, &QCheckBox::toggled, [this, checkBoxes, checkBox](bool checked) {
			if (checked) {
				// Désélectionner toutes les autres cases à cocher sauf celle qui vient d'être cochée
				for (QCheckBox *otherCheckBox : checkBoxes) {
					if (otherCheckBox != checkBox) {
						otherCheckBox->setChecked(false);
					}
				}
			}
		});
	}
}
