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
        // Recherche du port s�rie correspondant � votre Arduino
        const auto serialPorts = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo& serialPort : serialPorts) {
            if (serialPort.portName() == "COM8") { // Remplacez "COM8" par le port s�rie de votre Arduino
                portName = serialPort.portName();
                break;
            }
        }

        if (portName.isEmpty()) {
            qDebug() << "Aucun port s�rie correspondant � Arduino trouv�.";
            return;
        }

        // Ouverture du port s�rie
        serial.setPortName(portName);
        serial.setBaudRate(QSerialPort::Baud9600);
        if (!serial.open(QIODevice::ReadOnly)) {
            qDebug() << "Erreur: Impossible d'ouvrir le port s�rie.";
            return;
        }
        else {
            qDebug() << "Port s�rie ouvert avec succ�s.";
        }

        // Lecture des donn�es de la liaison s�rie
        QObject::connect(&serial, &QSerialPort::readyRead, this, &SerialPortReader::readData);
    }

private slots:
    void readData()
    {
        static QByteArray receivedData;
        receivedData.append(serial.readAll()); // Ajouter les nouvelles donn�es re�ues

        // Si les donn�es contiennent un caract�re de fin de ligne, traiter la ligne compl�te
        if (receivedData.contains('\n')) {
            QString dataString = QString::fromUtf8(receivedData).trimmed(); // Convertir les donn�es en cha�ne
            if (dataString.startsWith("Joystick direction:")) {
                qDebug() << dataString;
            }
            else {
                QStringList dataList = dataString.split(':'); // S�parer le num�ro et le nom de l'option

                // V�rifier que la liste contient au moins deux �l�ments
                if (dataList.size() >= 2) {
                    QString optionNumber = dataList.at(0);
                    QString optionName = dataList.at(1);
                    qDebug() << "Option s�lectionn�e :" << optionNumber << " - " << optionName;
                }
                else {
                    qDebug() << "Erreur: Donn�es s�rie mal formatt�es.";
                }
            }
            receivedData.clear(); // Effacer les donn�es re�ues pour la prochaine lecture
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
