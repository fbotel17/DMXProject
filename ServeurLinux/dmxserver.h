#ifndef DMXSERVER_H
#define DMXSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QDataStream>
#include <QVector>
#include "constructtrame.h" // Ajouter cette ligne

class DMXServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit DMXServer(QObject *parent = nullptr);
    ~DMXServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void readData();
    void processWebSocketMessage(const QString &message);
    void onNewWebSocketConnection();
    void socketDisconnected();

private:
    void processDMXData(int sceneId);
    QTcpSocket *clientSocket = nullptr;
    QWebSocketServer *webSocketServer;
    QList<QWebSocket *> webSocketClients;
    QDataStream in, out;
    ConstructTrame constructTrame; // Ajouter cette ligne
};

#endif // DMXSERVER_H
