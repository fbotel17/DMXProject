#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    // Recherche du port série correspondant à votre Arduino
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

    // Ouverture du port série
    QSerialPort serial;
    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud9600);
    if (!serial.open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur: Impossible d ouvrir le port serie.";
        return 1;
    }
    else {
        qDebug() << "Port serie ouvert avec succes.";
    }

    // Lecture des données de la liaison série
    QObject::connect(&serial, &QSerialPort::readyRead, [&]() {
        static QByteArray receivedData;
        receivedData.append(serial.readAll()); // Ajouter les nouvelles données reçues

        // Si les données contiennent un caractère de fin de ligne, traiter la ligne complète
        if (receivedData.contains('\n')) {
            QString dataString = QString::fromUtf8(receivedData).trimmed(); // Convertir les données en chaîne
            QStringList dataList = dataString.split(':'); // Séparer le numéro et le nom de l'option

            // Vérifier que la liste contient au moins deux éléments
            if (dataList.size() >= 2) {
                QString optionNumber = dataList.at(0);
                QString optionName = dataList.at(1);
                qDebug() << "Option selectionnee :" << optionNumber << " - " << optionName;
            }
            else {
                qDebug() << "Erreur: Donnees serie mal formatees.";
            }

            receivedData.clear(); // Effacer les données reçues pour la prochaine lecture
        }
        });

    return a.exec();
}
