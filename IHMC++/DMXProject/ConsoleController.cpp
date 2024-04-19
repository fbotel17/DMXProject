#include "ConsoleController.h"
#include <QDebug>
#include <QJsonDocument>


ConsoleController::ConsoleController(QObject* parent) : QObject(parent)
{
    connect(&m_serialPort, &QSerialPort::readyRead, this, &ConsoleController::processSerialData);
}

ConsoleController::~ConsoleController()
{
    disconnectFromArduino();
}

bool ConsoleController::connectToArduino(const QString& portName)
{
    m_serialPort.setPortName(portName);
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Connected to Arduino on" << portName;
        return true;
    }
    else {
        qDebug() << "Failed to connect to Arduino on" << portName;
        return false;
    }
}

void ConsoleController::disconnectFromArduino()
{
    if (m_serialPort.isOpen()) {
        m_serialPort.close();
        qDebug() << "Disconnected from Arduino";
    }
}

bool ConsoleController::isConnected() const
{
    return m_serialPort.isOpen();
}

void ConsoleController::processSerialData()
{
    QByteArray data = m_serialPort.readAll();
    // Analyze serial data and emit corresponding signals
    // Depending on the received data, emit signals to trigger actions in your application
}

void ConsoleController::sendSceneNames(const QStringList& scenes) {
    if (!isConnected()) {
        qDebug() << "Not connected to Arduino";
        return;
    }

    foreach(const QString & scene, scenes) {
        QByteArray data = "Scene:" + scene.toUtf8() + "\n";
        m_serialPort.write(data);
        if (!m_serialPort.waitForBytesWritten(1000)) {
            qDebug() << "Failed to send scene:" << scene;
        }
    }
}
