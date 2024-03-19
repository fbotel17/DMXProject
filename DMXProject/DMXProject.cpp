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
	db.setDatabaseName("testCodeDMX");
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
	query.prepare("INSERT INTO scene (nom) VALUES (:nom)");
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
	

	// Exécuter une requête pour récupérer toutes les scènes existantes
	QSqlQuery query("SELECT * FROM scene");

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
	

	// Exécuter une requête pour récupérer tous les équipements existants
	QSqlQuery query("SELECT nom FROM equipement");

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
		qDebug() << "Équipement inséré avec succès!";
	}
	else {
		qDebug() << "Erreur lors de l'insertion de l'équipement:" << query.lastError().text();
	}
}





void DMXProject::afficherScenesCheckbox()
{
	

	// Exécuter une requête pour récupérer tous les équipements existants
	QSqlQuery query("SELECT nom FROM scene");

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




void DMXProject::createChannelLabelsAndLineEdits(int channelCount, int numCanal) {
	// Supprimez les widgets précédemment ajoutés s'il y en a
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
	// Récupérer l'ID du dernier équipement inséré
	QSqlQuery query;
	query.exec("SELECT LAST_INSERT_ID()");
	int idEquip = query.next() ? query.value(0).toInt() : -1;

	if (idEquip == -1) {
		qDebug() << "Erreur : impossible de récupérer l'ID du dernier équipement inséré.";
		return;
	}

	// Récupérer la valeur de numCanal
	int idNumCanal = ui.adresseEquipEdit->text().toInt();

	// Parcourir tous les QLineEdit et insérer les données dans la table "champ"
	QLayoutItem *child;
	while ((child = ui.verticalLayout_17->takeAt(0)) != nullptr) {
		QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(child->widget());
		if (lineEdit) {
			QString nom = lineEdit->text();

			// Insérer les données dans la table "champ"
			query.prepare("INSERT INTO champ (idEquip, idNumCanal, nom) VALUES (:idEquip, :idNumCanal, :nom)");
			query.bindValue(":idEquip", idEquip);
			query.bindValue(":idNumCanal", idNumCanal);
			query.bindValue(":nom", nom);

			if (!query.exec()) {
				qDebug() << "Erreur lors de l'insertion des données dans la table 'champ' : " << query.lastError().text();
			}

			idNumCanal++; // Incrémenter idNumCanal pour le prochain enregistrement
		}
		delete child->widget();
		delete child;
	}
}
