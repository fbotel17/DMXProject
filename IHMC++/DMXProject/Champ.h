#pragma once

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

class Champ
{
public:
    int id;
    int idEquip;
    int idNumCanal;
    QString nom;

    void insertChamp(int idEquip, int idNumCanal, const QString& nom);
};

