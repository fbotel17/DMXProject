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
#include <QListWidgetItem>
#include <QScrollArea>
#include <QStringList>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QWizard>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSqlQuery>
#include <QPixmap>


DMXProject::DMXProject(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);

	// Connexion � la base de donn�es MySQL
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.64.213");
	db.setDatabaseName("testCodeDMX");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "�chec de la connexion � la base de donn�es.";
		return;
	}

	consoleMaterielle = new ConsoleMaterielle(ui.verticalSlider, this);

	// Connexion au serveur TCP
	QTcpSocket* socket = new QTcpSocket(this);
	socket->connectToHost("192.168.64.170", 12345);

	// Connexion � l'Arduino
	consoleController = new ConsoleController(this);
	if (!consoleController->connectToArduino("COM8")) {
		qDebug() << "�chec de la connexion � l'Arduino.";
	}

	// Connecter les signaux de ConsoleController aux slots de DMXProject
	connect(consoleController, &ConsoleController::sceneAddRequested, this, &DMXProject::on_actionCreer_une_sc_ne_triggered);
	connect(consoleController, &ConsoleController::sceneEditRequested, this, &DMXProject::on_actionConfigurer_une_sc_ne_2_triggered);
	connect(consoleController, &ConsoleController::sceneDeleteRequested, this, &DMXProject::on_actionSupprimer_un_equipement_triggered);


	connect(consoleController, &ConsoleController::sendSceneNamesRequested, this, &DMXProject::sendSceneNamesToArduino);

	connect(ui.ValidSceneEquipButton, &QPushButton::clicked, this, &DMXProject::onValidSceneEquipButtonClicked);
	connect(ui.ValidSceneEquipButton, &QPushButton::clicked, this, &DMXProject::onValidSceneEquipButtonClickedArduino);
	connect(ui.validateCanalButton, &QPushButton::clicked, this, &DMXProject::onValidateCanalButtonClicked);
	connect(ui.verticalSlider, &QSlider::valueChanged, this, &DMXProject::updateChampSliderValue);

	connect(consoleMaterielle, &ConsoleMaterielle::confirmButtonPressed, this, &DMXProject::onValidateButtonPressed);

	connect(ui.verticalSlider, &QSlider::valueChanged, this, &DMXProject::onSliderValueChanged);

	connect(consoleMaterielle, &ConsoleMaterielle::previousChannel, this, &DMXProject::selectPreviousScene);
	connect(consoleMaterielle, &ConsoleMaterielle::nextChannel, this, &DMXProject::selectNextScene);
	connect(consoleMaterielle, &ConsoleMaterielle::validateButtonPressed, this, &DMXProject::validateSelection);


	// Initialiser l'affichage du label avec la valeur actuelle du slider
	onSliderValueChanged(ui.verticalSlider->value());


	// Afficher toutes les sc�nes existantes
	Scene scene;
	QList<Scene> scenes = scene.getAllScenes();
	foreach(const Scene & s, scenes) {
		ui.listWidget->addItem(s.getNom());
	}

	// Afficher les �quipements
	equipement->afficherEquipements(ui.verticalLayoutEquipements);

	afficherScenesCheckbox();
	Gerer_un_equipement();
	fillSceneComboBox2();
	fillEquipComboBox();
	gererScenes(); 
	connect(ui.testSceneButton, &QPushButton::clicked, this, &DMXProject::testScene);

	// Envoyer les noms des sc�nes � l'Arduino
	QStringList sceneNames;
	foreach(const Scene & s, scenes) {
		sceneNames.append(s.getNom());
	}
	consoleController->sendSceneNames(sceneNames);

	int nouvelleLargeur = 400; // Par exemple, vous pouvez d�finir la largeur souhait�e
	int nouvelleHauteur = 300; // Par exemple, vous pouvez d�finir la hauteur souhait�e

	// Ins�rer une image dans le label_10
	QString path = QDir::currentPath() + "/image/page.png";
	QPixmap image(path);

	// Redimensionner l'image avec les nouvelles dimensions
	image = image.scaled(nouvelleLargeur, nouvelleHauteur, Qt::KeepAspectRatio); // Redimensionner l'image avec les dimensions sp�cifi�es

	// Redimensionner le QLabel pour qu'il puisse afficher l'image enti�re
	ui.label_10->resize(image.size());

	// Afficher l'image dans le QLabel
	ui.label_10->setPixmap(image);

	// Mettre � jour le layout si n�cessaire
	ui.label_10->updateGeometry();
}

DMXProject::~DMXProject()
{
}

void DMXProject::sendData(const QByteArray& data)
{
	quint16 blockSize = data.size();
	out << blockSize << data;
	tcpSocket->flush(); // Appeler flush() sur l'objet QTcpSocket

	QByteArray response;
	in >> response;

	// Traiter la r�ponse du serveur
}

void DMXProject::sendSceneNamesToArduino(const QStringList& scenes)
{
	if (!consoleController->isConnected()) {
		qDebug() << "Not connected to Arduino";
		return;
	}

	foreach(const QString & scene, scenes) {
		QByteArray data = scene.toUtf8() + "\n";
		qDebug() << "Sending scene:" << scene;
		consoleController->sendData(data);
		// Ajout d'un d�lai pour s'assurer que chaque ligne est bien envoy�e
	}
}

void DMXProject::on_pushButtonValider_clicked()
{
	// R�cup�rer le contenu de lineEditNomScene
	QString nomScene = ui.lineEditNomScene->text();

	// Ex�cuter une requ�te SQL pour ins�rer les donn�es dans la table scene
	scene->insertScene(nomScene);

	ui.listWidget->clear();

	scene->afficherScenes(ui.listWidget);


}

void DMXProject::on_actionCreer_une_sc_ne_triggered()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void DMXProject::on_actionConfigurer_une_sc_ne_2_triggered()
{
	equipement->afficherEquipements(ui.verticalLayoutEquipements);
	afficherScenesCheckbox();
	ui.stackedWidget->setCurrentIndex(1);
}

void DMXProject::on_actionAjouter_un_equipement_triggered()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void DMXProject::on_actionSupprimer_un_equipement_triggered()
{
	Gerer_un_equipement();
	ui.stackedWidget->setCurrentIndex(5);
}

void DMXProject::on_actionTester_une_scene_triggered()
{
	fillSceneComboBox();
	ui.stackedWidget->setCurrentIndex(6);
}

void DMXProject::on_actionArduino_triggered()
{
	ui.stackedWidget->setCurrentIndex(8);
}

void DMXProject::on_actionGerer_une_scene_triggered()
{
	gererScenes();
	ui.stackedWidget->setCurrentIndex(9);
}


// Modifiez votre slot on_buttonEquip_clicked pour appeler cette fonction
void DMXProject::on_buttonEquip_clicked() {
	QString nomEquipement = ui.nomEquipEdit->text();
	QString adresseEquipement = ui.adresseEquipEdit->text();
	int startChannelAddress = ui.adresseEquipEdit->text().toInt(); // Convertir le texte en entier
	int nbCanaux = ui.nbCannauxEdit->text().toInt(); // Convertir le texte en entier
	createChannelLabelsAndLineEdits(nbCanaux, startChannelAddress);

	equipement->insertEquipement(nomEquipement, adresseEquipement, nbCanaux);
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


void DMXProject::createChannelLabelsAndLineEdits(int channelCount, int startChannelAddress) {
	// Supprimez les widgets pr�c�demment ajout�s s'il y en a
	ui.stackedWidget->setCurrentIndex(3);
	QLayoutItem* child;
	while ((child = ui.verticalLayout_17->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Affichez les QLabel et les QLineEdit pour chaque canal
	for (int i = 0; i < channelCount; ++i) {
		QLabel* label = new QLabel("Nom du canal " + QString::number(startChannelAddress) + ":");
		QLineEdit* lineEdit = new QLineEdit;
		ui.verticalLayout_17->addWidget(label);
		ui.verticalLayout_17->addWidget(lineEdit);
		startChannelAddress++; // Incr�menter l'adresse de d�part pour le prochain �quipement
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
			champ->insertChamp(idEquip, idNumCanal, nom);
			

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
				m_selectedScene = sceneName;
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
	clearForm();

	// R�cup�rer l'ID de la sc�ne s�lectionn�e
	QSqlQuery sceneQuery;
	sceneQuery.prepare("SELECT id FROM scene WHERE nom = :nom");
	sceneQuery.bindValue(":nom", selectedScene);
	if (!sceneQuery.exec() || !sceneQuery.next()) {
		qDebug() << "Erreur lors de l'ex�cution de la requ�te sceneQuery:" << sceneQuery.lastError().text();
		ui.stackedWidget->setCurrentIndex(1); // Rediriger en cas d'erreur
		return;
	}
	int idScene = sceneQuery.value(0).toInt();

	// V�rifier si des canaux existent d�j� pour cette sc�ne
	QSqlQuery checkCanauxQuery;
	checkCanauxQuery.prepare("SELECT COUNT(*) FROM canaux WHERE idScene = :idScene");
	checkCanauxQuery.bindValue(":idScene", idScene);
	if (!checkCanauxQuery.exec() || !checkCanauxQuery.next()) {
		qDebug() << "Erreur lors de l'ex�cution de la requ�te checkCanauxQuery:" << checkCanauxQuery.lastError().text();
		ui.stackedWidget->setCurrentIndex(1); // Rediriger en cas d'erreur
		return;
	}
	int countCanaux = checkCanauxQuery.value(0).toInt();

	// Si des canaux existent d�j�, demander confirmation � l'utilisateur
	if (countCanaux > 0) {
		QMessageBox msgBox;
		msgBox.setText("La sc�ne s�lectionn�e poss�de d�j� une configuration. Voulez-vous la supprimer ?");
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Ok) {
			// Supprimer les canaux existants
			QSqlQuery deleteQuery;
			deleteQuery.prepare("DELETE FROM canaux WHERE idScene = :idScene");
			deleteQuery.bindValue(":idScene", idScene);
			if (!deleteQuery.exec()) {
				qDebug() << "Erreur lors de l'ex�cution de la requ�te deleteQuery:" << deleteQuery.lastError().text();
				ui.stackedWidget->setCurrentIndex(1); // Rediriger en cas d'erreur
				return;
			}
		}
		else {
			// Annuler l'op�ration
			ui.stackedWidget->setCurrentIndex(1); // Rediriger apr�s annulation
			return;
		}
	}

	// Cr�er un QWizard pour configurer les canaux de chaque �quipement dans la sc�ne
	QWizard* wizard = new QWizard(this);

	// Cr�er une page pour chaque �quipement s�lectionn�
	for (const QString& equipementName : selectedEquipements) {
		QWizardPage* page = new QWizardPage(this);
		QVBoxLayout* layout = new QVBoxLayout(page);
		layout->addWidget(new QLabel(QString("Configurer les canaux pour %1 :").arg(equipementName), page));

		// R�cup�rer les informations sur les canaux pour cet �quipement
		QSqlQuery query(QString("SELECT idNumCanal, nom FROM champ WHERE idEquip = (SELECT id FROM equipement WHERE nom = '%1') ORDER BY idNumCanal").arg(equipementName));

		// Ajouter des widgets pour chaque canal de cet �quipement
		QList<QPair<int, int>> channelData; // Ajouter une liste de paires pour stocker les valeurs des QSpinBox
		while (query.next()) {
			QString canalName = query.value(1).toString();
			int canalNumber = query.value(0).toInt();
			QLabel* label = new QLabel(QString("%1 :").arg(canalName), page);
			QSpinBox* spinBox = new QSpinBox(page);
			spinBox->setObjectName(QString("spinBox_%1").arg(canalNumber));
			spinBox->setMaximum(255); // D�finir la valeur maximale du QSpinBox � 255
			layout->addWidget(label);
			layout->addWidget(spinBox);

			// Ajouter la valeur du QSpinBox � la liste channelData
			channelData.append(qMakePair(canalNumber, spinBox->value()));
		}

		// Ajouter la liste channelData � la page du QWizard
		page->setProperty("channelData", QVariant::fromValue(channelData));

		wizard->addPage(page);
	}

	// Connecter le signal accepted() du QWizard � un slot pour enregistrer les param�tres de canal dans la base de donn�es
	connect(wizard, &QWizard::accepted, this, &DMXProject::saveSettings);

	// Connecter les signaux finished et rejected pour g�rer la redirection
	connect(wizard, &QWizard::rejected, [this, wizard]() {
		ui.stackedWidget->setCurrentIndex(1);
		wizard->deleteLater();
		});
	connect(wizard, &QWizard::finished, [this, wizard]() {
		ui.stackedWidget->setCurrentIndex(1);
		wizard->deleteLater();
		});

	// Afficher le QWizard
	wizard->show();
}





void DMXProject::clearForm()
{
	// Effacer le layout existant
	QLayoutItem *child;
	while ((child = ui.verticalLayout_17->takeAt(0)) != nullptr)
	{
		delete child->widget();
		delete child;
	}

	// Vider la liste des pointeurs de QLineEdit
	m_lineEdits.clear();
}


void DMXProject::Gerer_un_equipement()
{
	QLayoutItem* child;

	// Supprimer tous les widgets existants dans le layout
	while ((child = ui.verticalLayout_3->takeAt(0)) != nullptr)
	{
		delete child->widget();
		delete child;
	}

	// Vider la liste des pointeurs de QLineEdit
	m_lineEdits.clear();

	// Cr�er un QLabel avec du texte en grand au milieu
	QLabel* gererEquipementLabel = new QLabel("Gerer un equipement");
	gererEquipementLabel->setAlignment(Qt::AlignCenter);
	QFont font = gererEquipementLabel->font();
	font.setPointSize(24); // D�finir la taille de la police
	gererEquipementLabel->setFont(font);

	// Utiliser un QHBoxLayout pour centrer le QLabel horizontalement
	QHBoxLayout* labelLayout = new QHBoxLayout;
	labelLayout->addWidget(gererEquipementLabel);
	ui.verticalLayout_3->addLayout(labelLayout);

	// Cr�er un bouton "Nouvel �quipement" plus petit mais plus �pais
	QPushButton* newEquipmentButton = new QPushButton("Nouvel equipement");
	newEquipmentButton->setFixedSize(150, 50); // D�finir une taille fixe pour le bouton
	newEquipmentButton->setStyleSheet("QPushButton { padding: 10px; font-weight: bold; }"); // D�finir du padding et un texte en gras

	QObject::connect(newEquipmentButton, &QPushButton::clicked, this, &DMXProject::handleNewEquipmentButtonClicked);
	ui.verticalLayout_3->addWidget(newEquipmentButton);

	// Cr�er un nouveau QTableView
	QTableView* tableView = new QTableView;

	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setSelectionMode(QAbstractItemView::NoSelection);
	tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

	// Cr�er un mod�le de tableau vide avec cinq colonnes
	QStandardItemModel* model = new QStandardItemModel(0, 5);
	tableView->setModel(model);

	// D�finir les en-t�tes de colonne
	model->setHorizontalHeaderLabels(QStringList() << "Nom de l'equipement" << "Adresse" << "Nombre de Canal" << "Modifier" << "Supprimer");

	// Ex�cuter une requ�te pour r�cup�rer tous les �quipements existants
	QSqlQuery query("SELECT id, nom, adresse, nbCanal FROM equipement");

	int row = 0; // Variable pour garder une trace du num�ro de ligne

	// Parcourir les r�sultats de la requ�te et ajouter chaque �quipement en tant que ligne dans le mod�le de tableau
	while (query.next()) {
		int currentIdEquipement = query.value(0).toInt(); // R�cup�rer l'ID de l'�quipement actuel

		// V�rifier si l'ID de l'�quipement actuel correspond � l'ID de l'�quipement � supprimer
		if (currentIdEquipement == m_idEquipementASupprimer) {
			continue; // Ignorer cet �quipement car il doit �tre supprim�
		}

		QString nomEquipement = query.value(1).toString();
		QString adresseEquipement = query.value(2).toString();
		int nbCanalEquipement = query.value(3).toInt();

		QList<QStandardItem*> rowData;
		rowData << new QStandardItem(nomEquipement);
		rowData << new QStandardItem(adresseEquipement);
		rowData << new QStandardItem(QString::number(nbCanalEquipement));

		// Cr�er un bouton pour la quatri�me colonne "Modifier"
		QPushButton* modifyButton = new QPushButton("Modifier");
		QObject::connect(modifyButton, &QPushButton::clicked, this, [this, currentIdEquipement, nomEquipement, adresseEquipement, nbCanalEquipement]() {
			handleModifyButtonClicked(currentIdEquipement, nomEquipement, adresseEquipement, nbCanalEquipement);
			}); // Connecter le signal clicked du bouton � une lambda fonction
		model->setItem(row, 3, new QStandardItem(""));
		tableView->setIndexWidget(model->index(row, 3), modifyButton);

		// Cr�er un bouton pour la cinqui�me colonne "Supprimer"
		QPushButton* deleteButton = new QPushButton("Supprimer");
		deleteButton->setObjectName(QString::number(currentIdEquipement)); // Utiliser l'ID de l'�quipement comme nom d'objet
		QObject::connect(deleteButton, &QPushButton::clicked, this, &DMXProject::handleDeleteButtonClicked); // Connecter le signal clicked du bouton � une slot
		model->setItem(row, 4, new QStandardItem(""));
		tableView->setIndexWidget(model->index(row, 4), deleteButton);

		// Ajouter les autres donn�es de l'�quipement � la m�me ligne
		for (int i = 0; i < rowData.size(); ++i) {
			model->setItem(row, i, rowData[i]);
		}

		++row; // Incr�menter le num�ro de ligne
	}

	// Redimensionner les colonnes pour qu'elles s'adaptent au contenu
	tableView->resizeColumnsToContents();

	// D�finir les politiques de taille pour le tableau et le widget parent
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->verticalHeader()->setStretchLastSection(true);
	tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	// Ajouter le QTableView au layout verticalLayout_3 en prenant tout l'espace disponible
	ui.verticalLayout_3->addWidget(tableView);

	// R�initialiser l'ID de l'�quipement � supprimer
	m_idEquipementASupprimer = -1;
}


void DMXProject::handleNewEquipmentButtonClicked()
{
	// Logique pour ajouter un nouvel �quipement
	QDialog dialog(this);
	dialog.setWindowTitle("Ajouter un nouvel equipement");

	QFormLayout form(&dialog);

	// Ajout des champs d'entr�e
	QLineEdit* nomEdit = new QLineEdit(&dialog);
	form.addRow("Nom de l'equipement:", nomEdit);

	QLineEdit* adresseEdit = new QLineEdit(&dialog);
	form.addRow("Adresse:", adresseEdit);

	QLineEdit* canauxEdit = new QLineEdit(&dialog);
	form.addRow("Nombre de canaux:", canauxEdit);

	// Ajout des boutons
	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);

	// Connexion des boutons
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	// Afficher la bo�te de dialogue
	if (dialog.exec() == QDialog::Accepted) {
		QString nom = nomEdit->text();
		QString adresse = adresseEdit->text();
		int canaux = canauxEdit->text().toInt();

		// Ajouter l'�quipement � la base de donn�es
		QSqlQuery query;
		query.prepare("INSERT INTO equipement (nom, adresse, nbCanal) VALUES (?, ?, ?)");
		query.addBindValue(nom);
		query.addBindValue(adresse);
		query.addBindValue(canaux);

		if (query.exec()) {
			// R�afficher la liste des �quipements
			Gerer_un_equipement();

			// Cr�er une nouvelle fen�tre pour les �tiquettes de canal et les champs de texte associ�s
			QDialog channelDialog(this);
			channelDialog.setWindowTitle("Noms des canaux");

			QVBoxLayout channelLayout(&channelDialog);

			// R�cup�rer la valeur de l'adresse pour l'incr�mentation
			int adresseInt = adresse.toInt();

			// Affichez les QLabel et les QLineEdit pour chaque canal
			QVector<QLineEdit*> channelLineEditVector;
			for (int i = 0; i < canaux; ++i) {
				QLabel* label = new QLabel("Nom du canal " + QString::number(adresseInt + i) + " :");
				QLineEdit* lineEdit = new QLineEdit(&channelDialog);
				channelLayout.addWidget(label);
				channelLayout.addWidget(lineEdit);
				channelLineEditVector.append(lineEdit);
			}

			// Ajouter les boutons OK et Annuler � la fen�tre des canaux
			QDialogButtonBox channelButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &channelDialog);
			channelLayout.addWidget(&channelButtonBox);

			// Connexion des boutons
			QObject::connect(&channelButtonBox, &QDialogButtonBox::accepted, [&]() {
				// Ins�rer les noms des canaux dans la base de donn�es
				QSqlQuery channelQuery;

				// Pr�parer la requ�te pour l'insertion des noms des canaux
				channelQuery.prepare("INSERT INTO champ (idEquip, idNumCanal, nom) VALUES (:idEquip, :idNumCanal, :nom)");

				// R�cup�rer l'ID de l'�quipement ins�r�
				int idEquip = query.lastInsertId().toInt();

				// Parcourir les noms des canaux et les ins�rer dans la base de donn�es
				for (int i = 0; i < canaux; ++i) {
					QString channelName = channelLineEditVector[i]->text();

					channelQuery.bindValue(":idEquip", idEquip);
					channelQuery.bindValue(":idNumCanal", adresseInt + i ); // Identifiant num�rique du canal
					channelQuery.bindValue(":nom", channelName);

					if (!channelQuery.exec()) {
						qDebug() << "Erreur lors de l'insertion des donn�es dans la table champ:" << channelQuery.lastError().text();
						QMessageBox::warning(this, "Erreur", "�chec de l'ajout des noms des canaux � la base de donn�es: " + channelQuery.lastError().text());
						break;
					}
				}

				// Fermer la fen�tre des canaux une fois que l'insertion est termin�e
				channelDialog.accept();
				});

			QObject::connect(&channelButtonBox, &QDialogButtonBox::rejected, &channelDialog, &QDialog::reject);

			// Afficher la fen�tre des canaux
			channelDialog.exec();
		}
		else {
			QMessageBox::warning(this, "Erreur", "�chec de l'ajout de l'�quipement � la base de donn�es.");
		}
	}
}



void DMXProject::handleDeleteButtonClicked()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender()); // R�cup�rer le bouton qui a �mis le signal
	if (button) {
		int idEquipement = button->objectName().toInt(); // R�cup�rer l'ID de l'�quipement � partir du nom d'objet du bouton
		// Appeler la fonction de suppression en passant l'ID de l'�quipement � supprimer
		equipement->supprimerEquipement(idEquipement, this);
		Gerer_un_equipement();
	}
}

void DMXProject::handleModifyButtonClicked(int idEquipement, const QString& nomEquipement, const QString& adresseEquipement, int nbCanalEquipement)
{
	// Cr�ation d'une bo�te de dialogue pour modifier les informations de l'�quipement
	QDialog dialog(this);
	dialog.setWindowTitle("Modifier �quipement");

	// Cr�ation des widgets pour saisir les nouvelles informations
	QLabel* labelNom = new QLabel("Nom :", &dialog);
	QLineEdit* lineEditNom = new QLineEdit(nomEquipement, &dialog);

	QLabel* labelAdresse = new QLabel("Adresse :", &dialog);
	QLineEdit* lineEditAdresse = new QLineEdit(adresseEquipement, &dialog);

	QLabel* labelNbCanal = new QLabel("Nombre de canaux :", &dialog);
	QSpinBox* spinBoxNbCanal = new QSpinBox(&dialog);
	spinBoxNbCanal->setValue(nbCanalEquipement);

	// Cr�ation du bouton de confirmation
	QPushButton* buttonConfirm = new QPushButton("Confirmer", &dialog);
	connect(buttonConfirm, &QPushButton::clicked, &dialog, &QDialog::accept);

	// Cr�ation du layout pour organiser les widgets
	QVBoxLayout* layout = new QVBoxLayout(&dialog);
	layout->addWidget(labelNom);
	layout->addWidget(lineEditNom);
	layout->addWidget(labelAdresse);
	layout->addWidget(lineEditAdresse);
	layout->addWidget(labelNbCanal);
	layout->addWidget(spinBoxNbCanal);
	layout->addWidget(buttonConfirm);

	// Affichage de la bo�te de dialogue
	if (dialog.exec() == QDialog::Accepted) {
		// R�cup�rer les nouvelles valeurs saisies par l'utilisateur
		QString nouveauNom = lineEditNom->text();
		QString nouvelleAdresse = lineEditAdresse->text();
		int nouveauNbCanal = spinBoxNbCanal->value();

		// Appeler la fonction de modification avec les nouvelles valeurs
		equipement->updateEquipement(idEquipement, nouveauNom, nouvelleAdresse, nouveauNbCanal);
		// Mettre � jour l'affichage des �quipements
		equipement->afficherEquipements(ui.verticalLayoutEquipements);

		Gerer_un_equipement();

	}
}


void DMXProject::saveSettings() {
	// R�cup�rer l'ID de la sc�ne s�lectionn�e
	int idScene = scene->getSceneId(m_selectedScene); //getSceneId(m_selectedScene);

	QWizard* wizard = qobject_cast<QWizard*>(sender());
	if (wizard) {
		QList<QWizardPage*> pages = wizard->findChildren<QWizardPage*>();
		for (QList<QWizardPage*>::iterator it = pages.begin(); it != pages.end(); ++it) {
			QWizardPage* page = *it;

			// R�cup�rer les valeurs actuelles des QSpinBox dans la page actuelle
			QList<QPair<int, int>> channelData;
			QList<QSpinBox*> spinBoxes = page->findChildren<QSpinBox*>();
			for (QSpinBox* spinBox : spinBoxes) {
				// R�cup�rer le num�ro de canal � partir du nom d'objet du QSpinBox
				QString objectName = spinBox->objectName();
				int index = objectName.lastIndexOf('_');
				int numCanal = objectName.mid(index + 1).toInt();

				// R�cup�rer la valeur actuelle du QSpinBox
				int valeur = spinBox->value();

				// Ajouter la paire (numCanal, valeur) � la liste channelData
				channelData.append(qMakePair(numCanal, valeur));
			}

			// Enregistrer les param�tres de canal pour cet �quipement dans la base de donn�es
			canal->insertCanal(idScene, channelData);
		}
	}

	// Fermer le QWizard
	qobject_cast<QWizard*>(sender())->close();
}

void DMXProject::sendDMXFrame()
{
	// R�cup�rer l'ID de la sc�ne s�lectionn�e dans votre interface utilisateur
	int sceneId = 5; //ui->sceneComboBox->currentData().toInt()

	// R�cup�rer les valeurs des canaux correspondants dans votre base de donn�es
	QSqlQuery query;
	query.prepare("SELECT numCanal, valeur FROM canaux WHERE idScene = :sceneId");
	query.bindValue(":sceneId", sceneId);
	query.exec();

	// Construire la trame DMX
	QByteArray dmxFrame(512, 0);
	while (query.next()) {
		int numCanal = query.value(0).toInt();
		int valeur = query.value(1).toInt();
		dmxFrame[numCanal - 1] = valeur;
	}

	// Envoyer la trame DMX � votre serveur Linux en utilisant une connexion TCP
	QTcpSocket socket;
	socket.connectToHost("192.168.64.170", 12345); // Remplacez par l'adresse IP et le port de votre serveur Linux
	if (socket.waitForConnected()) {
		socket.write(dmxFrame);
		socket.waitForBytesWritten();
		socket.disconnectFromHost();
	}
	else {
		qDebug() << "Erreur : impossible de se connecter au serveur.";
	}
}

void DMXProject::fillSceneComboBox()
{
	QSqlQuery query;
	query.prepare("SELECT nom FROM scene");
	query.exec();

	while (query.next()) {
		ui.sceneComboBox->addItem(query.value(0).toString());
	}
}

void DMXProject::testScene()
{
	// R�cup�rer le nom de la sc�ne s�lectionn�e dans le QComboBox
	QString sceneName = ui.sceneComboBox->currentText();

	// S�lectionner l'ID de la sc�ne correspondant au nom s�lectionn� dans la base de donn�es
	QSqlQuery query;
	query.prepare("SELECT id FROM scene WHERE nom = :sceneName");
	query.bindValue(":sceneName", sceneName);
	query.exec();

	int sceneId = -1;
	if (query.next()) {
		sceneId = query.value(0).toInt();
	}

	if (sceneId != -1) {
		// Convertir l'ID de la sc�ne en QByteArray
		QByteArray sceneIdData = QByteArray::number(sceneId);

		// Envoyer l'ID de la sc�ne au serveur Linux en utilisant une connexion TCP
		QTcpSocket socket;
		socket.connectToHost("192.168.64.170", 12345); // Remplacez par l'adresse IP et le port de votre serveur Linux
		if (socket.waitForConnected()) {
			socket.write(sceneIdData);
			socket.waitForBytesWritten();
			socket.disconnectFromHost();
		}
		else {
			qDebug() << "Erreur : impossible de se connecter au serveur.";
		}
	}
	else {
		qDebug() << "Erreur : sc�ne non trouv�e.";
	}
}


void afficherEmplacementsLibresDansTrame() {
	// Connexion � la base de donn�es et r�cup�ration des adresses et du nombre de canaux de chaque �quipement
	QSqlQuery query("SELECT adresse, nbCanal FROM equipement ORDER BY adresse ASC;");

	// Structure de donn�es pour stocker les emplacements des �quipements dans la trame
	std::map<int, int> emplacements;

	// Assignation des emplacements pour chaque �quipement
	while (query.next()) {
		int adresse = query.value(0).toInt();
		int nbCanal = query.value(1).toInt();
		int emplacement = adresse + nbCanal; // Calcul de l'emplacement dans la trame
		emplacements[emplacement] = nbCanal; // Stockage de l'emplacement dans la map
	}

	// Calcul des emplacements libres dans la trame
	int dernierEmplacementUtilise = 0;
	int nombreEmplacementsLibres = 0;
	for (const auto& pair : emplacements) {
		if (pair.first > dernierEmplacementUtilise) {
			nombreEmplacementsLibres += pair.first - dernierEmplacementUtilise;
		}
		dernierEmplacementUtilise = pair.first + pair.second;
	}
	nombreEmplacementsLibres += 255 - dernierEmplacementUtilise;

	// Affichage des emplacements libres dans une bo�te de dialogue
	QMessageBox::information(nullptr, "Emplacements libres dans la trame",
		QString("Nombre d'emplacements libres dans la trame : %1").arg(nombreEmplacementsLibres));
}

void DMXProject::fillSceneComboBox2() {
	Scene scene;
	QList<Scene> scenes = scene.getAllScenes();
	foreach(const Scene & s, scenes) {
		ui.SceneComboBox->addItem(s.getNom());
	}
}


void DMXProject::fillEquipComboBox() {
	Equipement equip;
	QList<Equipement> equips = equip.getAllEquipements();
	foreach(const Equipement & e, equips) {
		ui.EquipComboBox->addItem(e.getNom());
	}
}


void DMXProject::onValidSceneEquipButtonClicked() {
	// R�cup�rer l'index de la sc�ne s�lectionn�e dans le QComboBox
	int index = ui.SceneComboBox->currentIndex();

	// V�rifier si une sc�ne est s�lectionn�e
	if (index != -1) {
		// R�cup�rer le nom de la sc�ne s�lectionn�e
		QString sceneName = ui.SceneComboBox->currentText();

		// Afficher le nom de la sc�ne dans le QLabel
		ui.nomSceneAfficheLabel->setText(sceneName);
	}
	else {
		// Aucune sc�ne s�lectionn�e, afficher un message d'erreur ou laisser le QLabel vide
		ui.nomSceneAfficheLabel->setText("");
	}
}

void DMXProject::fetchEquipmentChampData(int equipId) {
	QSqlQuery query;
	query.prepare("SELECT champ.nom, champ.idNumCanal "
		"FROM champ "
		"JOIN equipement ON champ.idEquip = equipement.id "
		"WHERE equipement.id = ? "
		"ORDER BY champ.idNumCanal");
	query.addBindValue(equipId);

	if (query.exec()) {
		m_champNames.clear();
		m_champNumbers.clear();
		// R�initialiser la liste m_champSliderValues avec la bonne taille et la remplir avec des z�ros
		m_champSliderValues.clear();
		int champCount = query.size();
		for (int i = 0; i < champCount; ++i) {
			m_champSliderValues.append(0);
		}

		while (query.next()) {
			m_champNames.append(query.value(0).toString());
			m_champNumbers.append(query.value(1).toInt());
		}
		m_currentChampIndex = 0;

		// Afficher le nom du premier champ dans le QLabel
		if (!m_champNames.isEmpty()) {
			ui.valeurAfficheLabel->setText(m_champNames.at(m_currentChampIndex));
		}
	}
	else {
		qDebug() << "Erreur lors de l'ex�cution de la requ�te : " << query.lastError();
	}
}


void DMXProject::saveSceneEquipmentData(int idScene) {
	// V�rifier si des canaux existent d�j� pour cette sc�ne
	QSqlQuery checkCanauxQuery;
	checkCanauxQuery.prepare("SELECT COUNT(*) FROM canaux WHERE idScene = :idScene");
	checkCanauxQuery.bindValue(":idScene", idScene);
	if (!checkCanauxQuery.exec() || !checkCanauxQuery.next()) {
		qDebug() << "Erreur lors de l'ex�cution de la requ�te checkCanauxQuery:" << checkCanauxQuery.lastError().text();
		return;
	}
	int countCanaux = checkCanauxQuery.value(0).toInt();

	// Si des canaux existent d�j�, demander confirmation � l'utilisateur
	if (countCanaux > 0) {
		QMessageBox msgBox;
		msgBox.setText("La sc�ne s�lectionn�e poss�de d�j� une configuration. Voulez-vous la supprimer ?");
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Ok) {
			// Supprimer les canaux existants
			QSqlQuery deleteQuery;
			deleteQuery.prepare("DELETE FROM canaux WHERE idScene = :idScene");
			deleteQuery.bindValue(":idScene", idScene);
			if (!deleteQuery.exec()) {
				qDebug() << "Erreur lors de l'ex�cution de la requ�te deleteQuery:" << deleteQuery.lastError().text();
				return;
			}
		}
		else {
			// Annuler l'op�ration
			return;
		}
	}

	// Enregistrer les nouvelles valeurs de canal pour la sc�ne
	QSqlQuery insertQuery;
	insertQuery.prepare("INSERT INTO canaux (idScene, numCanal, valeur) VALUES (:idScene, :numCanal, :valeur)");

	for (int i = 0; i < m_champNumbers.size(); ++i) {
		insertQuery.bindValue(":idScene", idScene);
		insertQuery.bindValue(":numCanal", m_champNumbers[i]);
		insertQuery.bindValue(":valeur", m_champSliderValues[i]);

		if (!insertQuery.exec()) {
			qDebug() << "Erreur lors de l'ex�cution de la requ�te insertQuery:" << insertQuery.lastError().text();
			return;
		}
	}

	// Rediriger vers la page pr�c�dente apr�s l'enregistrement r�ussi
}


void DMXProject::onValidSceneEquipButtonClickedArduino() {
	// R�cup�rer l'index de la sc�ne s�lectionn�e dans le QComboBox
	int sceneIndex = ui.SceneComboBox->currentIndex();

	// V�rifier si une sc�ne est s�lectionn�e
	if (sceneIndex != -1) {
		// R�cup�rer l'index de l'�quipement s�lectionn� dans le QComboBox
		int equipIndex = ui.EquipComboBox->currentIndex();

		// V�rifier si un �quipement est s�lectionn�
		if (equipIndex != -1) {
			// R�cup�rer l'ID de l'�quipement s�lectionn� � partir de vos donn�es d'�quipement

			QString equipName = ui.EquipComboBox->currentText();

			int equipId = equipement->getEquipmentId(equipName);

			// R�cup�rer les noms et les num�ros de canal des champs de l'�quipement s�lectionn�
			fetchEquipmentChampData(equipId);
		}
		else {
			// Aucun �quipement s�lectionn�, afficher un message d'erreur ou effectuer d'autres actions de suivi
		}
	}
	else {
		// Aucune sc�ne s�lectionn�e, afficher un message d'erreur ou effectuer d'autres actions de suivi
	}
}

void DMXProject::onValidateCanalButtonClicked() {
	// V�rifier si il y a d'autres champs � traiter
	if (m_currentChampIndex < m_champNumbers.count() - 1) {
		// Passer au champ suivant
		m_currentChampIndex++;

		// Afficher le nom du champ courant dans le QLabel
		ui.valeurAfficheLabel->setText(m_champNames.at(m_currentChampIndex));
	}
	else {
		// Tous les champs ont �t� trait�s, enregistrer les valeurs dans la table 'canaux'
		QString sceneName = ui.SceneComboBox->currentText();

		// V�rifier si une sc�ne est s�lectionn�e
		if (sceneName != -1) {
			// R�cup�rer l'ID de la sc�ne s�lectionn�e � partir de vos donn�es de sc�ne
			int sceneId = scene->getSceneId(sceneName); 

			// R�cup�rer l'ID de l'�quipement s�lectionn� � partir de vos donn�es d'�quipement
			QString equipName = ui.EquipComboBox->currentText();

			int equipId = equipement->getEquipmentId(equipName); 

			// Enregistrer les valeurs des champs dans la table 'canaux'
			saveSceneEquipmentData(sceneId);
		}
		else {
			// Aucune sc�ne s�lectionn�e, afficher un message d'erreur ou effectuer d'autres actions de suivi
		}
	}
}

void DMXProject::updateChampSliderValue(int value) {
    if (m_currentChampIndex >= 0 && m_currentChampIndex < m_champSliderValues.count()) {
        m_champSliderValues[m_currentChampIndex] = value;
    }
}

void DMXProject::onValidateButtonPressed() {
	onValidateCanalButtonClicked();
}

void DMXProject::gererScenes() {
	// Supprimer tous les widgets existants dans le layout
	QLayoutItem* child;
	while ((child = ui.verticalLayout_7->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}

	// Cr�er un QLabel avec du texte en grand au milieu
	QLabel* gererSceneLabel = new QLabel("Gerer les scenes");
	gererSceneLabel->setAlignment(Qt::AlignCenter);
	QFont font = gererSceneLabel->font();
	font.setPointSize(24); // D�finir la taille de la police
	gererSceneLabel->setFont(font);

	// Utiliser un QHBoxLayout pour centrer le QLabel horizontalement
	QHBoxLayout* labelLayout = new QHBoxLayout;
	labelLayout->addWidget(gererSceneLabel);
	ui.verticalLayout_7->addLayout(labelLayout);

	// Cr�er un bouton pour ajouter une nouvelle sc�ne
	QPushButton* addSceneButton = new QPushButton("Ajouter une scene");
	addSceneButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // D�finir une taille fixe pour le bouton
	addSceneButton->setMaximumHeight(80); // D�finir la hauteur maximale du bouton
	connect(addSceneButton, &QPushButton::clicked, this, &DMXProject::ajouterScene);

	// Cr�er un layout horizontal pour le bouton "Ajouter une sc�ne"
	QHBoxLayout* addButtonLayout = new QHBoxLayout;
	addButtonLayout->addWidget(addSceneButton);
	addButtonLayout->addStretch(); // Ajouter un espace flexible � droite du bouton
	ui.verticalLayout_7->addLayout(addButtonLayout);

	// Cr�er un nouveau QTableView
	QTableView* tableView = new QTableView;

	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setSelectionMode(QAbstractItemView::NoSelection);
	tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

	// Cr�er un mod�le de tableau vide avec trois colonnes
	QStandardItemModel* model = new QStandardItemModel(0, 3);
	tableView->setModel(model);

	// D�finir les en-t�tes de colonne
	model->setHorizontalHeaderLabels(QStringList() << "Nom de la scene" << "Modifier" << "Supprimer");

	// Ex�cuter une requ�te pour r�cup�rer tous les noms de sc�nes existantes
	QSqlQuery query("SELECT nom FROM scene");

	int row = 0; // Variable pour garder une trace du num�ro de ligne

	// Parcourir les r�sultats de la requ�te et ajouter chaque nom de sc�ne en tant que ligne dans le mod�le de tableau
	while (query.next()) {
		QString nomScene = query.value(0).toString();

		// Colonne "Nom de la sc�ne"
		QStandardItem* itemName = new QStandardItem(nomScene);
		model->setItem(row, 0, itemName);

		// Colonne "Modifier"
		QPushButton* modifyButton = new QPushButton("Modifier");
		QObject::connect(modifyButton, &QPushButton::clicked, this, [this, nomScene]() {
			handleModifySceneButtonClicked(nomScene);
			});
		tableView->setIndexWidget(model->index(row, 1), modifyButton);

		// Colonne "Supprimer"
		QPushButton* deleteButton = new QPushButton("Supprimer");
		deleteButton->setObjectName(nomScene); // Utiliser le nom de la sc�ne comme nom d'objet
		QObject::connect(deleteButton, &QPushButton::clicked, this, &DMXProject::handleDeleteSceneButtonClicked);
		tableView->setIndexWidget(model->index(row, 2), deleteButton);

		++row; // Incr�menter le num�ro de ligne
	}

	// Redimensionner les colonnes pour qu'elles s'adaptent au contenu
	tableView->resizeColumnsToContents();

	// D�finir les politiques de taille pour le tableau et le widget parent
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->verticalHeader()->setStretchLastSection(true);
	tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	// Ajouter le QTableView au layout verticalLayout_7 en prenant tout l'espace disponible
	ui.verticalLayout_7->addWidget(tableView);
}




void DMXProject::handleDeleteSceneButtonClicked() {
	// V�rifier si l'�metteur du signal est bien un QPushButton
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (!button) {
		qDebug() << "Erreur: emetteur du signal n'est pas un QPushButton.";
		return;
	}

	// R�cup�rer le nom de la sc�ne � partir de l'objet QPushButton
	QString nomScene = button->objectName(); // Utiliser le nom de la sc�ne comme ID

	// Effectuer la suppression des d�pendances dans la table canaux
	QSqlQuery queryCanaux;
	queryCanaux.prepare("DELETE FROM canaux WHERE idScene IN (SELECT id FROM scene WHERE nom = :nomScene)");
	queryCanaux.bindValue(":nomScene", nomScene);
	if (!queryCanaux.exec()) {
		qDebug() << "Erreur lors de la suppression des dependances dans la table canaux:" << queryCanaux.lastError().text();
		return;
	}

	// Effectuer la suppression des d�pendances dans la table lightBoard
	QSqlQuery queryLightBoard;
	queryLightBoard.prepare("DELETE FROM lightBoard WHERE idScene IN (SELECT id FROM scene WHERE nom = :nomScene)");
	queryLightBoard.bindValue(":nomScene", nomScene);
	if (!queryLightBoard.exec()) {
		qDebug() << "Erreur lors de la suppression des dependances dans la table lightBoard:" << queryLightBoard.lastError().text();
		return;
	}

	// Effectuer la suppression de la sc�ne dans la table scene
	QSqlQuery queryScene;
	queryScene.prepare("DELETE FROM scene WHERE nom = :nomScene");
	queryScene.bindValue(":nomScene", nomScene);
	if (!queryScene.exec()) {
		qDebug() << "Erreur lors de la suppression de la scene dans la table scene:" << queryScene.lastError().text();
		return;
	}
	// Cr�er un bouton pour ajouter une nouvelle sc�ne
	QPushButton* addSceneButton = new QPushButton("Ajouter une sc�ne");
	connect(addSceneButton, &QPushButton::clicked, this, &DMXProject::ajouterScene);
	ui.verticalLayout_7->addWidget(addSceneButton);

	// Rafra�chir l'affichage des sc�nes apr�s la suppression
	gererScenes();

	// Afficher le nom de la sc�ne supprim�e dans le QDebug
	qDebug() << "Scene supprimee avec succes. Nom de la scene supprimee:" << nomScene;
}

void DMXProject::handleModifySceneButtonClicked(const QString& nomScene) {
	// Cr�er une bo�te de dialogue de modification de la sc�ne
	QDialog* modifySceneDialog = new QDialog(this);
	modifySceneDialog->setWindowTitle("Modifier la scene");

	// Cr�er un layout vertical pour la bo�te de dialogue
	QVBoxLayout* dialogLayout = new QVBoxLayout(modifySceneDialog);

	// Ajouter un QLabel pour afficher le texte d'instruction
	QLabel* instructionLabel = new QLabel("Entrez le nouveau nom de la scene:");
	dialogLayout->addWidget(instructionLabel);

	// Ajouter un QLineEdit pour permettre � l'utilisateur de saisir le nouveau nom de la sc�ne
	QLineEdit* sceneNameLineEdit = new QLineEdit(nomScene);
	dialogLayout->addWidget(sceneNameLineEdit);

	// Ajouter les boutons "Annuler" et "Modifier"
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
	dialogLayout->addWidget(buttonBox);

	// Connecter le bouton "Annuler" pour fermer la bo�te de dialogue sans effectuer de modifications
	connect(buttonBox, &QDialogButtonBox::rejected, modifySceneDialog, &QDialog::reject);

	// Connecter le bouton "Modifier" pour effectuer les modifications et fermer la bo�te de dialogue
	connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
		QString newSceneName = sceneNameLineEdit->text();
		// Effectuer la requ�te SQL pour mettre � jour le nom de la sc�ne dans la base de donn�es
		QSqlQuery query;
		query.prepare("UPDATE scene SET nom = :newSceneName WHERE nom = :oldSceneName");
		query.bindValue(":newSceneName", newSceneName);
		query.bindValue(":oldSceneName", nomScene);
		if (!query.exec()) {
			qDebug() << "Erreur lors de la mise a jour du nom de la scene:" << query.lastError().text();
			return;
		}

		// Afficher un message de confirmation
		QMessageBox::information(this, "Modification reussie", "Le nom de la scene a ete modifie avec succes en : " + newSceneName + "\n Attention la scene a pu remonte dans le tableau");

		// Rafra�chir l'affichage des sc�nes apr�s la modification
		gererScenes();

		// Fermer la bo�te de dialogue
		modifySceneDialog->accept();
		});

	// Afficher la bo�te de dialogue de modification de la sc�ne
	modifySceneDialog->exec();
}

void DMXProject::ajouterScene() {
	// Cr�er une bo�te de dialogue pour ajouter une nouvelle sc�ne
	QDialog* addSceneDialog = new QDialog(this);
	addSceneDialog->setWindowTitle("Ajouter une nouvelle sc�ne");

	// Cr�er un layout vertical pour la bo�te de dialogue
	QVBoxLayout* dialogLayout = new QVBoxLayout(addSceneDialog);

	// Ajouter un QLabel pour afficher le texte d'instruction
	QLabel* instructionLabel = new QLabel("Entrez le nom de la nouvelle sc�ne:");
	dialogLayout->addWidget(instructionLabel);

	// Ajouter un QLineEdit pour permettre � l'utilisateur de saisir le nom de la nouvelle sc�ne
	QLineEdit* sceneNameLineEdit = new QLineEdit();
	dialogLayout->addWidget(sceneNameLineEdit);

	// Ajouter les boutons "Annuler" et "Ajouter"
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
	dialogLayout->addWidget(buttonBox);

	// Connecter le bouton "Annuler" pour fermer la bo�te de dialogue sans ajouter de nouvelle sc�ne
	connect(buttonBox, &QDialogButtonBox::rejected, addSceneDialog, &QDialog::reject);

	// Connecter le bouton "Ajouter" pour ajouter la nouvelle sc�ne � la base de donn�es et fermer la bo�te de dialogue
	connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
		QString newSceneName = sceneNameLineEdit->text();
		// Effectuer la requ�te SQL pour ajouter une nouvelle sc�ne dans la base de donn�es
		QSqlQuery query;
		query.prepare("INSERT INTO scene (nom) VALUES (:newSceneName)");
		query.bindValue(":newSceneName", newSceneName);
		if (!query.exec()) {
			qDebug() << "Erreur lors de l'ajout de la nouvelle sc�ne:" << query.lastError().text();
			return;
		}

		// Afficher un message de confirmation
		QMessageBox::information(this, "Sc�ne ajout�e", "La nouvelle sc�ne a �t� ajout�e avec succ�s : " + newSceneName);

		// Rafra�chir l'affichage des sc�nes apr�s l'ajout
		gererScenes();

		// Fermer la bo�te de dialogue
		addSceneDialog->accept();
		});

	// Afficher la bo�te de dialogue pour ajouter une nouvelle sc�ne
	addSceneDialog->exec();
}

void DMXProject::onSliderValueChanged(int value)
{
	// Mettre � jour le texte du label avec la valeur du slider
	ui.valeurSlider->setText(QString::number(value));
}

void DMXProject::selectPreviousScene() {
	int currentIndex = ui.sceneComboBox->currentIndex();
	if (currentIndex > 0) {
		ui.sceneComboBox->setCurrentIndex(currentIndex - 1);
	}

	// Envoyer le nom de la sc�ne s�lectionn�e � l'Arduino
	QString textCombobox = ui.sceneComboBox->currentText();
	QByteArray sceneName = textCombobox.toUtf8();
	sceneName.append("\n");  // Ajoutez un retour � la ligne pour indiquer la fin de la transmission
	consoleMaterielle->sendData(sceneName);  // Utilisez une m�thode de ConsoleMaterielle pour envoyer les donn�es
}

void DMXProject::selectNextScene() {
	int currentIndex = ui.sceneComboBox->currentIndex();
	if (currentIndex < ui.sceneComboBox->count() - 1) {
		ui.sceneComboBox->setCurrentIndex(currentIndex + 1);
	}

	// Envoyer le nom de la sc�ne s�lectionn�e � l'Arduino
	QString textCombobox = ui.sceneComboBox->currentText();
	QByteArray sceneName = textCombobox.toUtf8();
	sceneName.append("\n");  // Ajoutez un retour � la ligne pour indiquer la fin de la transmission
	consoleMaterielle->sendData(sceneName);  // Utilisez une m�thode de ConsoleMaterielle pour envoyer les donn�es
}


void DMXProject::confirmSelection()
{
	// Action � d�finir pour la confirmation de s�lection
	qDebug() << "Scene confirmed:" << ui.sceneComboBox->currentText();
}

void DMXProject::validateSelection()
{
	ui.testSceneButton->click();
	// Action � d�finir pour la validation de s�lection
	qDebug() << "Scene validated:" << ui.sceneComboBox->currentText();
}