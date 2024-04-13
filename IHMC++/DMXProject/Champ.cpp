#include "Champ.h"


void Champ::insertChamp(int idEquip, int idNumCanal, const QString& nom)
{
    QSqlQuery query;
    query.prepare("INSERT INTO champ (idEquip, idNumCanal, nom) VALUES (:idEquip, :idNumCanal, :nom)");
    query.bindValue(":idEquip", idEquip);
    query.bindValue(":idNumCanal", idNumCanal);
    query.bindValue(":nom", nom);

    if (query.exec()) {
        qDebug() << "Champ inséré avec succès!";
    }
    else {
        qDebug() << "Erreur lors de l'insertion duchamp:" << query.lastError().text();
    }
}