#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int joystickX = A2;
int joystickY = A0;
int joystickButton = 7; // Bouton de retour
int backButton = A3;   // Bouton de sélection

int threshold = 100; // Seuil pour la direction du joystick

// Définir les options du menu
String menuOptions[] = {"Banane", "Kiwi", "Orange", "Pomme"};
int selectedOption = 0;

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

  // Détecter les changements d'état du bouton de sélection
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) { // Bouton de sélection enfoncé
      if (showMenu) {
        optionSelected = true;
        showMenu = false;
        displaySelectedOption();
        sendSelectedOption(); // Envoyer l'option sélectionnée via le port série
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

  // Si le menu est affiché et le joystick est déplacé horizontalement
  if (showMenu && directionX != "") {
    if (directionX == "Gauche") {
      moveSelection(-1); // Déplacer la sélection vers la gauche
    } else if (directionX == "Droite") {
      moveSelection(1); // Déplacer la sélection vers la droite
    }
    delay(250); // Délai pour éviter les défilements rapides
  }

  // Afficher le menu ou l'option sélectionnée
  if (showMenu) {
    displayMenu();
  }

  delay(100);
}

// Fonction pour déplacer la sélection du menu
void moveSelection(int direction) {
  selectedOption += direction;
  if (selectedOption < 0) {
    selectedOption = 3; // Si on est au début du menu, boucler vers le bas
  }
  if (selectedOption > 3) {
    selectedOption = 0; // Si on est à la fin du menu, boucler vers le haut
  }
}

// Fonction pour afficher le menu
void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 8); // Déplacer le curseur vers le haut
  display.println("Menu:");
  int startY = 18; // Position verticale de départ pour les options
  int lineHeight = 9; // Hauteur de ligne
  for (int i = 0; i < 4; i++) {
    int yPos = startY + i * lineHeight; // Calculer la position verticale pour chaque option
    if (i == selectedOption) {
      display.setCursor(0, yPos); // Définir la position du curseur
      display.println("> Option " + String(i+1));
    } else {
      display.setCursor(0, yPos);
      display.println("  Option " + String(i+1));
    }
  }
  // Afficher la direction du joystick en bas du menu
  display.setCursor(0, 55);
  display.display();
}

// Fonction pour afficher l'option sélectionnée
void displaySelectedOption() {
  display.clearDisplay();
  display.setTextSize(1); // Réduire la taille du texte
  display.setTextColor(WHITE);
  int yPos = (SCREEN_HEIGHT - 8) / 2; // Centrer verticalement le texte
  int xPos = (SCREEN_WIDTH - (menuOptions[selectedOption].length() * 6)) / 2; // Centrer horizontalement le texte
  display.setCursor(xPos, yPos);
  display.println(menuOptions[selectedOption]);
  display.display();
}

// Fonction pour envoyer l'option sélectionnée via le port série
// Fonction pour envoyer l'option sélectionnée via le port série
void sendSelectedOption() {
  Serial.print(selectedOption + 1); // Envoyer le numéro de l'option
  Serial.print(":"); // Séparateur
  Serial.println(menuOptions[selectedOption]); // Envoyer le nom de l'option
}

