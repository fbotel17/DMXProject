#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QRegularExpression>

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    // Cherche le port s�rie correspondant � votre Arduino
    QString portName;
    const auto serialPorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& serialPort : serialPorts) {
        if (serialPort.portName() == "COM8") { // Remplacez "COM8" par le port s�rie de votre Arduino
            portName = serialPort.portName();
            break;
        }
    }

    if (portName.isEmpty()) {
        qDebug() << "Aucun port serie correspondant a Arduino trouve.";
        return 1;
    }

    // Ouvre le port s�rie
    QSerialPort serial;
    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud9600);
    if (!serial.open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur: Impossible d'ouvrir le port serie.";
        return 1;
    }
    else {
        qDebug() << "Port serie ouvert avec succes.";
    }

    // Lecture des donn�es de la liaison s�rie jusqu'� ce que l'application se termine
    QString receivedData; // Variable pour stocker les donn�es re�ues

   

    QObject::connect(&serial, &QSerialPort::readyRead, [&]() {
        QByteArray newData = serial.readAll(); // Lire les nouvelles donn�es
        receivedData += QString(newData); // Ajouter les nouvelles donn�es � celles d�j� re�ues

        // V�rifier si une ligne compl�te a �t� re�ue
        int newlineIndex = receivedData.indexOf('\n');
        if (newlineIndex != -1) {
            QString voltageString = receivedData.left(newlineIndex).trimmed(); // Extraire la ligne compl�te
            receivedData = receivedData.mid(newlineIndex + 1); // Supprimer la ligne compl�te des donn�es re�ues

            // Extraire uniquement la partie num�rique de la cha�ne de caract�res
            int indexOfSpace = voltageString.indexOf(' ');
            if (indexOfSpace != -1) {
                voltageString = voltageString.mid(indexOfSpace + 1);
            }

            QRegularExpression regex("^-?[0-9]*\\.?[0-9]+(?:e[0-9]+)?$"); // Expression r�guli�re pour v�rifier le format du nombre � virgule flottante
            if (regex.match(voltageString).hasMatch()) {
                bool ok;
                double voltage = voltageString.toDouble(&ok); // Convertir la cha�ne en double
                if (ok) {
                    qDebug() << "Voltage recu depuis l Arduino : " << voltageString;
                }
            }
            else {
                qDebug() << "Erreur de format de donnees : " << voltageString;
            }
        }
        });



    return a.exec();
}
