#include "DMXProject.h"
#include <QSqlDatabase>
#include <QtSql>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>


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
	// Récupérer les équipements depuis la base de données
	QSqlQuery query("SELECT nom FROM Equipement");
	QStringList equipements;
	while (query.next()) {
		equipements << query.value(0).toString();
	}

	// Créer les widgets
	QLabel *labelNomScene = new QLabel("Nom de la scene:", this);
	QLineEdit *lineEditNomScene = new QLineEdit(this);

	QLabel *labelEquipements = new QLabel("Equipements:", this);

	// Créer dynamiquement les cases à cocher pour chaque équipement
	QList<QCheckBox*> checkBoxes;
	for (const QString &equipement : equipements) {
		QCheckBox *checkBox = new QCheckBox(equipement, this);
		checkBoxes << checkBox;
	}

	QLabel *labelValeurCanal = new QLabel("Valeur du Canal:", this);
	QSpinBox *spinBoxValeurCanal1 = new QSpinBox(this);
	QSpinBox *spinBoxValeurCanal2 = new QSpinBox(this);

	QPushButton *pushButtonValider = new QPushButton("Valider", this);

	// Organiser les widgets dans une mise en page
	QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

	// Ajouter les widgets au layout
	layoutPrincipal->addWidget(labelNomScene);
	layoutPrincipal->addWidget(lineEditNomScene);
	layoutPrincipal->addWidget(labelEquipements);

	// Ajouter les cases à cocher au layout
	for (QCheckBox *checkBox : checkBoxes) {
		layoutPrincipal->addWidget(checkBox);
	}

	layoutPrincipal->addWidget(labelValeurCanal);
	layoutPrincipal->addWidget(spinBoxValeurCanal1);
	layoutPrincipal->addWidget(spinBoxValeurCanal2);
	layoutPrincipal->addWidget(pushButtonValider);

	// Appliquer le layout à la fenêtre principale
	QWidget *widgetCentral = new QWidget(this);
	widgetCentral->setLayout(layoutPrincipal);
	setCentralWidget(widgetCentral);

	// Connecter le bouton Valider à un slot
	connect(pushButtonValider, SIGNAL(clicked()), this, SLOT(on_pushButtonValider_clicked()));
}

DMXProject::~DMXProject()
{}
