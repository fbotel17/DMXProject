#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSlider>

class ConsoleMaterielle : public QObject
{
    Q_OBJECT

public:
    ConsoleMaterielle(QSlider* slider, QObject* parent = nullptr);
    ~ConsoleMaterielle();

signals:
    void channelValueChanged(int value);

private slots:
    void onDataReceived();

private:
    QSerialPort* port;
    QSlider* slider;
};
