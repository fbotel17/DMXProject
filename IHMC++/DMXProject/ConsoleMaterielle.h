#pragma once
#include <qobject.h>
#include <qserialport.h>

class ConsoleMaterielle : public QObject
{
	Q_OBJECT

		QSerialPort* port;

public:
	ConsoleMaterielle();
	~ConsoleMaterielle();

private slots:
	void onDataReceived();

signals:
	void channelValueChanged(int);
	//void nextChannel();
	//void previousChannel();

};

