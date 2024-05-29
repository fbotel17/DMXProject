#include "constructtrame.h"

ConstructTrame::ConstructTrame(QObject *parent) : QObject(parent)
{
    // Initialiser la connexion à la base de données si nécessaire
}

QVector<int> ConstructTrame::buildTrame(int sceneId)
{
    QVector<int> dmxValues = getDMXValuesFromDatabase(sceneId);
    // Construire la trame DMX avec les valeurs récupérées
    QVector<int> dmxTrame(512, 0); // Initialiser avec 512 canaux à 0
    for (int i = 0; i < dmxValues.size(); ++i) {
        dmxTrame[i] = dmxValues[i];
    }
    return dmxTrame;
}

QVector<int> ConstructTrame::getDMXValuesFromDatabase(int sceneId)
{
    QVector<int> dmxValues(512, 0); // 512 canaux initialisés à 0
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.prepare("SELECT numCanal, valeur FROM canaux WHERE idScene = :sceneId");
    query.bindValue(":sceneId", sceneId);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des valeurs DMX:" << query.lastError().text();
        return dmxValues;
    }

    while (query.next()) {
        int channelNumber = query.value(0).toInt();
        int channelValue = query.value(1).toInt();
        if (channelNumber >= 1 && channelNumber <= 512) {
            dmxValues[channelNumber - 1] = channelValue;
        }
    }

    return dmxValues;
}

