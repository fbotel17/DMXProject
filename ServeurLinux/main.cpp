#include <QCoreApplication>
#include <iostream>
#include <cmath>
#include "enttecdmxusb.h"
#include "dmxserver.h" // Remplacez "dmxserver.h" par le nom de votre fichier d'en-tête de classe serveur


#define DMXDEVICE "/dev/ttyUSB0"
#define NUM_CHANNELS 512 // Nombre de canaux DMX

using namespace std;

// Définition des canaux DMX pour les couleurs
#define CHANNEL_RED   1
#define CHANNEL_GREEN 2
#define CHANNEL_BLUE  3

// Définition des paramètres pour les transitions de couleur
#define TRANSITION_SPEED 0.05 // Vitesse de transition de couleur (en radians par itération)

// Fonction pour régler les canaux DMX pour une couleur spécifique
void setRGBColor(EnttecDMXUSB &interfaceDMX, int r, int g, int b) {
    interfaceDMX.SetCanalDMX(CHANNEL_RED, r);
    interfaceDMX.SetCanalDMX(CHANNEL_GREEN, g);
    interfaceDMX.SetCanalDMX(CHANNEL_BLUE, b);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

        DMXServer server; // Remplacez "DMXServer" par le nom de votre classe serveur

        return a.exec();
}
