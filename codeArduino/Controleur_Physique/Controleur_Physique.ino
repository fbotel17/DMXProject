#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int confirmButtonPin = A3;
const int validateButtonPin = 8; // Nouveau bouton sur le port digital D2
int confirmButtonState = 0;
int lastConfirmButtonState = 0;
unsigned long lastConfirmButtonDebounceTime = 0;
unsigned long confirmButtonDebounceDelay = 50;

int validateButtonState = 0;
int lastValidateButtonState = 0;
unsigned long lastValidateButtonDebounceTime = 0;
unsigned long validateButtonDebounceDelay = 50;

unsigned long lastJoystickMoveTime = 0;
const unsigned long joystickMoveDelay = 200; // Délai de 200 ms pour le joystick

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(confirmButtonPin, INPUT);
  pinMode(validateButtonPin, INPUT); // Configurer le nouveau bouton en entrée

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Startup...");
  display.display();
}

void loop() {
  int val = analogRead(A2);
  String str = "V";
  str += val;
  Serial.println(str);

  int joystickX = analogRead(A0);
  int joystickY = analogRead(A1);

  unsigned long currentMillis = millis();
  if (currentMillis - lastJoystickMoveTime >= joystickMoveDelay) {
    if (joystickX < 300) {
      Serial.println("JLEFT");
      lastJoystickMoveTime = currentMillis;
    } else if (joystickX > 700) {
      Serial.println("JRIGHT");
      lastJoystickMoveTime = currentMillis;
    }
  }

  // Lire l'état du bouton de confirmation avec hystérésis et délai de rebond
  if (currentMillis - lastConfirmButtonDebounceTime >= confirmButtonDebounceDelay) {
    int reading = digitalRead(confirmButtonPin);
    if (reading != lastConfirmButtonState) {
      lastConfirmButtonDebounceTime = currentMillis;
      if (reading == HIGH && lastConfirmButtonState == LOW) {
        confirmButtonState = HIGH;
      } else {
        confirmButtonState = LOW;
      }
    }
    lastConfirmButtonState = reading;
  }

  if (confirmButtonState == HIGH) {
    Serial.println("CONFIRM");
    confirmButtonState = LOW;
  }

  // Lire l'état du bouton de validation avec hystérésis et délai de rebond
  if (currentMillis - lastValidateButtonDebounceTime >= validateButtonDebounceDelay) {
    int reading = digitalRead(validateButtonPin);
    if (reading != lastValidateButtonState) {
      lastValidateButtonDebounceTime = currentMillis;
      if (reading == HIGH && lastValidateButtonState == LOW) {
        validateButtonState = HIGH;
      } else {
        validateButtonState = LOW;
      }
    }
    lastValidateButtonState = reading;
  }

  if (validateButtonState == HIGH) {
    Serial.println("VALIDATE");
    validateButtonState = LOW;
}

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

  int lineHeight = 10;
  int yPos = 0;

  int separatorIndex;
  while ((separatorIndex = message.indexOf('\n')) != -1) {
    String line = message.substring(0, separatorIndex);
    display.setCursor(0, yPos);
    display.print(line);
    yPos += lineHeight;
    message = message.substring(separatorIndex + 1);
  }

  display.setCursor(0, yPos);
  display.print(message);

  display.display();
}
