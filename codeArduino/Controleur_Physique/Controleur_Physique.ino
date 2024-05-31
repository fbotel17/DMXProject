const int buttonPin = A2; // Le bouton est connecté à A2
int buttonState = 0; // Variable pour stocker l'état du bouton

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(buttonPin, INPUT);
}

void loop() {
  // Lire la valeur du potentiomètre
  int val = analogRead(A3);
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

  // Lire l'état du bouton
  buttonState = digitalRead(buttonPin);

  // Si le bouton est pressé, envoyer "CONFIRM" à l'interface utilisateur
  if (buttonState == HIGH) {
    Serial.println("CONFIRM");
  }

  delay(20);
}
