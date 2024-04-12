#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    // Recherche du port s�rie correspondant � votre Arduino
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

    // Ouverture du port s�rie
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

    // Lecture des donn�es de la liaison s�rie
    QObject::connect(&serial, &QSerialPort::readyRead, [&]() {
        static QByteArray receivedData;
        receivedData.append(serial.readAll()); // Ajouter les nouvelles donn�es re�ues

        // Si les donn�es contiennent un caract�re de fin de ligne, traiter la ligne compl�te
        if (receivedData.contains('\n')) {
            QString dataString = QString::fromUtf8(receivedData).trimmed(); // Convertir les donn�es en cha�ne
            QStringList dataList = dataString.split(':'); // S�parer le num�ro et le nom de l'option

            // V�rifier que la liste contient au moins deux �l�ments
            if (dataList.size() >= 2) {
                QString optionNumber = dataList.at(0);
                QString optionName = dataList.at(1);
                qDebug() << "Option selectionnee :" << optionNumber << " - " << optionName;
            }
            else {
                qDebug() << "Erreur: Donnees serie mal formatees.";
            }

            receivedData.clear(); // Effacer les donn�es re�ues pour la prochaine lecture
        }
        });

    return a.exec();
}
