#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buttonPin = A3;
int buttonState = 0;
int lastButtonState = 0; // Nouvelle variable pour l'état précédent du bouton
unsigned long lastButtonDebounceTime = 0; // Nouvelle variable pour stocker le temps du dernier rebond
unsigned long buttonDebounceDelay = 50; // Délai de rebond en millisecondes

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(buttonPin, INPUT);

  // Initialisation de l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adresse I2C 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Boucle infinie si l'écran ne s'initialise pas
  }

  // Efface l'écran
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Startup...");
  display.display();
}

void loop() {
  // Lire la valeur du potentiomètre
  int val = analogRead(A2);
  String str = "V";
  str += val;
  Serial.println(str);

  // Lire les valeurs du joystick
  int joystickX = analogRead(A0);
  int joystickY = analogRead(A1);

  // Détecter le mouvement du joystick vers la gauche ou la droite
  if (joystickX < 300) { // Si la valeur est inférieure à un seuil, mouvement vers la gauche
    Serial.println("JLEFT");
  } else if (joystickX > 700) { // Si la valeur est supérieure à un seuil, mouvement vers la droite
    Serial.println("JRIGHT");
  }

  // Lire l'état du bouton avec hystérésis et délai de rebond
  unsigned long currentMillis = millis();
  if (currentMillis - lastButtonDebounceTime >= buttonDebounceDelay) {
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonState) {
      lastButtonDebounceTime = currentMillis;
      if (reading == HIGH && lastButtonState == LOW) {
        buttonState = HIGH;
      } else {
        buttonState = LOW;
      }
    }
    lastButtonState = reading;
  }

  // Si le bouton est pressé, envoyer "CONFIRM" à l'interface utilisateur
  if (buttonState == HIGH) {
    Serial.println("CONFIRM");
    buttonState = LOW; // Réinitialiser l'état du bouton après l'envoi
  }

  // Lire les messages série et mettre à jour l'écran OLED
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    displayMessage(message);
  }

  delay(20);
}


void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  int lineHeight = 10; // Hauteur de chaque ligne
  int yPos = 0; // Position Y initiale

  // Séparer les messages par ligne
  int separatorIndex;
  while ((separatorIndex = message.indexOf('\n')) != -1) {
    String line = message.substring(0, separatorIndex);
    display.setCursor(0, yPos);
    display.print(line);
    yPos += lineHeight;
    message = message.substring(separatorIndex + 1);
  }
  // Afficher la dernière ligne
  display.setCursor(0, yPos);
  display.print(message);

  display.display();
}
