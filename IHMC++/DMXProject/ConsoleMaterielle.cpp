#include "ConsoleMaterielle.h"

ConsoleMaterielle::ConsoleMaterielle()
{
	port = new QSerialPort("COM8", this);
	QObject::connect(port, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
	port->setBaudRate(9600);
	port->setDataBits(QSerialPort::Data8);
	port->setParity(QSerialPort::NoParity);
	port->open(QIODevice::ReadWrite);
}

ConsoleMaterielle::~ConsoleMaterielle()
{
	port->deleteLater();
}


void ConsoleMaterielle::onDataReceived()
{
	if (port->canReadLine())
	{
		QByteArray data = port->readLine();

		if (data.startsWith("V"))		// Valeur potentiomètre
		{
			QString str = QString::fromUtf8(data);
			QStringRef substr(&str, 1, str.length() - 1);
			QString valueStr = substr.toString();

			int val = valueStr.toInt();
			double dval = val;
			dval = dval / 1024.0 * 255.0;
			val = dval;

			emit channelValueChanged(val);
		}

		else if (data.startsWith("J"))    // Mouvement du joystick
		{
			QString str = QString::fromUtf8(data);
			if (str.contains("LEFT"))
			{
				//emit previousChannel();
			}
			else if (str.contains("RIGHT"))
			{
				//emit nextChannel();
			}
		}
	}
}