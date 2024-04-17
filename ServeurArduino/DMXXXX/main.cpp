#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortReader(QObject* parent = nullptr) : QObject(parent) {}

    void start()
    {
        // Recherche du port série correspondant à votre Arduino
        const auto serialPorts = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo& serialPort : serialPorts) {
            if (serialPort.portName() == "COM8") { // Remplacez "COM8" par le port série de votre Arduino
                portName = serialPort.portName();
                break;
            }
        }

        if (portName.isEmpty()) {
            qDebug() << "Aucun port série correspondant à Arduino trouvé.";
            return;
        }

        // Ouverture du port série
        serial.setPortName(portName);
        serial.setBaudRate(QSerialPort::Baud9600);
        if (!serial.open(QIODevice::ReadOnly)) {
            qDebug() << "Erreur: Impossible d'ouvrir le port série.";
            return;
        }
        else {
            qDebug() << "Port série ouvert avec succès.";
        }

        // Lecture des données de la liaison série
        QObject::connect(&serial, &QSerialPort::readyRead, this, &SerialPortReader::readData);
    }

private slots:
    void readData()
    {
        static QByteArray receivedData;
        receivedData.append(serial.readAll()); // Ajouter les nouvelles données reçues

        // Si les données contiennent un caractère de fin de ligne, traiter la ligne complète
        if (receivedData.contains('\n')) {
            QString dataString = QString::fromUtf8(receivedData).trimmed(); // Convertir les données en chaîne
            if (dataString.startsWith("Joystick direction:")) {
                qDebug() << dataString;
            }
            else {
                QStringList dataList = dataString.split(':'); // Séparer le numéro et le nom de l'option

                // Vérifier que la liste contient au moins deux éléments
                if (dataList.size() >= 2) {
                    QString optionNumber = dataList.at(0);
                    QString optionName = dataList.at(1);
                    qDebug() << "Option sélectionnée :" << optionNumber << " - " << optionName;
                }
                else {
                    qDebug() << "Erreur: Données série mal formattées.";
                }
            }
            receivedData.clear(); // Effacer les données reçues pour la prochaine lecture
        }
    }

private:
    QSerialPort serial;
    QString portName;
};

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    SerialPortReader reader;
    reader.start();

    return a.exec();
}

#include "main.moc"
