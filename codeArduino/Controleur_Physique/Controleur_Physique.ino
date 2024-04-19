#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MAX_SCENES 10 // Définir le nombre maximum de scènes que vous pouvez avoir

String sceneNames[MAX_SCENES]; // Utilisation d'un tableau statique pour stocker les noms des scènes
int totalScenes = 0; // Variable pour suivre le nombre total de scènes

int selectedScene = 0;

int joystickX = A2;
int joystickY = A0;
int joystickButton = 7; // Bouton de retour
int backButton = A3;   // Bouton de sélection

int threshold = 100; // Seuil pour la direction du joystick

bool showMenu = true; // Variable pour suivre si le menu ou l'option sélectionnée doit être affichée
bool optionSelected = false; // Variable pour suivre si une option a été sélectionnée
bool lastButtonState = HIGH; // Variable pour stocker l'état précédent du bouton de sélection

void setup() {
  Serial.begin(9600);

  pinMode(joystickButton, INPUT_PULLUP); // Activer la résistance de tirage sur le bouton de retour
  pinMode(backButton, INPUT_PULLUP);     // Activer la résistance de tirage sur le bouton de sélection

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'allocation SSD1306"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Menu");
  display.display();
}

void loop() {
  int xValue = analogRead(joystickX);
  int yValue = analogRead(joystickY);
  int buttonState = digitalRead(backButton);
  int backState = digitalRead(joystickButton);

  String directionX = ""; // Direction horizontale du joystick

  // Détecter les mouvements horizontaux du joystick
  if (xValue < 512 - threshold) {
    directionX = "Gauche";
  } else if (xValue > 512 + threshold) {
    directionX = "Droite";
  }

  String directionY = ""; // Direction verticale du joystick

  // Détecter les mouvements verticaux du joystick
  if (yValue < 512 - threshold) {
    directionY = "Haut";
  } else if (yValue > 512 + threshold) {
    directionY = "Bas";
  }

  // Si le joystick est déplacé verticalement, changer la sélection de la scène
  if (directionY != "") {
    if (showMenu) {
      if (directionY == "Haut") {
        moveSelection(-1); // Déplacer la sélection vers le haut
      } else if (directionY == "Bas") {
        moveSelection(1); // Déplacer la sélection vers le bas
      }
      displaySelectedScene();
      delay(250); // Délai pour éviter les changements rapides
    }
  }

  // Détecter les changements d'état du bouton de sélection
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) { // Bouton de sélection enfoncé
      if (showMenu) {
        optionSelected = true;
        showMenu = false;
        displaySelectedScene();
        sendSelectedScene(); // Envoyer la scène sélectionnée via le port série
      }
    }
    delay(250); // Délai pour éviter les doubles clics
  }

  // Si le bouton de retour est enfoncé et qu'il n'était pas déjà pressé
  if (backState == LOW && !showMenu) {
    showMenu = true;
    optionSelected = false;
    delay(250); // Délai pour éviter les doubles clics
  }

  // Mettre à jour l'état précédent du bouton de sélection
  lastButtonState = buttonState;

  delay(10);
}

// Fonction pour déplacer la sélection de la scène
void moveSelection(int direction) {
  selectedScene += direction;
  if (selectedScene < 0) {
    selectedScene = totalScenes - 1; // Définir la sélection à la dernière scène si nous sommes au début
  }
  if (selectedScene >= totalScenes) {
    selectedScene = 0; // Définir la sélection à la première scène si nous sommes à la fin
  }
}

// Fonction pour afficher la scène sélectionnée
void displaySelectedScene() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Selected Scene:");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(sceneNames[selectedScene]);
  display.display();
}

// Fonction pour envoyer la scène sélectionnée via le port série
void sendSelectedScene() {
  Serial.println("Scene:" + sceneNames[selectedScene]);
}
