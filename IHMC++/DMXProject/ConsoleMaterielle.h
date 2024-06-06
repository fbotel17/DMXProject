#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSlider>
#include <QDebug>

class ConsoleMaterielle : public QObject
{
    Q_OBJECT

public:
    ConsoleMaterielle(QSlider* slider, QObject* parent = nullptr);
    ~ConsoleMaterielle();
    void sendData(const QByteArray& data);

signals:
    void channelValueChanged(int value);
    void previousChannel();
    void nextChannel();
    void confirmButtonPressed();
    void validateButtonPressed();


private slots:
    void onDataReceived();

private:
    QSerialPort* port;
    QSlider* slider;
};
