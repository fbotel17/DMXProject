#pragma once
#ifndef CANAL_H
#define CANAL_H

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

class Canal : public QObject
{
    Q_OBJECT

public:
    Canal();

    void insertCanal(int idScene, const QList<QPair<int, int>>& channelData);
};

#endif // CANAL_H


