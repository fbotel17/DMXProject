#ifndef CONSTRUCTTRAME_H
#define CONSTRUCTTRAME_H

#include <QObject>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class ConstructTrame : public QObject
{
    Q_OBJECT
public:
    explicit ConstructTrame(QObject *parent = nullptr);
    QVector<int> buildTrame(int sceneId);

private:
    QVector<int> getDMXValuesFromDatabase(int sceneId);
};

#endif // CONSTRUCTTRAME_H
