#include "Canal.h"

Canal::Canal()
{
}

void Canal::insertCanal(int idScene, const QList<QPair<int, int>>& channelData) {
    QSqlQuery query;
    query.prepare("INSERT INTO canaux (idScene, numCanal, valeur) VALUES (:idScene, :numCanal, :valeur)");
    query.bindValue(":idScene", idScene);

    for (const auto& channel : channelData) {
        int numCanal = channel.first;
        int valeur = channel.second;

        // V�rifier si le num�ro de canal existe dans la table champ
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM champ WHERE idNumCanal = :numCanal");
        checkQuery.bindValue(":numCanal", numCanal);
        if (checkQuery.exec() && checkQuery.next()) {
            int count = checkQuery.value(0).toInt();
            if (count == 0) {
                qDebug() << "Le num�ro de canal" << numCanal << "n'existe pas dans la table champ.";
                continue; // Ignorer cette insertion
            }
        }
        else {
            qDebug() << "Erreur lors de la v�rification de l'existence du num�ro de canal" << numCanal << ":" << checkQuery.lastError().text();
            continue; // Ignorer cette insertion
        }

        query.bindValue(":numCanal", numCanal);
        query.bindValue(":valeur", valeur);

        if (!query.exec()) {
            qDebug() << "Erreur lors de l'insertion des donn�es dans la table 'canaux' : " << query.lastError().text();
        }
    }
}


