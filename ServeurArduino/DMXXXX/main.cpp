#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QRegularExpression>

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    // Cherche le port série correspondant à votre Arduino
    QString portName;
    const auto serialPorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& serialPort : serialPorts) {
        if (serialPort.portName() == "COM8") { // Remplacez "COM8" par le port série de votre Arduino
            portName = serialPort.portName();
            break;
        }
    }

    if (portName.isEmpty()) {
        qDebug() << "Aucun port serie correspondant a Arduino trouve.";
        return 1;
    }

    // Ouvre le port série
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

    // Lecture des données de la liaison série jusqu'à ce que l'application se termine
    QString receivedData; // Variable pour stocker les données reçues

   

    QObject::connect(&serial, &QSerialPort::readyRead, [&]() {
        QByteArray newData = serial.readAll(); // Lire les nouvelles données
        receivedData += QString(newData); // Ajouter les nouvelles données à celles déjà reçues

        // Vérifier si une ligne complète a été reçue
        int newlineIndex = receivedData.indexOf('\n');
        if (newlineIndex != -1) {
            QString voltageString = receivedData.left(newlineIndex).trimmed(); // Extraire la ligne complète
            receivedData = receivedData.mid(newlineIndex + 1); // Supprimer la ligne complète des données reçues

            // Extraire uniquement la partie numérique de la chaîne de caractères
            int indexOfSpace = voltageString.indexOf(' ');
            if (indexOfSpace != -1) {
                voltageString = voltageString.mid(indexOfSpace + 1);
            }

            QRegularExpression regex("^-?[0-9]*\\.?[0-9]+(?:e[0-9]+)?$"); // Expression régulière pour vérifier le format du nombre à virgule flottante
            if (regex.match(voltageString).hasMatch()) {
                bool ok;
                double voltage = voltageString.toDouble(&ok); // Convertir la chaîne en double
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
