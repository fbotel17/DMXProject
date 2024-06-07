#include "scene.h"
#include <QSqlError>
#include <QDebug>

Scene::Scene() : id(0), nom("") {}

Scene::Scene(int id, const QString& nom) : id(id), nom(nom) {}

int Scene::getId() const
{
    return id;
}

void Scene::setId(int id)
{
    this->id = id;
}

QString Scene::getNom() const
{
    return nom;
}

void Scene::setNom(const QString& nom)
{
    this->nom = nom;
}

void Scene::insertScene(const QString& nomScene)
{
    QSqlQuery query;
    query.prepare("INSERT INTO scene (nom) VALUES (:nom)");
    query.bindValue(":nom", nomScene);

    if (query.exec()) {
        qDebug() << "Scene inseree avec succes!";
    }
    else {
        qDebug() << "Erreur lors de l'insertion de la scene:" << query.lastError().text();
    }
}

void Scene::updateScene(const QString& oldNomScene, const QString& newNomScene)
{
    QSqlQuery query;
    query.prepare("UPDATE scene SET nom = (:newNom) WHERE nom = (:oldNom)");
    query.bindValue(":newNom", newNomScene);
    query.bindValue(":oldNom", oldNomScene);

    if (query.exec()) {
        qDebug() << "Scene modifiee avec succes!";
    }
    else {
        qDebug() << "Erreur lors de la modification de la scene:" << query.lastError().text();
    }
}

QList<Scene> Scene::getAllScenes()
{
    QList<Scene> scenes;

    QSqlQuery query("SELECT * FROM scene");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();

        Scene scene(id, nom);
        scenes.append(scene);
    }

    return scenes;
}

void Scene::afficherScenes(QListWidget* listWidget)
{
    QSqlQuery query("SELECT * FROM scene");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();

        QListWidgetItem* item = new QListWidgetItem(nom);
        listWidget->addItem(item);
    }
}

int Scene::getSceneId(const QString& sceneName)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM scene WHERE nom = :nom");
    query.bindValue(":nom", sceneName);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    else {
        qDebug() << "Erreur lors de la recuperation de l'ID de la scene : " << query.lastError().text();
        return -1;
    }
}