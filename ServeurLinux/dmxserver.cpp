#include "dmxserver.h"
#include "enttecdmxusb.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>


#define NUM_CHANNELS 512 // Nombre de canaux DMX
#define DMXDEVICE "/dev/ttyUSB0"

// Constructeur de la classe DMXServer
DMXServer::DMXServer(QObject *parent)
    : QTcpServer(parent),
      webSocketServer(new QWebSocketServer(QStringLiteral("DMX Server"),
                                           QWebSocketServer::NonSecureMode, this)),
      constructTrame(this) // Initialiser la classe ConstructTrame
{
    // Écouter les connexions entrantes sur le port 12345 pour TCP
    if (!listen(QHostAddress::Any, 12345)) {
        qDebug() << "Erreur : impossible de démarrer le serveur TCP.";
        return;
    }
    qDebug() << "Serveur TCP démarré sur le port 12345.";

    // Écouter les connexions entrantes sur le port 12346 pour WebSocket
    if (webSocketServer->listen(QHostAddress::Any, 12346)) {
        qDebug() << "Serveur WebSocket démarré sur le port 12346.";
        connect(webSocketServer, &QWebSocketServer::newConnection, this, &DMXServer::onNewWebSocketConnection);
    } else {
        qDebug() << "Erreur : impossible de démarrer le serveur WebSocket.";
    }

    // Configuration de la base de données (à adapter selon votre configuration)
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.64.213");
    db.setDatabaseName("testCodeDMX");
    db.setUserName("root");
    db.setPassword("root");

    if (!db.open()) {
        qDebug() << "Erreur : impossible de se connecter à la base de données.";
    }
}

// Destructeur de la classe DMXServer
DMXServer::~DMXServer()
{
    // Fermer le socket du client
    if (clientSocket) {
        clientSocket->close();
    }
    // Fermer tous les WebSocket clients
    qDeleteAll(webSocketClients.begin(), webSocketClients.end());
    webSocketServer->close();
    QSqlDatabase::database().close();
}

// Cette méthode est appelée lorsqu'un nouveau client se connecte au serveur TCP
void DMXServer::incomingConnection(qintptr socketDescriptor)
{
    // Créer un nouveau socket pour le client
    QTcpSocket *clientSocket = new QTcpSocket(this); // Correction: ne plus utiliser un membre de classe pour le clientSocket

    // Définir le descripteur de socket pour le nouveau client
    clientSocket->setSocketDescriptor(socketDescriptor);

    // Configurer les flux d'entrée et de sortie pour le socket du client
    in.setDevice(clientSocket);
    out.setDevice(clientSocket);
    out.setVersion(QDataStream::Qt_5_0);

    // Connecter les signaux et les slots pour lire et écrire des données
    connect(clientSocket, &QTcpSocket::readyRead, this, &DMXServer::readData);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

    qDebug() << "Nouveau client TCP connecté :" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
}

// Cette méthode est appelée lorsqu'un nouveau client se connecte au serveur WebSocket
void DMXServer::onNewWebSocketConnection()
{
    QWebSocket *webSocket = webSocketServer->nextPendingConnection();
    connect(webSocket, &QWebSocket::textMessageReceived, this, &DMXServer::processWebSocketMessage);
    connect(webSocket, &QWebSocket::disconnected, this, &DMXServer::socketDisconnected);
    webSocketClients << webSocket;
    qDebug() << "Nouveau client WebSocket connecté :" << webSocket->peerAddress().toString() << ":" << webSocket->peerPort();
}

// Cette méthode est appelée lorsque des données sont disponibles pour être lues à partir du socket TCP du client
void DMXServer::readData()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) {
        return;
    }

    QByteArray data = clientSocket->readAll();
    bool ok;
    int sceneId = data.toInt(&ok);
    if (ok) {
        processDMXData(sceneId);
    } else {
        qDebug() << "Erreur : ID de scène invalide reçu.";
    }
}

// Cette méthode est appelée lorsqu'un message WebSocket est reçu
void DMXServer::processWebSocketMessage(const QString &message)
{
    // Convertir le message JSON en objet JSON
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
    if (!jsonDocument.isObject()) {
        qDebug() << "Erreur : le message JSON est invalide.";
        return;
    }

    // Extraire l'ID de la scène
    QJsonObject jsonObject = jsonDocument.object();
    if (jsonObject.contains("idScene")) {
        QString sceneIdString = jsonObject.value("idScene").toString();
        bool ok;
        int sceneId = sceneIdString.toInt(&ok);
        if (!ok) {
            qDebug() << "Erreur : l'ID de scène n'est pas un entier valide.";
            return;
        }

        qDebug() << "ID de scène reçu via WebSocket:" << sceneId;

        // Construire la trame DMX en utilisant l'ID de la scène
        QVector<int> dmxValues = constructTrame.buildTrame(sceneId);

        // Configurer les canaux DMX sur l'interface Enttec DMX USB
        EnttecDMXUSB interfaceDMX(DMX_USB_PRO, DMXDEVICE);
        if (interfaceDMX.IsAvailable()) {
            for (int i = 0; i < dmxValues.size(); ++i) {
                interfaceDMX.SetCanalDMX(i + 1, dmxValues[i]);
            }
            interfaceDMX.SendDMX();
        }
    } else {
        qDebug() << "Erreur : le message JSON ne contient pas de clé 'idScene'.";
    }
}


// Traitement des données DMX
void DMXServer::processDMXData(int sceneId)
{
    qDebug() << "ID de scène reçu :" << sceneId;

    // Construire la trame DMX avec l'ID de la scène
    QVector<int> dmxValues = constructTrame.buildTrame(sceneId);

    // Régler les canaux DMX sur l'interface Enttec DMX USB
    EnttecDMXUSB interfaceDMX(DMX_USB_PRO, DMXDEVICE);
    if (interfaceDMX.IsAvailable()) {
        for (int i = 0; i < dmxValues.size(); ++i) {
            interfaceDMX.SetCanalDMX(i + 1, dmxValues[i]);
        }
        interfaceDMX.SendDMX();
    }
}

// Gestion de la déconnexion des WebSocket
void DMXServer::socketDisconnected()
{
    QWebSocket *webSocket = qobject_cast<QWebSocket *>(sender());
    if (webSocket) {
        webSocketClients.removeAll(webSocket);
        webSocket->deleteLater();
        qDebug() << "Client WebSocket déconnecté.";
    }
}
