#pragma once
#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

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

class DMXProject;

class Equipement
{
public:
    int id;
    QString nom;
    QString adresse;
    int nbCanaux;

    Equipement();
    Equipement(int id, const QString& nom, const QString& adresse, int nbCanaux);

    void insertEquipement(const QString& nomEquipement, const QString& adresseEquipement, int nbCanaux);
    void updateEquipement(int idEquipement, const QString& nomEquipement, const QString& adresseEquipement, int nbCanalEquipement);
    QList<Equipement> getAllEquipements();
    void afficherEquipements(QVBoxLayout* mainLayoutEquipements);
    int getEquipmentId(const QString& equipmentName);
    void supprimerEquipement(int idEquipement, DMXProject* dmx);

};

#endif // EQUIPEMENT_H

